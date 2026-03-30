/*
 * Copyright (c) FishPlusPlus.
 */
#include "AntiCrash.h"
#include "Util/Util.h"
#include <windows.h>
#include "external/minhook/minhook.h"

namespace {

using fn_RaiseFailFastException = void(WINAPI*)(PEXCEPTION_RECORD, PCONTEXT, DWORD);
using fn_RtlFailFast = void(WINAPI*)(ULONG);
using fn_TerminateProcess = BOOL(WINAPI*)(HANDLE, UINT);
using fn_ExitProcess = void(WINAPI*)(UINT);
using fn_NtTerminateProcess = LONG(NTAPI*)(HANDLE, LONG);
using fn_RtlExitUserProcess = void(WINAPI*)(ULONG);

fn_RaiseFailFastException  oRaiseFailFastException  = nullptr;
fn_RtlFailFast             oRtlFailFast             = nullptr;
fn_TerminateProcess        oTerminateProcess         = nullptr;
fn_ExitProcess             oExitProcess              = nullptr;
fn_NtTerminateProcess      oNtTerminateProcess       = nullptr;
fn_RtlExitUserProcess      oRtlExitUserProcess       = nullptr;

void WINAPI hkRaiseFailFastException(PEXCEPTION_RECORD, PCONTEXT, DWORD dwFlags) {
    Util::log("[AntiCrash] RaiseFailFastException intercepted (flags=0x%08X)\n", dwFlags);
}

void WINAPI hkRtlFailFast(ULONG Code) {
    Util::log("[AntiCrash] RtlFailFast intercepted (code=%lu)\n", Code);
}

BOOL WINAPI hkTerminateProcess(HANDLE hProcess, UINT uExitCode) {
    if (hProcess == GetCurrentProcess() || hProcess == reinterpret_cast<HANDLE>(-1)) {
        Util::log("[AntiCrash] TerminateProcess suppressed (code=0x%08X)\n", uExitCode);
        return TRUE;
    }
    return oTerminateProcess(hProcess, uExitCode);
}

void WINAPI hkExitProcess(UINT uExitCode) {
    Util::log("[AntiCrash] ExitProcess suppressed (code=0x%08X)\n", uExitCode);
}

LONG NTAPI hkNtTerminateProcess(HANDLE ProcessHandle, LONG ExitStatus) {
    if (ProcessHandle == GetCurrentProcess() || ProcessHandle == reinterpret_cast<HANDLE>(-1) || ProcessHandle == nullptr) {
        Util::log("[AntiCrash] NtTerminateProcess suppressed (status=0x%08X)\n", ExitStatus);
        return 0;
    }
    return oNtTerminateProcess(ProcessHandle, ExitStatus);
}

void WINAPI hkRtlExitUserProcess(ULONG ExitStatus) {
    Util::log("[AntiCrash] RtlExitUserProcess suppressed (status=0x%08X)\n", ExitStatus);
}

void HookByName(const char* mod, const char* func, void* detour, void** ppOrig) {
    HMODULE hMod = GetModuleHandleA(mod);
    if (!hMod) return;
    auto* pFunc = reinterpret_cast<void*>(GetProcAddress(hMod, func));
    if (!pFunc) return;
    if (MH_CreateHook(pFunc, detour, ppOrig) == MH_OK)
        Util::log("[AntiCrash] Hooked %s::%s\n", mod, func);
}

}

namespace AntiCrash {

void Install() {
    HookByName("kernelbase.dll", "RaiseFailFastException", reinterpret_cast<void*>(hkRaiseFailFastException), reinterpret_cast<void**>(&oRaiseFailFastException));
    HookByName("ntdll.dll", "RtlFailFast", reinterpret_cast<void*>(hkRtlFailFast), reinterpret_cast<void**>(&oRtlFailFast));
    HookByName("kernel32.dll", "TerminateProcess", reinterpret_cast<void*>(hkTerminateProcess), reinterpret_cast<void**>(&oTerminateProcess));
    HookByName("kernel32.dll", "ExitProcess", reinterpret_cast<void*>(hkExitProcess), reinterpret_cast<void**>(&oExitProcess));
    HookByName("ntdll.dll", "NtTerminateProcess", reinterpret_cast<void*>(hkNtTerminateProcess), reinterpret_cast<void**>(&oNtTerminateProcess));
    HookByName("ntdll.dll", "RtlExitUserProcess", reinterpret_cast<void*>(hkRtlExitUserProcess), reinterpret_cast<void**>(&oRtlExitUserProcess));
}

}
