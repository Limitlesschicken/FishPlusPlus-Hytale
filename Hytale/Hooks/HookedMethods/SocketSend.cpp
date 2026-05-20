/*
 * Copyright (c) FishPlusPlus.
 */
#include "../Hooks.h"
#include "Events/EventRegister.h"

#pragma optimize("", off)
#pragma runtime_checks("", off)
__declspec(safebuffers) __declspec(noinline)
void __fastcall Hooks::hkSocketSend(void* instance, void* error, void* byteArray, char socketFlags, void* param5) {
    uint64_t buffer = *(uint64_t*)((uint64_t)byteArray);
    uint64_t payloadPtr = buffer + 0x18;
    Object* packet = (Object*)payloadPtr;

    bool cancel = false;
    int batchOffset = *(int*)((uint64_t)byteArray + 0x8);
    uint64_t targetBuffer = buffer;
    targetBuffer = buffer + batchOffset;

    int packetID = *(const int*)(targetBuffer + 0x14);
    EventRegister::PacketSendEvent.Invoke(packet, packetID, cancel);

    if (!cancel)
        Hooks::oSocketSend(instance, error, byteArray, socketFlags, param5);
}
#pragma runtime_checks("", restore)
#pragma optimize("", on)