/*
 * Copyright (c) FishPlusPlus.
 */
#include "../Hooks.h"
#include "Events/EventRegister.h"

#include "sdk/Hytale/Input.h"

#pragma optimize("", off)
#pragma runtime_checks("", off)

__declspec(safebuffers) __declspec(noinline)
void __fastcall Hooks::hkHandleScreenshotting(App* app) {
    if (Util::app != app)
        Util::app = app;


    Hooks::oHandleScreenshotting(app);
    
    SDK::Main();

    EventRegister::FrameEvent.Invoke();
}
#pragma runtime_checks("", restore)
#pragma optimize("", on)