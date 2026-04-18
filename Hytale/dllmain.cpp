/*
 * Copyright (c) FishPlusPlus.
 */
#include "Core.h"
#include "Hooks/Hooks.h"
#include "sdk/BaseDataTypes/ConcurrentDictionary.h"

#define GetSig(name, pattern) SM::name##Address = Util::PatternScan(pattern); \
Util::log("Found %s sig at: 0x%llX - 0x%llX = 0x%lX", #name, SM::name##Address, gameBase, (SM::name##Address - gameBase));\
if (!Util::IsValidPtr(SM::name##Address)) {                             \
    Util::log("Failed to get %s address", #name);                     \
    return false;                                                       \
}

#define GetMethodSigByRef(name, pattern) SM::name##Address = Util::RelativeVirtualAddress(Util::PatternScan(pattern), 0x1, 0x5); \
Util::log("Found Method %s sig at: 0x%llX - 0x%llX = 0x%lX", #name, SM::name##Address, gameBase, (SM::name##Address - gameBase));\
if (!Util::IsValidPtr(SM::name##Address)) {                             \
    Util::log("Failed to get %s address", #name);                     \
    return false;                                                       \
}

#define GetGlobalSigByRef(name, pattern) SM::name##Address = Util::RelativeVirtualAddress(Util::PatternScan(pattern), 0x3); \
Util::log("Found Global %s sig at: 0x%llX - 0x%llX = 0x%lX", #name, SM::name##Address, gameBase, (SM::name##Address - gameBase));\
if (!Util::IsValidPtr(SM::name##Address)) {                             \
    Util::log("Failed to get %s address", #name);                     \
    return false;                                                       \
}

#define DEFINE_PATTERN(name, pattern) API::name = (API::name##_t)Util::PatternScan(pattern); Util::log("Found " #name " at: 0x%llX", API::name);
/*// Available GC Registered Thread
void __fastcall GCThread(void* pArg) {
    while (!uninjecting) {
        Sleep(100);
    }
}*/


bool InitSigs() {
	GetMethodSigByRef(SetCursorHidden, "E8 ? ? ? ? 0F B6 4B ? 85 C9 74");
	GetMethodSigByRef(UpdateInputStates, "E8 ? ? ? ? 83 7E ? ? 75 ? 48 83 C4");
	GetMethodSigByRef(GCToEEInterface_CreateThread, "E8 ? ? ? ? 0F B6 C0 89 05 ? ? ? ? 85 C0");
	GetMethodSigByRef(beginGLContext, "E8 ? ? ? ? 8B 4D ? 44 8B 45 ? 8B 55 ? 41 FF D7");
	GetMethodSigByRef(endGLContext, "E8 ? ? ? ? 48 8B 75 ? 4C 8D 4E ? 48 8B 5D ? 48 8B 53 ? 44 3B 72");
	GetMethodSigByRef(renderQueueFlush, "E8 ? ? ? ? CC 57 56 55 53 48 83 EC ? 48 8B D9 48 8B F2 41 8B F8");
	GetMethodSigByRef(submitDrawCommands, "E8 ? ? ? ? E9 ? ? ? ? 90 90 90 90 90 90 48 83 79");
    GetMethodSigByRef(SetClientBlock, "E8 ? ? ? ? 48 8B CB 8B D6 44 8B C7 45 8B CE 48 83 C4");

	GetGlobalSigByRef(g_UniformManager, "48 8B 0D ? ? ? ? 48 8B 49 ? 45 8B 07");
	GetGlobalSigByRef(g_BufferManager, "48 8B 05 ? ? ? ? 48 8B 40 ? 45 8B 51 ? 4C 8B B0 ? ? ? ? 44 8B CA");
	GetGlobalSigByRef(g_GlobalStateTable, "48 8D 05 ? ? ? ? 48 83 78 ? ? 0F 85 ? ? ? ? 48 8B 0D ? ? ? ? 48 8B 49 ? 45 8B 07");

    GetSig(Object_ToString, "48 83 EC ? E8 ? ? ? ? 48 8B 48 ? 48 85 C9 74 ? 48 8B 09 48 85 C9 75 ? 48 8B C8 E8 ? ? ? ? EB ? 48 8B C1 48 8B C8 48 8B 00 48 83 C4 ? 48 FF 60");
    GetSig(RhpNewFast, "8B 15 ? ? ? ? 65 48 8B 04 25 ? ? ? ? ? ? ? ? BA");
    GetSig(SendPacketImmediate, "55 41 57 41 56 41 55 41 54 57 56 53 48 81 EC ? ? ? ? 48 8D AC 24 ? ? ? ? 0F 57 E4 0F 29 65 ? 0F 29 65 ? 0F 29 65 ? 0F 29 65 ? 33 C0 48 89 45 ? 48 89 4D ? 48 8B D9"); //48 8b d9 48 8b f2 48 8d 0d ? ? ? ? e8 ? ? ? ? 48 89 85 ? ? ? ? 48 8d 48 08 48 8b d3 e8 ? ? ? ? 48 85 f6
    GetSig(RhpNewArray_Generic, "48 81 FA ? ? ? ? 73 ? 67 8D 04 D5");

    GetGlobalSigByRef(g_GlobalStateTable, "48 8D 05 ? ? ? ? 48 83 78 ? ? 0F 85 ? ? ? ? 48 8B 0D ? ? ? ? 48 8B 49 ? 45 8B 07");

    GetGlobalSigByRef(Array_SyncInteractionChain_MT, "48 8D 0D ? ? ? ? E8 ? ? ? ? 4C 8B F0 45 33 ED 48 8B 53");
    GetGlobalSigByRef(SyncInteractionChain_MT, "48 8D 0D ? ? ? ? E8 ? ? ? ? 4C 8B F8 41 C7 47 ? ? ? ? ? 8B 8B");
    GetGlobalSigByRef(InteractionChainData_MT, "48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8B F0 48 BA"); // or 48 8D 0D ? ? ? ? E8 ? ? ? ? 4C 8B F8 48 B9 // 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8B CB 48 BA
    GetGlobalSigByRef(BlockPosition_MT, "48 8D 0D ? ? ? ? E8 ? ? ? ? 8B 0B 89 48 ? 8B 4B"); // or 48 8D 0D ? ? ? ? E8 ? ? ? ? 8B 4B ? 8B D6
    GetGlobalSigByRef(Array_InteractionSyncData_MT, "48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8B F0 45 33 FF"); 
    GetGlobalSigByRef(InteractionSyncData_MT, "48 8D 0D ? ? ? ? E8 ? ? ? ? C7 40 ? ? ? ? ? 48 B9");
    GetGlobalSigByRef(Array_MapChunk_MT, "48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8B F8 33 DB E9 ? ? ? ? 8B 4C 24 ? 41 3B CE 0F 87 ? ? ? ? 8B C1 48 03 C5 41 8B D6 2B D1 0F 57 C0 0F 11 44 24 ? 48 89 44 24 ? 89 54 24 ? 48 8D 4C 24 ? E8 ? ? ? ? 3B 5F ? 0F 83 ? ? ? ? 48 8D 4C DF ? 48 8B D0 E8 ? ? ? ? 8B 74 24 ? 8B 4C 24 ? 41 3B CE 0F 87 ? ? ? ? 8B C1 48 03 C5 41 8B D6 2B D1 41 BF ? ? ? ? 85 D2 0F 84 ? ? ? ? ? ? ? 74 ? 83 FA ? 0F 82 ? ? ? ? 48 83 C0 ? 83 C2 ? 48 89 44 24 ? 89 54 24 ? 48 8D 4C 24 ? E8 ? ? ? ? 44 8D 78 ? 41 03 F7 89 74 24 ? FF C3 3B 5C 24 ? 0F 8C ? ? ? ? 48 8B C7 48 83 C4 ? 5B 5D 5E 5F 41 5E 41 5F C3 48 8B CF E8 ? ? ? ? 48 8B C8 E8 ? ? ? ? ? 48 8B CF 8B 54 24 ? E8 ? ? ? ? 48 8B C8 E8 ? ? ? ? ? 48 8B CF 8B 54 24 ? 44 8B C6 E8 ? ? ? ? 48 8B C8 E8 ? ? ? ? ? 48 8B CF 8B 54 24 ? 44 8B C3 E8 ? ? ? ? 48 8B C8 E8 ? ? ? ? ? 8B 54 24 ? ? ? ? 03 54 24 ? 48 8B CF 45 8B C6 E8 ? ? ? ? 48 8B C8 E8 ? ? ? ? ? E8 ? ? ? ? ? E8 ? ? ? ? ? 90");
    GetGlobalSigByRef(MapChunk_MT, "48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8B F0 8B 4B ? 85 C9 0F 84 ? ? ? ? ? ? ? 48 FF C0 FF C9 83 F9 ? 7C ? ? ? 89 4E ? 8B 4B ? 83 F9 ? 72 ? ? ? ? 48 83 C0 ? 83 C1 ? 83 F9 ? 7C ? ? ? 89 4E ? 8B 4B ? 85 C9 74 ? ? ? ? ? ? ? 74 ? 83 F9 ? 72 ? 48 83 C0 ? 83 C1 ? 0F 57 C0 0F 11 44 24 ? 48 89 44 24 ? 89 4C 24 ? 48 8D 4C 24 ? E8 ? ? ? ? EB ? 33 C0 48 8D 4E ? 48 8B D0 E8 ? ? ? ? 48 8B C6 48 83 C4 ? 5B 5E C3 B9 ? ? ? ? E8 ? ? ? ? ? E8 ? ? ? ? ? E8 ? ? ? ? ? 90 53");

	Util::log("Finished initializing signatures");
	return true;
}

DWORD WINAPI startPoint(LPVOID lpParam) {
	Util::allocate_console();

    Globals::optionsHelper = *(OptionsHelper**)Util::RelativeVirtualAddress(Util::PatternScan("48 8B 35 ? ? ? ? 48 8B 56 ? 48 85 D2 75 ? 48 8B 4B"), 0x3);
    Globals::paths = *(Paths**)Util::RelativeVirtualAddress(Util::PatternScan("48 8B 0D ? ? ? ? 48 8B 49 ? E8 ? ? ? ? 48 8D 35"), 0x3);
    Globals::buildInfo = *(BuildInfo**)Util::RelativeVirtualAddress(Util::PatternScan("48 8B 1D ? ? ? ? 48 8B 73 ? 48 8B 4B"), 0x3);
    Util::log("Found game directory at: %s", Globals::paths->ClientGameDirectory->getString().c_str());
    Util::log("The games build-date and githash are: %s", Globals::buildInfo->getVersion->getString().c_str());

    if (!InitSigs()){
        Util::log("Failed to Init Sigs");
        Util::free_console();
        return 0;
    }

    while (!(FindWindowA(nullptr, "Hytale"))) {
        Sleep(100);
    }

    if (!Hooks::CreateHooks()) {
		Util::log("Failed to create hooks");
        Util::free_console();
        return 0;
    }
/*    // Available GC Registered Thread
    typedef void(__fastcall* ThreadStart)(void* pArg);
    bool success = ((bool(__fastcall*)(ThreadStart, void*, bool, const char*))SM::GCToEEInterface_CreateThreadAddress)(GCThread, nullptr, true, "HytaleInternal");*/
    
    
    return 1;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
		dllBase = (uint64_t) hModule;
		gameBase = (uint64_t) GetModuleHandleA(nullptr);

        MODULEINFO moduleInfo;
        if (GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(MODULEINFO))) {
            SIZE_T moduleSize = moduleInfo.SizeOfImage;
            dllBaseEnd = dllBase + moduleSize;
        }
        //return startPoint(0x0);
        CreateThread(nullptr, 0, startPoint, 0, 0, nullptr);
        //break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

