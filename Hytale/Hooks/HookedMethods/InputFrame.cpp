/*
 * Copyright (c) FishPlusPlus.
 */
#include "../Hooks.h"
#include "Events/EventRegister.h"

#include "sdk/Hytale/Input.h"

#pragma optimize("", off)
#pragma runtime_checks("", off)

__declspec(safebuffers) __declspec(noinline)
void __fastcall Hooks::hkInputFrame(Input* input) {
    if (Util::app != input->app)
        Util::app = input->app;

    Hooks::oInputFrame(input);
    //Util::log("%llx", input->app);
    
    SDK::Main();

    EventRegister::FrameEvent.Invoke();
}
#pragma runtime_checks("", restore)
#pragma optimize("", on)