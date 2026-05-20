/*
 * Copyright (c) FishPlusPlus.
 */
#include "Core.h"
#include "Hooks.h"
#include "GCPatch.h"
#include "sdk/Packets/SyncInteractionChains.h"
#include "sdk/Packets/DropItemStack.h"
#include "sdk/Packets/ClientBlockPlace.h"
#include "Events/EventRegister.h"

#include <cmath>
#include <limits>

// Helper macros for creating hooks
// These macros simplify the process of creating hooks by combining pattern scanning and hook creation into a single step. They also log the addresses found for easier debugging.
#define CREATE_HOOK(name) \
if (MH_CreateHook((LPVOID)name##Address, &hk##name, reinterpret_cast<LPVOID*>(&o##name)) != MH_OK) {\
    Util::log("Failed to hook %s", #name);\
    return false;\
} else \
    allHooks.push_back(std::make_pair((void*)o##name, (void*)name##Address));

#define CREATE_SIG_HOOK(name, pattern) \
std::uintptr_t name##Address = Util::PatternScan(pattern);\
Util::log("Found %s sig at: 0x%llX - 0x%llX = 0x%lX", #name, name##Address, gameBase, (name##Address - gameBase));\
CREATE_HOOK(name)

#define CREATE_SIG_HOOK_BY_REF(name, pattern) \
std::uintptr_t name##Address = Util::RelativeVirtualAddress(Util::PatternScan(pattern), 0x1, 0x5);\
Util::log("Found %s sig at: 0x%llX - 0x%llX = 0x%lX", #name, name##Address, gameBase, (name##Address - gameBase));\
CREATE_HOOK(name)


void RebuildPacketsFromBuffer(void* byteArray) {
    if (byteArray == nullptr)
        return;

    const int totalLength = *(const int*) ((uint64_t) byteArray + 0xC);
    int batchOffset = *(int*)((uint64_t) byteArray + 0x8);

    uint64_t buffer = *(uint64_t*) ((uint64_t) byteArray);
    if (buffer == 0)
		return;

    if (totalLength < 0)
        return;
        
    uint64_t targetBuffer = buffer;
    if (batchOffset != 0)
        targetBuffer = buffer + batchOffset;

    int packetID = *(const int*)(targetBuffer + 0x14);

    if (packetID == 117) { // ClientPlaceBlock packet, sent by the client when placing a block. Contains the position, block ID, and other data related to the block placement.
        bool ClientPlaceBlockRebuild = false;
        if (!ClientPlaceBlockRebuild)
            return;

        int dataSize = *(int*)(buffer + 0x10);
        uint64_t payloadPtr = buffer + 0x18;
        if (dataSize < sizeof(ClientPlaceBlockPacketBuffer))
            return;

        ClientPlaceBlockPacketBuffer* packet = (ClientPlaceBlockPacketBuffer*)(payloadPtr);
    }
    else if (packetID == 4) { // Pong packet, sent by the client in response to a server ping. Contains a timestamp that can be used to measure latency.
        return; // This packet is sent very frequently and isn't very interesting, so we can skip logging it to avoid spamming the logs.
    }
    else if (packetID == 108) { // ClientMovement packet, sent by the client to update the player's position and movement state. Contains the player's current position, velocity, and other movement-related data.
        return; // This packet is also sent very frequently and isn't very interesting on its own, so we can skip logging it as well.        
    }
    else if (packetID == 290) { // SyncInteractionChains packet
        int dataSize = *(int*)(buffer + 0x10);

        //Util::log("SyncInteractionChains packet sent by client with data size: %d", dataSize);
        return;
    }
    else if (packetID == 174) {
        uint64_t payloadPtr = buffer + 0x18;
        DropItemStack* packet = (DropItemStack*)(payloadPtr);

    }
    else {
      //Util::log("Packet with ID %d sent by client", packetID);
    }
}

void __fastcall Hooks::hktemp(void* instance, void* object) {
	HytaleString* name = Util::ObjectToString(object);
	std::string nameStr = name ? name->getString() : "nullptr";
    if (Hash::Hash(nameStr) == "Hytale.Protocol.Packets.Interaction.SyncInteractionChains"_hash) {
		SyncInteractionChainsPacket* packet = (SyncInteractionChainsPacket*) object;
		//packet->DBGPrint();
	}
    //Util::log("%s", nameStr.c_str());
	Hooks::otemp(instance, object);
}

/*
* Creates and registers all hooks
*/
bool Hooks::CreateHooks() {
    
    PatchGC();
    
    std::vector<std::pair<void*, void*>> allHooks;

    Util::log("Creating Hooks");

    if (MH_Initialize() != MH_OK) {
        Util::log("Failed to initialize MinHook");
        return false;
    }

    std::uintptr_t WglSwapBuffersAddress = (uint64_t) GetProcAddress(GetModuleHandleA("opengl32.dll"), "wglSwapBuffers");
    ValidPtrBool(WglSwapBuffersAddress);

    CREATE_HOOK(WglSwapBuffers);
    CREATE_SIG_HOOK_BY_REF(WeatherUpdate, "E8 ? ? ? ? 48 8B 4B ? 48 8B 49 ? BA ? ? ? ? ? ? E8 ? ? ? ? 80 BB ? ? ? ? 00 75 ? 48 8B 8B ? ? ? ? F3 0F 10 8B ? ? ? ? ? ? E8 ? ? ? ? 48 8B 8B");
    CREATE_SIG_HOOK_BY_REF(TimeModuleNewFrame, "E8 ? ? ? ? 44 0F B6 83 ? ? ? ? F3 0F 10 8B");
    CREATE_SIG_HOOK_BY_REF(DoMoveCycle, "E8 ? ? ? ? FF CE 75 ? 48 8B 4B");
    CREATE_SIG_HOOK_BY_REF(HandleScreenShotting, "E8 ? ? ? ? 4C 8B 7D ? 49 8B 8F ? ? ? ? 39 09");
    CREATE_SIG_HOOK_BY_REF(OnUserInput, "E8 ? ? ? ? 48 8B 53 ? 48 8B 92 ? ? ? ? 38 12");
    CREATE_SIG_HOOK_BY_REF(OnChat, "E8 ? ? ? ? 48 8B 4D ? 48 8B 89 ? ? ? ? 48 8B 89");
    CREATE_SIG_HOOK_BY_REF(DrawEntityCharactersAndItems, "E8 ? ? ? ? 48 8B 4B ? 48 8B 49 ? BA ? ? ? ? 39 09 E8 ? ? ? ? 48 8B 85");
    CREATE_SIG_HOOK_BY_REF(DrawPostEffect, "E8 ? ? ? ? 80 7B ? ? 75 ? 48 89 5D");
    CREATE_SIG_HOOK_BY_REF(BuildGeometry, "E8 ? ? ? ? 48 89 7D ? ? ? ? 00 75");
    CREATE_SIG_HOOK_BY_REF(ProcessPacket, "E8 ? ? ? ? 90 48 83 C4 ? 5B 5E C3 48 8D 4C 24");
    CREATE_SIG_HOOK_BY_REF(SocketSend, "E8 ? ? ? ? 0F 10 45 ? 0F 11 45 ? EB ? 48 89 85");
	CREATE_SIG_HOOK_BY_REF(MainMenuPostEffectRendererDraw, "E8 ? ? ? ? 4C 8B 6D ? 49 8B 5D");

    if (MH_CreateHook((LPVOID) SM::SendPacketImmediateAddress, &hktemp, reinterpret_cast<LPVOID*>(&otemp)) != MH_OK) {
        Util::log("Failed to hook %s", "temp"); return false;
    } else allHooks.push_back(std::make_pair((void*) otemp, (void*) SM::SendPacketImmediateAddress));

    MH_EnableHook(MH_ALL_HOOKS);

    RegisterTrampolines(allHooks);
    
    return true;
}