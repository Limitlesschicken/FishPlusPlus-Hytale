/*
 * Copyright (c) FishPlusPlus.
 */
#include "../Hooks.h"
#include "Events/EventRegister.h"

int teleportTicks = 0;

void MovementUpdate(DefaultMovementController* dmc, Vector3& offset) {
    if (HookData::queueTeleport) {
        HookData::queueTeleport = false;
        teleportTicks = 5;
    }

    //doing it for 5 ticks makes it work? just 1 tick doesent work
    if (teleportTicks > 0) {
        Util::getLocalPlayer()->SetPositionTeleport(HookData::teleportTarget);
        teleportTicks--;
        return;
    }

    EventRegister::DoMoveCycleEvent.Invoke(dmc, offset);
}

#pragma optimize("", off)
#pragma runtime_checks("", off)
__declspec(safebuffers) __declspec(noinline)
void __fastcall Hooks::hkGroundUpdate(void* a1, DefaultMovementController* dmc, Vector3 offset) {
    if (!initialized)
        return Hooks::oGroundUpdate(a1, dmc, offset);
    
    MovementUpdate(dmc, offset);
    
	Hooks::oGroundUpdate(a1, dmc, offset);
}
#pragma runtime_checks("", restore)
#pragma optimize("", on)

#pragma optimize("", off)
#pragma runtime_checks("", off)
__declspec(safebuffers) __declspec(noinline)
void __fastcall Hooks::hkAirborneUpdate(DefaultMovementController* dmc, Vector3 offset) {
    if (!initialized) 
        return Hooks::oAirborneUpdate(dmc, offset);

    
    MovementUpdate(dmc, offset);

    Hooks::oAirborneUpdate(dmc, offset);
}
#pragma runtime_checks("", restore)
#pragma optimize("", on)