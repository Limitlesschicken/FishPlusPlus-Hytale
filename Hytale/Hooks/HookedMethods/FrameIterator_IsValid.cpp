/*
 * Copyright (c) FishPlusPlus.
 */
#include "../Hooks.h"

bool __fastcall Hooks::hkFrameIterator_IsValid(GCInstance* instance) {
    if (!instance)
        return Hooks::oFrameIterator_IsValid(instance);

    bool isValid = Hooks::oFrameIterator_IsValid(instance);
    if (instance->address >= dllBase && instance->address <= dllBaseEnd) {
        Util::log("GC: Marking DLL frame 0x%llX with ActiveStackFrame\n", instance->address);
        instance->address = 0; // Clear address to prevent GC from trying to read method info
        instance->originalControlPC = 0;
        instance->codeManager = nullptr;
        instance->pendingFuncletFramePointer = nullptr;
        instance->framePointer = nullptr;
        instance->pConservativeStackRangeLowerBound = 0x0;
        instance->pConservativeStackRangeUpperBound = 0x0;
        instance->flags |= GCFlag::MethodStateCalculated;
        instance->flags |= GCFlag::ActiveStackFrame;
        instance->flags |= GCFlag::SkipNativeFrames;
        return false;
    }

    return isValid;
}