/*
 * Copyright (c) FishPlusPlus.
 */
#include "../Hooks.h"
#include "Events/EventRegister.h"

void __fastcall Hooks::hkSocketSend(void* instance, void* error, void* byteArray, char socketFlags, void* param5) {
    uint64_t buffer = *(uint64_t*)((uint64_t)byteArray);
    uint64_t payloadPtr = buffer + 0x18;
    Object* packet = (Object*)payloadPtr;

    bool cancel = false;
    PacketIndex index = GetPacketIndex(packet);
    EventRegister::PacketRecieveEvent.Invoke(packet, index, cancel);

    if (!cancel)
        return Hooks::oSocketSend(instance, error, byteArray, socketFlags, param5);
}