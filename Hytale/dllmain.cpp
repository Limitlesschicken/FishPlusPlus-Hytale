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
	GetMethodSigByRef(SetCursorHidden, "E8 ? ? ? ? 80 7B ? 00 74 ? 33 C9 EB ? 48 8B 4B");
	GetMethodSigByRef(UpdateInputStates, "E8 ? ? ? ? 83 7E ? ? 75 ? 48 83 C4");
	GetMethodSigByRef(GCToEEInterface_CreateThread, "E8 ? ? ? ? 0F B6 C0 89 05 ? ? ? ? 85 C0");
	GetMethodSigByRef(beginGLContext, "E8 ? ? ? ? 8B 4D ? 8B 95");
	GetMethodSigByRef(endGLContext, "E8 ? ? ? ? 45 0B E6");
    //GetMethodSigByRef(SetClientBlock, "E8 ? ? ? ? 48 8B CB 8B D6 44 8B C7 45 8B CE 48 83 C4");
    GetMethodSigByRef(drawGeometry, "E8 ? ? ? ? FF C7 48 89 5D");

	GetGlobalSigByRef(g_UniformManager, "48 8B 0D ? ? ? ? 48 8B 49 ? ? ? ? 4C 8B B1 ? ? ? ? 41 8B C8"); // 48 8B 0D ? ? ? ? 48 8B 49 ? 45 8B 07
	GetGlobalSigByRef(g_BufferManager, "48 8B 0D ? ? ? ? 48 8B 49 ? 41 8B 40 ? 48 8B B1 ? ? ? ? 44 8B C2 44 89 85 ? ? ? ? 8B D0 89 95 ? ? ? ? B9 ? ? ? ? 89 8D ? ? ? ? 48 8D 4D ? E8 ? ? ? ? 44 8B 85 ? ? ? ? 8B 95 ? ? ? ? 8B 8D ? ? ? ? FF D6 48 8D 4D ? E8 ? ? ? ? 48 8B 5D"); // 48 8B 05 ? ? ? ? 48 8B 40 ? 45 8B 51 ? 4C 8B B0 ? ? ? ? 44 8B CA
	GetGlobalSigByRef(g_GlobalStateTable, "48 83 78 ? 00 0F 85 ? ? ? ? 48 8B 0D ? ? ? ? 48 8B 49 ? ? ? ? 4C 8B B1 ? ? ? ? 41 8B C8"); // 48 8D 05 ? ? ? ? 48 83 78 ? ? 0F 85 ? ? ? ? 48 8B 0D ? ? ? ? 48 8B 49 ? 45 8B 07
    

    GetSig(Object_ToString, "48 83 EC ? E8 ? ? ? ? 48 8B 48 ? 48 85 C9 74 ? 48 8B 09 48 85 C9 75 ? 48 8B C8 E8 ? ? ? ? EB ? 48 8B C1 48 8B C8 48 8B 00 48 83 C4 ? 48 FF 60");
    GetSig(RhpNewFast, "8B 15 ? ? ? ? 65 48 8B 04 25 ? ? ? ? ? ? ? ? BA");
    GetSig(SendPacketImmediate, "55 41 57 41 56 41 55 41 54 57 56 53 48 83 EC ? 48 8D AC 24 ? ? ? ? 0F 57 E4 0F 11 65 ? 33 C0 48 89 45 ? 48 89 4D ? 48 8B D9");
    GetSig(RhpNewArray_Generic, "48 81 FA ? ? ? ? 73 ? 67 8D 04 D5");

    //GetGlobalSigByRef(g_GlobalStateTable, "48 8D 05 ? ? ? ? 48 83 78 ? ? 0F 85 ? ? ? ? 48 8B 0D ? ? ? ? 48 8B 49 ? 45 8B 07");

    /*
    GetGlobalSigByRef(Array_SyncInteractionChain_MT, "48 8D 0D ? ? ? ? E8 ? ? ? ? 4C 8B F0 45 33 ED 48 8B 53");
    GetGlobalSigByRef(SyncInteractionChain_MT, "48 8D 0D ? ? ? ? E8 ? ? ? ? 4C 8B F8 41 C7 47 ? ? ? ? ? 8B 8B");
    GetGlobalSigByRef(InteractionChainData_MT, "48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8B F0 48 BA"); // or 48 8D 0D ? ? ? ? E8 ? ? ? ? 4C 8B F8 48 B9 // 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8B CB 48 BA
    GetGlobalSigByRef(BlockPosition_MT, "48 8D 0D ? ? ? ? E8 ? ? ? ? ? ? 89 48 ? 8B 4B ? 89 48 ? 8B 4B"); // or 48 8D 0D ? ? ? ? E8 ? ? ? ? 8B 4B ? 8B D6
    GetGlobalSigByRef(Array_InteractionSyncData_MT, "48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8B F0 45 33 FF"); 
    GetGlobalSigByRef(InteractionSyncData_MT, "48 8D 0D ? ? ? ? E8 ? ? ? ? C7 40 ? ? ? ? ? 48 B9");
    */

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
        //Util::free_console();
        return 0;
    }

    while (!(FindWindowA(nullptr, "Hytale"))) {
        Sleep(100);
    }

    if (!Hooks::CreateHooks()) {
		Util::log("Failed to create hooks");
        //Util::free_console();
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

