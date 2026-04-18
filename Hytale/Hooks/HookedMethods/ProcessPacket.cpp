/*
 * Copyright (c) FishPlusPlus.
 */
#include "../Hooks.h"
#include "Events/EventRegister.h"

#pragma optimize("", off)
#pragma runtime_checks("", off)
__declspec(safebuffers) __declspec(noinline)
void __fastcall Hooks::hkProcessPacket(void* instance, Object* packet) {
	
	bool cancel = false;
	PacketIndex index = GetPacketIndex(packet);
	EventRegister::PacketRecieveEvent.Invoke(packet, index, cancel);

	//if (index == 138)
		//Util::log("Received packet with ID 138 (OpenWindow_S2C) at address 0x%llX", packet);
	//else if (index == 184)
		//Util::log("Received packet with ID 184 (SyncInteractionChains) at address 0x%llX", packet);
	//else if (index != 3 && index != 107)
		//Util::log("Received packet with ID %d at address 0x%llX", index, packet);

	if (!cancel)
		Hooks::oProcessPacket(instance, packet);
}
#pragma runtime_checks("", restore)
#pragma optimize("", on)