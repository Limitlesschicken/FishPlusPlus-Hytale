/*
 * Copyright (c) FishPlusPlus.
 */
#include "Core.h"
#include "external/minhook/minhook.h"

#include "Hooks/Hooks.h"

void createConsole() {
    AllocConsole();

    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONIN$", "r", stdin);
    freopen_s(&f, "CONOUT$", "w", stderr);

    std::cout << "Console allocated!\n";
}

DWORD WINAPI startPoint(LPVOID lpParam) {
    createConsole();

    if (!Hooks::CreateHooks())
        throw std::runtime_error("Failed to create hooks");

    while (true) {
        Sleep(100);

        if (GetAsyncKeyState(VK_END))
            break;
    }

    MH_DisableHook(MH_ALL_HOOKS);
	FreeConsole();

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
    Core::MODULEPTR = hModule;

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, startPoint, 0, 0, nullptr);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

