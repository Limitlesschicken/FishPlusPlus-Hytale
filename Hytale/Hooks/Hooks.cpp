/*
 * Copyright (c) FishPlusPlus.
 */
#include "Core.h"
#include "Hooks.h"
#include "GCPatch.h"

// Helper macros for creating hooks
// These macros simplify the process of creating hooks by combining pattern scanning and hook creation into a single step. They also log the addresses found for easier debugging.
#define CREATE_HOOK(name) \
if (MH_CreateHook((LPVOID)name##Address, &hk##name, reinterpret_cast<LPVOID*>(&o##name)) != MH_OK) {\
    Util::log("Failed to hook %s\n", #name);\
    return false;\
} else \
    allHooks.push_back(std::make_pair((void*)o##name, (void*)name##Address));

#define CREATE_SIG_HOOK(name, pattern) \
std::uintptr_t name##Address = Util::PatternScan(pattern);\
Util::log("Found %s sig at: 0x%llX - 0x%llX = 0x%lX\n", #name, name##Address, gameBase, (name##Address - gameBase));\
CREATE_HOOK(name)

#define CREATE_SIG_HOOK_BY_REF(name, pattern) \
std::uintptr_t name##Address = Util::RelativeVirtualAddress(Util::PatternScan(pattern), 1, 5);\
Util::log("Found %s sig at: 0x%llX - 0x%llX = 0x%lX\n", #name, name##Address, gameBase, (name##Address - gameBase));\
CREATE_HOOK(name)


HytaleString* TempToString(void* ptr) {
    __try {
        return Util::ObjectToString(ptr);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return nullptr;
    }
}




void __fastcall Hooks::hkSetClientBlock(void* instance, int x, int y, int z, uint32_t newID, int arg6, int arg7, bool notify) {
	Util::log("SetClientBlock called with x=%d, y=%d, z=%d, newID=%d\n", x, y, z, newID);
	Util::log("Other args: arg6=%d, arg7=%d, notify=%d\n", arg6, arg7, notify);

    HytaleString* instanceName = TempToString((void*) instance);
    Util::log("%s: %s @ 0x%llX\n", "instance", instanceName ? instanceName->getString().c_str() : "nullptr", instance);
    
	Hooks::oSetClientBlock(instance, x, y, z, newID, arg6, arg7, notify);
}


/*
* Creates and registers all hooks
*/
bool Hooks::CreateHooks() {
    // ═══════════════════════════════════════════════════════════════════════
    // CRITICAL: Patch the NativeAOT GC stack walker BEFORE creating any hooks.
    // Without this patch, MinHook trampolines create stack frames with PCs
    // inside our DLL. The GC can't find these in its method table and calls
    // RaiseFailFastException (0xC0000602).
    // ═══════════════════════════════════════════════════════════════════════
    if (!PatchGCStackWalker()) {
		Util::log("WARNING: GC patch #1 failed! Hooks on NativeAOT functions may crash due to GC FailFast.\n");
    }
    
    // GC-Patch2: Wrap the _1() (vtable[5] UnwindStackFrame) call in fn_KeepUnwinding
    // with SafeUnwind1. When the NativeAOT unwinder crashes on our trampoline (because
    // it uses stale method_info), SafeUnwind1 catches the AV and manually unwinds via
    // RtlLookupFunctionEntry (which finds our RtlAddFunctionTable entries) + RtlVirtualUnwind.
    // The FailFast code cave is a safety net in case SafeUnwind1 returns 0.
    if (!PatchGCStackWalkerKeepUnwinding()) {
		Util::log("WARNING: GC patch #2 failed! Store-Packets may crash due to unwinder AVs.\n");
    }
    
    std::vector<std::pair<void*, void*>> allHooks;

    Util::log("Creating Hooks\n");

    if (MH_Initialize() != MH_OK) {
        Util::log("Failed to initialize MinHook");
        return false;
    }

    std::uintptr_t WglSwapBuffersAddress = (uint64_t) GetProcAddress(GetModuleHandleA("opengl32.dll"), "wglSwapBuffers");
    ValidPtrBool(WglSwapBuffersAddress);

    CREATE_HOOK(WglSwapBuffers);
    CREATE_SIG_HOOK(WeatherUpdate, "41 57 41 56 41 55 41 54 57 56 55 53 48 81 EC ? ? ? ? 0F 29 B4 24 ? ? ? ? 0F 29 BC 24 ? ? ? ? 44 0F 29 84 24 ? ? ? ? 44 0F 29 8C 24 ? ? ? ? 0F 57 E4 0F 29 64 24 ? 0F 29 64 24 ? 48 B8"); //E8 ? ? ? ? 48 8B 4B ? 48 8B 49 ? BA ? ? ? ? 39 09 E8 ? ? ? ? 80 BB ? ? ? ? ? 75 ? 48 8B 8B ? ? ? ? F3 0F 10 8B ? ? ? ? 39 09 E8 ? ? ? ? 48 8B 8B
    CREATE_SIG_HOOK_BY_REF(DoMoveCycle, "E8 ? ? ? ? FF CE 75 ? 48 8B 4B");
    CREATE_SIG_HOOK_BY_REF(HandleScreenShotting, "E8 ? ? ? ? 4C 8B 7D ? 49 8B 8F ? ? ? ? 39 09");
    CREATE_SIG_HOOK_BY_REF(OnUserInput, "E8 ? ? ? ? 48 8B 53 ? 48 8B 92 ? ? ? ? 38 12");
    CREATE_SIG_HOOK_BY_REF(OnChat, "E8 ? ? ? ? 48 8B 4D ? 48 8B 89 ? ? ? ? 48 8B 89");
    CREATE_SIG_HOOK_BY_REF(DrawEntityCharactersAndItems, "E8 ? ? ? ? 48 8B 4B ? 48 8B 49 ? BA ? ? ? ? 39 09 E8 ? ? ? ? 48 8B 85");
    CREATE_SIG_HOOK_BY_REF(DrawPostEffect, "E8 ? ? ? ? 80 7B ? ? 75 ? 48 89 5D");
    CREATE_SIG_HOOK_BY_REF(BuildGeometry, "E8 ? ? ? ? 48 89 7D ? ? ? ? 00 75");

    //CREATE_SIG_HOOK_BY_REF(SetClientBlock, "E8 ? ? ? ? 48 8B CB 8B D6 44 8B C7 45 8B CE 48 83 C4");

    //CREATE_SIG_HOOK(ClassMethod6, "57 56 53 48 83 EC ? 0F 29 74 24 ? 0F 29 7C 24 ? 48 8B D9 48 8B F2 48 8B 4B ? 48 8B 89");

    MH_EnableHook(MH_ALL_HOOKS);

    RegisterAllTrampolinePages(allHooks);

    // ── Verify: check that RtlLookupFunctionEntry finds our registered trampolines ──
    int verified = 0, failed = 0;
    for (auto& [tramp, orig] : allHooks) {
        if (!tramp) continue;
        DWORD64 imgBase = 0;
        PRUNTIME_FUNCTION pFunc = RtlLookupFunctionEntry((DWORD64) tramp, &imgBase, NULL);
        if (pFunc) {
            verified++;
        } else {
            failed++;
			Util::log("WARNING: RtlLookupFunctionEntry failed for trampoline at 0x%llX\n", (uint64_t) tramp);
        }
    }
    Util::log("[Hooks] UNWIND_INFO verification: %i OK, %i FAILED\n", verified, failed);

	Util::log("Finished creating hooks (GC stack-walker patched).\n");
    return true;
}