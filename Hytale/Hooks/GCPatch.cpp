#include "GCPatch.h"
#include "Util/Util.h"

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstring>
#include <map>
#include <algorithm>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <winnt.h>

// ═══════════════════════════════════════════════════════════════════════════
// GC Stack-Walker Patch
// ═══════════════════════════════════════════════════════════════════════════
// NativeAOT's GC stack walker (sub_7FF73837EE00) calls sub_7FF73837C650 to
// look up method info for each frame's PC. If the PC is inside our injected
// DLL (not in the NativeAOT method table), it returns 0 and the GC calls
// RaiseFailFastException → crash (0xC0000602).
//
// This patch redirects the FailFast path to a code cave that cleanly sets
// the "done" flag and returns, making the GC skip the unknown frame.
//
// ADDITIONAL: fn_KeepUnwinding calls _1() which invokes RtlVirtualUnwind.
// When unwinding through MinHook trampolines (no .pdata), RtlVirtualUnwind
// crashes with AV at movzx ecx,byte ptr [rbx]. Store-packets (longer hook
// execution) increases the race window. Fix: (1) Safe wrapper for _1() catches
// AV and returns 0; (2) Patch fn_KeepUnwinding's FailFast path to jump to
// a code cave that sets done and returns cleanly.
// ═══════════════════════════════════════════════════════════════════════════
static uint64_t g_gcPatchCave = 0x0;
static uint64_t g_keepUnwindingCave = 0x0;

// Dummy-Struktur für *(a1+0x10): Caller macht mov rax,[ptr]; mov rax,[rax]; movzx r8d,[rax+12].
// [ptr] muss auf lesbares Objekt zeigen; [ptr][0] darf nicht 0 sein (sonst mov rax,[rax] crasht).
static uint64_t g_gcDummyRoot[8];

// ═══════════════════════════════════════════════════════════════════════════
// Fake Code Manager — replaces the real ICodeManager for unknown frames
// ═══════════════════════════════════════════════════════════════════════════
// When the GC encounters our trampoline/DLL frame (method lookup fails),
// we install this fake code manager at a1+0x150. It has a custom vtable:
//   - EnumGcRefs (offset 0x20): no-op → reports zero roots for our native frame
//   - UnwindStackFrame (offset 0x28): manual unwind via RtlLookupFunctionEntry
//   - IsFunclet (offset 0x10): returns 0
// This prevents the GC from scanning stale register locations as managed roots
// AND correctly advances the frame iterator past our trampoline.
//
// REGDISPLAY layout (AMD64, from NativeAOT regdisplay.h):
//   +0x00 pRax, +0x08 pRcx, +0x10 pRdx, +0x18 pRbx,
//   +0x20 pRbp, +0x28 pRsi, +0x30 pRdi,
//   +0x38 pR8,  +0x40 pR9,  +0x48 pR10, +0x50 pR11,
//   +0x58 pR12, +0x60 pR13, +0x68 pR14, +0x70 pR15,
//   +0x78 SP,   +0x80 IP,   +0x88 SSP
// ═══════════════════════════════════════════════════════════════════════════

struct Fp128 {
    uint64_t Low;
    uint64_t High;
};

struct NativeAOTRegDisplay {
    uintptr_t* pRax;
    uintptr_t* pRcx;
    uintptr_t* pRdx;
    uintptr_t* pRbx;
    uintptr_t* pRbp;
    uintptr_t* pRsi;
    uintptr_t* pRdi;
    uintptr_t* pR8;
    uintptr_t* pR9;
    uintptr_t* pR10;
    uintptr_t* pR11;
    uintptr_t* pR12;
    uintptr_t* pR13;
    uintptr_t* pR14;
    uintptr_t* pR15;
    uintptr_t  SP; // Stack pointer (RSP)
    uintptr_t  IP; // Instruction pointer (RIP)
    uintptr_t  SSP; // Shadow Stack Pointer (for Control Flow Guard)
    Fp128      Xmm[16 - 6]; // XMM6-XMM15 (128-bit each, used for floating-point and SIMD operations)
};

static volatile LONG g_fakeUnwindCount = 0;

/**
 * ManualUnwindFrame -- Unwind one stack frame using RtlVirtualUnwind.
 *
 * Populates CONTEXT + KNONVOLATILE_CONTEXT_POINTERS from NativeAOTRegDisplay,
 * calls RtlVirtualUnwind, then writes the unwound IP/SP/nonvolatile-register
 * pointers back into the REGDISPLAY.
 *
 * Precondition: pFunc must be non-null (caller already checked RtlLookupFunctionEntry).
 */
static void ManualUnwindFrame(NativeAOTRegDisplay* pReg, PRUNTIME_FUNCTION pFunc, DWORD64 imageBase) {
    CONTEXT ctx = { };
    KNONVOLATILE_CONTEXT_POINTERS ctxPtrs = { };

    if (pReg->pRbx) ctx.Rbx = *pReg->pRbx;
    if (pReg->pRbp) ctx.Rbp = *pReg->pRbp;
    if (pReg->pRsi) ctx.Rsi = *pReg->pRsi;
    if (pReg->pRdi) ctx.Rdi = *pReg->pRdi;
    if (pReg->pR12) ctx.R12 = *pReg->pR12;
    if (pReg->pR13) ctx.R13 = *pReg->pR13;
    if (pReg->pR14) ctx.R14 = *pReg->pR14;
    if (pReg->pR15) ctx.R15 = *pReg->pR15;
    ctx.Rsp = pReg->SP;
    ctx.Rip = pReg->IP;

    ctxPtrs.Rbx = (PDWORD64) pReg->pRbx;
    ctxPtrs.Rbp = (PDWORD64) pReg->pRbp;
    ctxPtrs.Rsi = (PDWORD64) pReg->pRsi;
    ctxPtrs.Rdi = (PDWORD64) pReg->pRdi;
    ctxPtrs.R12 = (PDWORD64) pReg->pR12;
    ctxPtrs.R13 = (PDWORD64) pReg->pR13;
    ctxPtrs.R14 = (PDWORD64) pReg->pR14;
    ctxPtrs.R15 = (PDWORD64) pReg->pR15;

    PVOID handlerData = nullptr;
    DWORD64 establisher = 0;
    RtlVirtualUnwind(UNW_FLAG_NHANDLER, imageBase, (DWORD64) pReg->IP,
        pFunc, &ctx, &handlerData, &establisher, &ctxPtrs);

    pReg->IP = ctx.Rip;
    pReg->SP = ctx.Rsp;
    if (ctxPtrs.Rbx) pReg->pRbx = (uintptr_t*) ctxPtrs.Rbx;
    if (ctxPtrs.Rbp) pReg->pRbp = (uintptr_t*) ctxPtrs.Rbp;
    if (ctxPtrs.Rsi) pReg->pRsi = (uintptr_t*) ctxPtrs.Rsi;
    if (ctxPtrs.Rdi) pReg->pRdi = (uintptr_t*) ctxPtrs.Rdi;
    if (ctxPtrs.R12) pReg->pR12 = (uintptr_t*) ctxPtrs.R12;
    if (ctxPtrs.R13) pReg->pR13 = (uintptr_t*) ctxPtrs.R13;
    if (ctxPtrs.R14) pReg->pR14 = (uintptr_t*) ctxPtrs.R14;
    if (ctxPtrs.R15) pReg->pR15 = (uintptr_t*) ctxPtrs.R15;
}

// vtable[5] replacement: manual unwind using our registered UNWIND_INFO
static char __fastcall FakeUnwindStackFrame(
    void* self,                    // RCX = this (fake code manager, unused)
    void* pMethodInfo,             // RDX = &method_info data (unused)
    uint32_t flags,                // R8  = unwind flags
    NativeAOTRegDisplay* pReg,     // R9  = REGDISPLAY pointer
    void** ppPrevTransitionFrame)  // [RSP+0x28] = output (PInvokeTransitionFrame**)
{
    // No reverse P/Invoke transition from our native frame
    *ppPrevTransitionFrame = nullptr;

    DWORD64 imageBase = 0;
    PRUNTIME_FUNCTION pFunc = RtlLookupFunctionEntry((DWORD64) pReg->IP, &imageBase, NULL);

    LONG count = InterlockedIncrement(&g_fakeUnwindCount);
    if (count <= 10)
        Util::log("[FakeUnwind] #%d called for IP=0x%llX (flags=0x%X) — %s", count, pReg->IP, flags, pFunc ? "RUNTIME_FUNCTION found" : "no RUNTIME_FUNCTION");

    if (!pFunc) {
        // No RUNTIME_FUNCTION → treat as leaf function (return addr at [RSP])
        uintptr_t retAddr = *(uintptr_t*) pReg->SP;
        pReg->IP = retAddr;
        pReg->SP += 8;
        return 1;
    }

    ManualUnwindFrame(pReg, pFunc, imageBase);

    if (count <= 10)
		Util::log("[FakeUnwind] Unwound to IP=0x%llX, SP=0x%llX", pReg->IP, pReg->SP);

    return 1;
}

// vtable[4] replacement: EnumGcRefs → no roots (our frame is native C++)
static void __fastcall FakeEnumGcRefs(void*, void*, void*, void*, void*, char) {
    // No managed roots in our native C++ hook frame. Do nothing.
}

// vtable[2] replacement: IsFunclet → false
static char __fastcall FakeIsFunclet(void*, void*) { return 0; }

// Generic no-op stubs for fake vtable entries (various arities)
static __int64 __fastcall FakeReturnZero(void*) { return 0; }
static __int64 __fastcall FakeReturnZero2(void*, void*) { return 0; }
static __int64 __fastcall FakeReturnZero3(void*, void*, void*) { return 0; }
static __int64 __fastcall FakeReturnZero4(void*, void*, void*, void*) { return 0; }
static __int64 __fastcall FakeReturnZero5(void*, void*, void*, void*, void*) { return 0; }

// vtable[10] (offset 0x50): EHEnumInit → return false (no exception handlers in our frame)
// This is called during exception dispatch to enumerate catch clauses.
// Returning false tells the runtime: "this frame has no exception handlers, skip it."
static char __fastcall FakeEHEnumInit(void*, void*, void*, void*) { return 0; }

// vtable[11] (offset 0x58): EHEnumNext → return false (no more exception handlers)
static char __fastcall FakeEHEnumNext(void*, void*) { return 0; }

// Fake vtable — FULL ICodeManager layout (no vdtor, no GetResumeSp/GetReturnValueKind on x64):
//   [0x00] IsSafePoint              [0x08] FindMethodInfo
//   [0x10] IsFunclet                [0x18] GetFramePointer
//   [0x20] EnumGcRefs               [0x28] UnwindStackFrame
//   [0x30] GetConservativeUB        [0x38] IsUnwindable
//   [0x40] GetReturnAddressHijack   [0x48] RemapHardwareFault
//   [0x50] EHEnumInit               [0x58] EHEnumNext
//   [0x60] GetMethodStartAddress    [0x68] GetOsModuleHandle
//   [0x70] GetClasslibFunction      [0x78] GetAssociatedData
static uintptr_t g_fakeVtable[16] = {
    (uintptr_t) &FakeReturnZero2,         // 0x00: IsSafePoint → false
    (uintptr_t) &FakeReturnZero3,         // 0x08: FindMethodInfo → false
    (uintptr_t) &FakeIsFunclet,           // 0x10: IsFunclet → false
    (uintptr_t) &FakeReturnZero3,         // 0x18: GetFramePointer → 0
    (uintptr_t) &FakeEnumGcRefs,          // 0x20: EnumGcRefs → no roots
    (uintptr_t) &FakeUnwindStackFrame,    // 0x28: UnwindStackFrame → manual unwind
    (uintptr_t) &FakeReturnZero2,         // 0x30: GetConservativeUpperBound → 0
    (uintptr_t) &FakeReturnZero2,         // 0x38: IsUnwindable → false
    (uintptr_t) &FakeReturnZero4,         // 0x40: GetReturnAddressHijackInfo → 0
    (uintptr_t) &FakeReturnZero2,         // 0x48: RemapHardwareFaultToGCSafePoint → 0
    (uintptr_t) &FakeEHEnumInit,          // 0x50: EHEnumInit → false (no handlers)
    (uintptr_t) &FakeEHEnumNext,          // 0x58: EHEnumNext → false (no more)
    (uintptr_t) &FakeReturnZero,          // 0x60: GetMethodStartAddress → 0
    (uintptr_t) &FakeReturnZero,          // 0x68: GetOsModuleHandle → 0
    (uintptr_t) &FakeReturnZero3,         // 0x70: GetClasslibFunction → 0
    (uintptr_t) &FakeReturnZero2,         // 0x78: GetAssociatedData → 0
};

// Fake code manager object: first QWORD = vtable pointer (C++ object layout)
static uintptr_t g_fakeCodeManagerObj = (uintptr_t) &g_fakeVtable[0];

// ── SafeUnwind1: wraps the NativeAOT UnwindStackFrame (vtable[5]) call ──
// The jump pad saves the vtable[5] function pointer (RAX) here before jumping to us.
// We call the target directly, catching any AV from mismatched RUNTIME_FUNCTION.
// On crash: use RtlLookupFunctionEntry (which finds our RtlAddFunctionTable entries)
// + RtlVirtualUnwind to manually advance the frame.
static volatile uintptr_t g_savedVtableTarget = 0;
static volatile LONG g_safeUnwind1FallbackCount = 0;

// vtable[5] (UnwindStackFrame) has 5 parameters:
//   RCX = codeManager, RDX = &(a1+0x158), R8 = flags, R9 = &(a1+0x20),
//   [RSP+0x28] = &(a1+0x218) (output pointer for handler data).
// fn_KeepUnwinding saves r14 (= a1+0x218) at [rsp+0x20] in its prologue,
// which becomes the 5th param at [RSP+0x28] after the `call` pushes a return address.
// Our jump pad uses `jmp` (not `call`), so the stack layout is identical.
typedef char(__fastcall* VtableUnwindFn_t)(__int64, __int64, __int64, __int64, uintptr_t*);

static char __fastcall SafeUnwind1(__int64 codeManager, __int64 ctx_ptr,
    __int64 flags, __int64 regdisplay,
    uintptr_t* output) {
    uintptr_t target = g_savedVtableTarget;
    if (!target) return 0;

    // Save for __except handler (R9 = a1+0x20 = REGDISPLAY ptr)
    volatile __int64 saved_regdisplay = regdisplay;

    __try {
        return ((VtableUnwindFn_t) target)(codeManager, ctx_ptr, flags, regdisplay, output);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        // vtable[5] crashed — stale method_info produced bad RUNTIME_FUNCTION.
        // Fall back to manual unwind using our registered UNWIND_INFO.
        LONG count = InterlockedIncrement(&g_safeUnwind1FallbackCount);
        if (count <= 5)
            Util::log("[SafeUnwind1] WARNING: UnwindStackFrame AV at IP=0x%llX, flags=0x%llX — falling back to manual unwind (count=%d)", saved_regdisplay + 0x80, flags, count);

        NativeAOTRegDisplay* pReg = reinterpret_cast<NativeAOTRegDisplay*>(saved_regdisplay);
        __int64 a1 = saved_regdisplay - 0x20;

        // Clear the output field (vtable[5] does *a5 = 0 as first thing)
        *reinterpret_cast<uintptr_t*>(a1 + 0x218) = 0;

        DWORD64 imageBase = 0;
        PRUNTIME_FUNCTION pFunc = RtlLookupFunctionEntry((DWORD64) pReg->IP, &imageBase, NULL);
        if (pFunc) {
            ManualUnwindFrame(pReg, pFunc, imageBase);
            return 1;
        }

        // No RUNTIME_FUNCTION found — stop scanning this thread (last resort)
        if (count <= 5)
			Util::log("[SafeUnwind1] WARNING: RtlLookupFunctionEntry failed for IP=0x%llX — stopping thread scan (count=%d)", pReg->IP, count);

        *reinterpret_cast<uintptr_t*>(a1 + 0xA0) = 0;
        *reinterpret_cast<uint32_t*>(a1 + 0x1B8) |= 0x10;  // done flag
        return 1;
    }
}

bool PatchGCStackWalker() {
    // ═══════════════════════════════════════════════════════════════════════
    // We patch 27 bytes in sub_7FF73837EE00 (the GC method-lookup function).
    //
    // Original code (27 bytes at offsets EEB4-EECE):
    //   EEB4: 48 89 06           mov [rsi], rax        ; store lookup result
    //   EEB7: 4C 8B FE           mov r15, rsi
    //   EEBA: 48 85 C0           test rax, rax
    //   EEBD: 75 13              jnz short +13h        ; jump if found
    //   EEBF: 33 D2              xor edx, edx          ; FailFast setup
    //   EEC1: 33 C9              xor ecx, ecx
    //   EEC3: 41 B8 01 00 00 00  mov r8d, 1
    //   EEC9: FF 15 xx xx xx xx  call [RaiseFailFastException]
    //
    // Patched code:
    //   EEB4: 48 85 C0           test rax, rax         ; check result FIRST
    //   EEB7: 74 09              jz short EEC2          ; if null → skip store, go to cave
    //   EEB9: 48 89 06           mov [rsi], rax        ; store only if valid
    //   EEBC: 4C 8B FE           mov r15, rsi
    //   EEBF: EB 11              jmp short EED2         ; continue to success path
    //   EEC1: 90                 nop
    //   EEC2: E9 [cave_offset]   jmp code_cave          ; → clean "done" return
    //   EEC7: 90 90 90 90 90 90 90 90                   ; dead code / nops
    //
    // The code cave installs a FAKE CODE MANAGER at *(a1+0x150). This fake
    // code manager has a custom vtable where EnumGcRefs is a no-op (reports
    // zero roots) and UnwindStackFrame does manual unwind via RtlLookupFunctionEntry.
    // This prevents the GC from scanning stale register locations as managed roots.
    // ═══════════════════════════════════════════════════════════════════════

    // AOB: mov [rsi],rax; mov r15,rsi; test rax,rax; jnz +13h
    // 11 bytes — short enough to survive patches, unique enough (1 match in HytaleClient.exe)
    const char* failFastPattern =
        "48 89 06"    // mov [rsi], rax
        " 4C 8B FE"  // mov r15, rsi
        " 48 85 C0"  // test rax, rax
        " 75 13";    // jnz short +0x13

    uint64_t found = Util::PatternScan(failFastPattern);
    if (!found) {
		Util::log("WARNING: GC patch failed! FailFast pattern not found in HytaleClient.exe");
        return false;
    }

    // found = EEB4 equivalent (start of mov [rsi], rax)
    uint64_t patchBase = found;  // 27 bytes to patch: EEB4 through EECE

	Util::log("[GC-Patch] Found pattern at: 0x%llX", found);

    // Allocate executable code cave near the patch site (within ±2GB for rel32)
    uintptr_t targetAddr = (uintptr_t) patchBase;
    g_gcPatchCave = 0x0;
    for (uintptr_t offset = 0x10000; offset < 0x7FFF0000; offset += 0x10000) {
        g_gcPatchCave = (uint64_t) VirtualAlloc(
            (void*) (targetAddr - offset), 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (g_gcPatchCave) break;
        g_gcPatchCave = (uint64_t) VirtualAlloc(
            (void*) (targetAddr + offset), 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (g_gcPatchCave) break;
    }
    if (!g_gcPatchCave) {
		Util::log("WARNING: GC patch failed! Could not allocate code cave near patch site");
        return false;
    }

    // ─── Build code cave assembly ───
    // Register state when entering the cave:
    //   rax = 0 (lookup failed), rbx = a1 (context), rdi = a1+0x1B8 (flags ptr)
    //   *(a1+0x150) has the REAL code manager from the previous frame
    //   Stack: push rbx; push rdi; sub rsp,38h; saved r15@[rsp+20h] r14@[rsp+28h]
    //          r13@[rsp+30h] rbp@[rsp+50h] rsi@[rsp+58h] r12@[rsp+60h]
    //
    // FIX: Replace the real code manager with our FAKE code manager.
    // This ensures EnumGcRefs reports ZERO roots for our native frame (preventing
    // the GC from scanning native C++ values as managed object pointers), and
    // UnwindStackFrame does a correct manual unwind via RtlLookupFunctionEntry.
    //
    // Also set *(a1+0x10) to g_gcDummyRoot so the caller doesn't crash on deref.
    g_gcDummyRoot[0] = (uint64_t) &g_gcDummyRoot;  // [ptr][0] zeigt auf sich selbst (lesbar)

    int idx = 0;
    auto emit = [&](std::initializer_list<uint8_t> bytes) {
        for (uint8_t b : bytes) ((uint8_t*)g_gcPatchCave)[idx++] = b;
    };

    // ── Install fake code manager at a1+0x150 ──
    // mov rax, &g_fakeCodeManagerObj
    emit({ 0x48, 0xB8 });
    for (int i = 0; i < 8; i++) ((uint8_t*)g_gcPatchCave)[idx++] = (uint8_t) ((uintptr_t) &g_fakeCodeManagerObj >> (i * 8));
    // mov [rbx+150h], rax                   ; *(a1+0x150) = fake code manager
    emit({ 0x48, 0x89, 0x83, 0x50, 0x01, 0x00, 0x00 });

    // ── Set dummy root pointer at a1+0x10 ──
    // mov rax, &g_gcDummyRoot
    emit({ 0x48, 0xB8 });
    for (int i = 0; i < 8; i++) ((uint8_t*)g_gcPatchCave)[idx++] = (uint8_t) ((uintptr_t) &g_gcDummyRoot >> (i * 8));
    // mov [rbx+10h], rax                    ; *(a1+0x10) = gültiger Dummy-Ptr
    emit({ 0x48, 0x89, 0x43, 0x10 });

    // ── Set MethodStateCalculated flag ──
    // or dword ptr [rdi], 10h               ; flags |= 0x10
    emit({ 0x83, 0x0F, 0x10 });

    // ── Restore callee-saved registers and return from sub_7FF73837EE00 ──
    emit({ 0x4C, 0x8B, 0x7C, 0x24, 0x20 }); // mov r15, [rsp+20h]
    emit({ 0x4C, 0x8B, 0x74, 0x24, 0x28 }); // mov r14, [rsp+28h]
    emit({ 0x4C, 0x8B, 0x6C, 0x24, 0x30 }); // mov r13, [rsp+30h]
    emit({ 0x4C, 0x8B, 0x64, 0x24, 0x60 }); // mov r12, [rsp+60h]
    emit({ 0x48, 0x8B, 0x6C, 0x24, 0x50 }); // mov rbp, [rsp+50h]
    emit({ 0x48, 0x8B, 0x74, 0x24, 0x58 }); // mov rsi, [rsp+58h]
    emit({ 0x48, 0x83, 0xC4, 0x38 });        // add rsp, 38h
    emit({ 0x5F });                            // pop rdi
    emit({ 0x5B });                            // pop rbx
    emit({ 0xC3 });                            // ret

	Util::log("[GC-Patch] Built code cave at 0x%llX (size: %d bytes)", g_gcPatchCave, idx);

    // ─── Patch the original 27 bytes ───
    // Calculate jump offset for the cave redirect (from EEC2+5 to cave)
    uint64_t caveJmpSrc = patchBase + 14;  // EEC2 = EEB4 + 14
    intptr_t relOffset = (intptr_t) g_gcPatchCave - (intptr_t) (caveJmpSrc + 5);

    if (relOffset > INT32_MAX || relOffset < INT32_MIN) {
		Util::log("WARNING: GC patch failed! Code cave too far for rel32 jump (offset=0x%llX)", relOffset);
        VirtualFree((void*)g_gcPatchCave, 0, MEM_RELEASE);
        g_gcPatchCave = 0;
        return false;
    }

    DWORD oldProt;
    VirtualProtect((void*)patchBase, 27, PAGE_EXECUTE_READWRITE, &oldProt);

    // New bytes (27 total):
    uint8_t patch[27] = {
        0x48, 0x85, 0xC0,             // +0:  test rax, rax
        0x74, 0x09,                    // +3:  jz short +9 → offset 14 (cave jmp)
        0x48, 0x89, 0x06,             // +5:  mov [rsi], rax  (store only if valid)
        0x4C, 0x8B, 0xFE,             // +8:  mov r15, rsi
        0xEB, 0x11,                    // +11: jmp short +11h → offset 24 (= original EED2)
        0x90,                          // +13: nop (alignment)
        0xE9, 0x00, 0x00, 0x00, 0x00, // +14: jmp code_cave (rel32, filled below)
        0x90, 0x90, 0x90, 0x90,       // +19: nops (dead code)
        0x90, 0x90, 0x90, 0x90        // +23: nops
    };

    // Fill in the relative offset for the cave jump
    *(int32_t*) (patch + 15) = (int32_t) relOffset;

    // jmp short at +11: next IP = +13, target = +13 + 0x11 = +30 = patchBase+0x1E = EED2 ✓
    // jz  short at +3:  next IP = +5,  target = +5 + 9    = +14 = cave jmp ✓

    memcpy((void*)patchBase, patch, 27);

    VirtualProtect((void*)patchBase, 27, oldProt, &oldProt);

	Util::log("[GC-Patch] Patched original code at 0x%llX → cave at 0x%llX", patchBase, g_gcPatchCave);
    return true;
}

// ═══════════════════════════════════════════════════════════════════════════
// fn_KeepUnwinding RtlVirtualUnwind AV Fix (Store-Packets crash)
// ═══════════════════════════════════════════════════════════════════════════
bool PatchGCStackWalkerKeepUnwinding() {
    // Pattern: mov rax,[rcx]; mov rax,[rax+28h]; call [rip+rel32]; test al,al; jnz short
    // First _1() call in fn_KeepUnwinding. CE/IDA: jnz is 75 10 (not 75 12) in current build.
    const char* pattern = "48 8B 01 48 8B 40 28 FF 15 ?? ?? ?? ?? 84 C0 75 ??";
    uint64_t found = Util::PatternScan(pattern);
    if (!found) {
		Util::log("[GC-Patch2] KeepUnwinding pattern not found (Store-Packets may crash)");
        return false;
    }

    // Cast to uint8_t* for pointer arithmetic
    uint8_t* foundPtr = (uint8_t*)found;

    // GOT entry: RIP after call (foundPtr+13) + rel32 at foundPtr+9
    // We read it just for validation — we don't call _1() ourselves.
    int32_t rel32 = *reinterpret_cast<int32_t*>(foundPtr + 9);
    void** gotEntry = reinterpret_cast<void**>(foundPtr + 13 + rel32);
    if (!*gotEntry) {
		Util::log("[GC-Patch2] WARNING: GOT entry for _1() is null at 0x%llX", (uintptr_t) gotEntry);
        return false;
    }

    // Call-Site-Patch: replace `call [rip+rel32]` (6 bytes) with `call jumpPad` (5 bytes + NOP)
    uint8_t* callInstr = foundPtr + 7;  // 48 8B 01 (3) + 48 8B 40 28 (4) = 7. FF 15 starts here.

    // FailFast block starts after: test al,al (2) + jnz (2) = 4 bytes past foundPtr+13
    uint8_t* failFastBlock = foundPtr + 13;  // test al,al
    uint8_t* patchStart = failFastBlock + 4;  // start of xor edx,edx block

    uintptr_t targetAddr = (uintptr_t) failFastBlock;
    g_keepUnwindingCave = 0;
    for (uintptr_t offset = 0x10000; offset < 0x7FFF0000; offset += 0x10000) {
        g_keepUnwindingCave = (uint64_t) VirtualAlloc(
            (void*) (targetAddr - offset), 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (g_keepUnwindingCave) break;
        g_keepUnwindingCave = (uint64_t) VirtualAlloc(
            (void*) (targetAddr + offset), 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (g_keepUnwindingCave) break;
    }
    if (!g_keepUnwindingCave) {
		Util::log("[GC-Patch2] KeepUnwinding code cave alloc failed");
        return false;
    }

    // ── Jump pad (cave+0x100): saves RAX (vtable[5] target) then jumps to SafeUnwind1 ──
    // Before the call site, the game does: mov rax,[rcx]; mov rax,[rax+28h]
    // so RAX = vtable[5] function pointer. We must NOT clobber it before saving.
    // Use R10 (volatile, not a parameter register) to address the global.
    //
    // 49 BA <imm64>       mov r10, &g_savedVtableTarget   (10 bytes)
    // 49 89 02             mov [r10], rax                   (3 bytes)
    // 48 B8 <imm64>       mov rax, &SafeUnwind1            (10 bytes)
    // FF E0                jmp rax                          (2 bytes)
    // Total: 25 bytes
    uint8_t* jumpPad = (uint8_t*)g_keepUnwindingCave + 0x100;
    int jp = 0;
    jumpPad[jp++] = 0x49; jumpPad[jp++] = 0xBA;  // mov r10, imm64
    *reinterpret_cast<uintptr_t*>(jumpPad + jp) = (uintptr_t) &g_savedVtableTarget;
    jp += 8;
    jumpPad[jp++] = 0x49; jumpPad[jp++] = 0x89; jumpPad[jp++] = 0x02;  // mov [r10], rax
    jumpPad[jp++] = 0x48; jumpPad[jp++] = 0xB8;  // mov rax, imm64
    *reinterpret_cast<uintptr_t*>(jumpPad + jp) = (uintptr_t) &SafeUnwind1;
    jp += 8;
    jumpPad[jp++] = 0xFF; jumpPad[jp++] = 0xE0;  // jmp rax

    intptr_t callRel32 = (intptr_t) jumpPad - (intptr_t) (callInstr + 5);

    DWORD oldProt;
    if (!VirtualProtect(callInstr, 6, PAGE_EXECUTE_READWRITE, &oldProt)) {
		Util::log("[GC-Patch2] WARNING: VirtualProtect failed for call-site patch");
        return false;
    }
    callInstr[0] = 0xE8;  // call rel32
    *reinterpret_cast<int32_t*>(callInstr + 1) = (int32_t) callRel32;
    callInstr[5] = 0x90;  // NOP (replaces 6th byte of FF 15 xx xx xx xx)
    VirtualProtect(callInstr, 6, oldProt, &oldProt);
    Util::log("[GC-Patch2] Patched SafeUnwind1 call-site at 0x%llX → jump pad at 0x%llX",
		(uintptr_t) callInstr, (uintptr_t) jumpPad);

    // ── FailFast code cave (cave+0x00): safety net if SafeUnwind1 returns 0 ──
    // At this point we're inside fn_KeepUnwinding's stack frame.
    // rbx = a1 (StackFrameIterator context).
    //
    // Register save locations (verified via IDA disassembly of fn_KeepUnwinding):
    //   Prologue: push rbx; push r12; sub rsp, 48h
    //   Saves:    r15 @ [rsp+30h], r14 @ [rsp+38h], r13 @ [rsp+40h]
    //             rbp @ [rsp+60h], rsi @ [rsp+68h], rdi @ [rsp+70h]
    //   Pushed:   r12 @ [rsp+48h] (pop r12), rbx @ [rsp+50h] (pop rbx)
    //   Epilogue: add rsp, 48h; pop r12; pop rbx; ret
    int idx = 0;
    auto emit = [&](std::initializer_list<uint8_t> bytes) {
        for (uint8_t b : bytes) ((uint8_t*)g_keepUnwindingCave)[idx++] = b;
    };
    emit({ 0x83, 0x8B, 0xB8, 0x01, 0x00, 0x00, 0x10 });  // or dword ptr [rbx+1B8h], 10h (done)
    emit({ 0x4C, 0x8B, 0x7C, 0x24, 0x30 });              // mov r15, [rsp+30h]
    emit({ 0x4C, 0x8B, 0x74, 0x24, 0x38 });              // mov r14, [rsp+38h]
    emit({ 0x4C, 0x8B, 0x6C, 0x24, 0x40 });              // mov r13, [rsp+40h]
    emit({ 0x48, 0x8B, 0x7C, 0x24, 0x70 });              // mov rdi, [rsp+70h]
    emit({ 0x48, 0x8B, 0x74, 0x24, 0x68 });              // mov rsi, [rsp+68h]
    emit({ 0x48, 0x8B, 0x6C, 0x24, 0x60 });              // mov rbp, [rsp+60h]
    emit({ 0x48, 0x83, 0xC4, 0x48 });                    // add rsp, 48h
    emit({ 0x41, 0x5C });                                  // pop r12
    emit({ 0x5B });                                        // pop rbx
    emit({ 0xC3 });                                        // ret

    // Patch: test al,al; jnz +0x12; jmp rel32 cave (replace xor/call FailFast)
    intptr_t relOffset = (intptr_t) g_keepUnwindingCave - (intptr_t) (patchStart + 5);
    if (relOffset > INT32_MAX || relOffset < INT32_MIN) {
        VirtualFree((void*)g_keepUnwindingCave, 0, MEM_RELEASE);
        g_keepUnwindingCave = 0;
        return false;
    }

    VirtualProtect(patchStart, 16, PAGE_EXECUTE_READWRITE, &oldProt);
    patchStart[0] = 0xE9;  // jmp rel32
    *reinterpret_cast<int32_t*>(patchStart + 1) = (int32_t) relOffset;
    memset(patchStart + 5, 0x90, 11);  // nops over rest of FailFast block (xor/call = 16 bytes)
    VirtualProtect(patchStart, 16, oldProt, &oldProt);

	Util::log("[GC-Patch2] Patched KeepUnwinding FailFast block at 0x%llX → cave at 0x%llX", (uintptr_t) patchStart, (uintptr_t) g_keepUnwindingCave);
    return true;
}

// ═══════════════════════════════════════════════════════════════════════════
// RegisterTrampolinePage
// ═══════════════════════════════════════════════════════════════════════════
#include <map>
#include <algorithm>
#include <winnt.h>

#pragma pack(push, 1)
struct DUMMY_UNWIND_INFO {
    uint8_t VersionAndFlags;
    uint8_t SizeOfProlog;
    uint8_t CountOfCodes;
    uint8_t FrameRegisterAndOffset;
};

typedef union _UNWIND_CODE {
    struct {
        uint8_t CodeOffset;
        uint8_t UnwindOp : 4;
        uint8_t OpInfo : 4;
    };
    uint16_t FrameOffset;
} UNWIND_CODE;
#pragma pack(pop)

// MinHook always appends a JMP to the original function at the end of the copied bytes.
// By finding this JMP, we can deduct exactly how many bytes of the original function's 
// prologue were copied into the trampoline.
static uint32_t GetCopiedSize(uint8_t* pTrampoline, uint8_t* pOriginal) {
    for (uint32_t i = 5; i < 28; i++) {
        if (pTrampoline[i] == 0xE9) { // JMP rel32
            int32_t rel32 = *(int32_t*) (pTrampoline + i + 1);
            if ((uintptr_t) (pTrampoline + i + 5 + rel32) == (uintptr_t) (pOriginal + i)) {
                return i;
            }
        } else if (pTrampoline[i] == 0xFF && pTrampoline[i + 1] == 0x25) { // JMP [RIP+...]
            int32_t rel32 = *(int32_t*) (pTrampoline + i + 2);
            uintptr_t dataAddr = (uintptr_t) (pTrampoline + i + 6 + rel32);
            // Safely read the target address embedded in the MinHook block
            if (dataAddr >= (uintptr_t) pTrampoline && dataAddr < (uintptr_t) pTrampoline + 64) {
                uintptr_t target = *(uintptr_t*) dataAddr;
                if (target == (uintptr_t) (pOriginal + i)) {
                    return i;
                }
            }
        }
    }
    return 0; // fallback if not found
}

// Structure to hold hook metadata before finalizing the function table
struct HookTrampData {
    uint32_t beginRva;
    uint32_t endRva;
    uint32_t unwindInfoRva;
};

void RegisterAllTrampolinePages(const std::vector<std::pair<void*, void*>>& hooks) {
    if (hooks.empty()) return;

    // Group hooks by their MinHook 4KB page
    std::map<uintptr_t, std::vector<std::pair<void*, void*>>> hooksByPage;
    for (const auto& h : hooks) {
        if (!h.first || !h.second) continue;
        uintptr_t pageBase = (uintptr_t) h.first & ~0xFFF;
        hooksByPage[pageBase].push_back(h);
    }

    for (auto& pair : hooksByPage) {
        uintptr_t pageBase = pair.first;
        auto& pageHooks = pair.second;

        std::vector<HookTrampData> trampData;

        DWORD oldProt;
        VirtualProtect((void*) pageBase, 4096, PAGE_EXECUTE_READWRITE, &oldProt);

        // Start allocating UNWIND_INFO at 0x400 (middle of the page, trampolines grow down from 0, relays up from 0xFFF)
        size_t allocOffset = 0x400;

        for (const auto& h : pageHooks) {
            void* pTrampoline = h.first;
            void* pOriginal = h.second;

            // Find UNWIND_INFO of the target original function
            DWORD64 ImageBase;
            PRUNTIME_FUNCTION pOrigRuntimeFunc = RtlLookupFunctionEntry((DWORD64) pOriginal, &ImageBase, NULL);
            if (!pOrigRuntimeFunc) {
				Util::log("[Hooks] WARNING: No RUNTIME_FUNCTION found for 0x%p (trampoline at 0x%p)", pOriginal, pTrampoline);
                // Even without UNWIND_INFO, it's safer to register a leaf function than crash.
                continue;
            }

            uint8_t* pOrigUnwindInfo = (uint8_t*) (ImageBase + pOrigRuntimeFunc->UnwindInfoAddress);

            uint8_t versionFlags = pOrigUnwindInfo[0];
            uint8_t countOfCodes = pOrigUnwindInfo[2];

            uint32_t copiedSize = GetCopiedSize((uint8_t*) pTrampoline, (uint8_t*) pOriginal);
            if (copiedSize == 0) {
				Util::log("[Hooks] WARNING: Could not determine copied size for 0x%p (trampoline at 0x%p), defaulting to 32 bytes", pOriginal, pTrampoline);
                copiedSize = 32; // Fallback bound
            }

            UNWIND_CODE* origCodes = (UNWIND_CODE*) (pOrigUnwindInfo + 4);
            UNWIND_CODE newCodes[128];
            uint8_t newCount = 0;
            bool hasSetFpReg = false;

            for (uint8_t i = 0; i < countOfCodes; ) {
                UNWIND_CODE code = origCodes[i];
                uint8_t slots = 1;
                uint8_t op = code.UnwindOp;

                if (op == 1) { // UWOP_ALLOC_LARGE
                    slots = (code.OpInfo == 0) ? 2 : 3;
                } else if (op == 4 || op == 8 || op == 6) { // UWOP_SAVE_NONVOL, UWOP_SAVE_XMM128, UWOP_EPILOG
                    slots = 2;
                } else if (op == 5 || op == 9 || op == 7) { // UWOP_SAVE_NONVOL_FAR, UWOP_SAVE_XMM128_FAR, UWOP_SPARE_CODE
                    slots = 3;
                }

                // Keep the operation ONLY if it was executed during the copied sequence
                if (code.CodeOffset <= copiedSize) {
                    if (op == 3) { // UWOP_SET_FPREG
                        hasSetFpReg = true;
                    }
                    for (uint8_t j = 0; j < slots; j++) {
                        newCodes[newCount++] = origCodes[i + j];
                    }
                }
                i += slots;
            }

            size_t unwindInfoSize = 4 + (newCount * 2);
            if (unwindInfoSize % 4 != 0) unwindInfoSize += 2;

            uint8_t* pNewUnwindInfo = (uint8_t*) (pageBase + allocOffset);

            pNewUnwindInfo[0] = versionFlags & 0x07;                // Filter flags
            pNewUnwindInfo[1] = (uint8_t) copiedSize;                // Adjust Prolog definition
            pNewUnwindInfo[2] = newCount;                           // Use updated code count
            pNewUnwindInfo[3] = hasSetFpReg ? pOrigUnwindInfo[3] : 0; // Clear FrameRegister if uncopied!

            memcpy(pNewUnwindInfo + 4, newCodes, newCount * 2);     // Copy precise codes

            HookTrampData td;
            td.beginRva = (uint32_t) ((uintptr_t) pTrampoline - pageBase);
            // 32 bytes safely covers the copied prologue bytes in the trampoline
            td.endRva = td.beginRva + 32;
            td.unwindInfoRva = (uint32_t) allocOffset;

            trampData.push_back(td);

            allocOffset += unwindInfoSize;
            if (allocOffset % 4 != 0) allocOffset += (4 - (allocOffset % 4));
        }

        // Sort trampolines by BeginAddress, required by RtlAddFunctionTable
        std::sort(trampData.begin(), trampData.end(), [](const HookTrampData& a, const HookTrampData& b) {
            return a.beginRva < b.beginRva;
        });

        // Add Relay Stubs as a Leaf Function at the end of the page (0xC00 - 0xFFF)
        DUMMY_UNWIND_INFO* pRelayUnwindInfo = (DUMMY_UNWIND_INFO*) (pageBase + allocOffset);
        pRelayUnwindInfo->VersionAndFlags = 1;
        pRelayUnwindInfo->SizeOfProlog = 0;
        pRelayUnwindInfo->CountOfCodes = 0;
        pRelayUnwindInfo->FrameRegisterAndOffset = 0;

        uint32_t relayUnwindRva = (uint32_t) allocOffset;
        allocOffset += sizeof(DUMMY_UNWIND_INFO);

        // Build the RUNTIME_FUNCTION array
        RUNTIME_FUNCTION* pRuntimeFuncArray = (RUNTIME_FUNCTION*) (pageBase + allocOffset);
        size_t count = trampData.size() + 1; // +1 for the Relay Stubs block

        for (size_t i = 0; i < trampData.size(); i++) {
            pRuntimeFuncArray[i].BeginAddress = trampData[i].beginRva;
            pRuntimeFuncArray[i].EndAddress = trampData[i].endRva;
            pRuntimeFuncArray[i].UnwindInfoAddress = trampData[i].unwindInfoRva;
        }

        pRuntimeFuncArray[trampData.size()].BeginAddress = 0xC00;
        pRuntimeFuncArray[trampData.size()].EndAddress = 0xFFF;
        pRuntimeFuncArray[trampData.size()].UnwindInfoAddress = relayUnwindRva;

        VirtualProtect((void*) pageBase, 4096, oldProt, &oldProt);

        if (RtlAddFunctionTable(pRuntimeFuncArray, (DWORD) count, pageBase)) {
			Util::log("[Hooks] Registered %i trampolines in UNWIND_INFO table for page 0x%p", (int) trampData.size(), (void*) pageBase);
        } else {
			Util::log("[Hooks] WARNING: RtlAddFunctionTable failed for page 0x%p (error code: %d)", (void*) pageBase, GetLastError());
        }
    }
}

void PatchGC() {
    if (!PatchGCStackWalker()) {
        Util::log("WARNING: GC patch #1 failed! Hooks on NativeAOT functions may crash due to GC FailFast.");
    }

    if (!PatchGCStackWalkerKeepUnwinding()) {
        Util::log("WARNING: GC patch #2 failed! Store-Packets may crash due to unwinder AVs.");
    }
}

void RegisterTrampolines(std::vector<std::pair<void*, void*>> allHooks) {
    RegisterAllTrampolinePages(allHooks);

    int verified = 0, failed = 0;
    for (auto& [tramp, orig] : allHooks) {
        if (!tramp) continue;
        DWORD64 imgBase = 0;
        PRUNTIME_FUNCTION pFunc = RtlLookupFunctionEntry((DWORD64)tramp, &imgBase, NULL);
        if (pFunc) {
            verified++;
        }
        else {
            failed++;
            Util::log("WARNING: RtlLookupFunctionEntry failed for trampoline at 0x%llX", (uint64_t)tramp);
        }
    }
    Util::log("[Hooks] UNWIND_INFO verification: %i OK, %i FAILED", verified, failed);

    Util::log("Finished creating hooks (GC stack-walker patched).");
}
