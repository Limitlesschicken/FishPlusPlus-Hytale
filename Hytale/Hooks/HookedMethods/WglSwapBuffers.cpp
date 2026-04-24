/*
 * Copyright (c) FishPlusPlus.
 */
#include "../Hooks.h"
#include "Features/FeatureHandler.h"
#include "Features/ConfigHandler.h"
#include "Renderer/FrameBufferRenderer/FrameBuffers.h"


static void* GetAnyGLFuncAddress(const char* name) {
    void* p = (void*) wglGetProcAddress(name);
    if (p == nullptr || p == (void*) 0x1 || p == (void*) 0x2 ||
        p == (void*) 0x3 || p == (void*) -1) {
        static HMODULE module = GetModuleHandleA("opengl32.dll");
        if (!module)
            module = LoadLibraryA("opengl32.dll");

        p = (void*) GetProcAddress(module, name);
    }
    return p;
}

#pragma optimize("", off)
#pragma runtime_checks("", off)
__declspec(safebuffers) __declspec(noinline)
BOOL WINAPI Hooks::hkWglSwapBuffers(HDC hdc) {
    if (!initialized) {
        if (!Util::IsValidPtr(Util::app))
            return Hooks::oWglSwapBuffers(hdc);

        if (!gladLoadGLLoader((GLADloadproc) GetAnyGLFuncAddress)) {
			Util::log("Failed to initialize GLAD");
            return -1;
        }

        ClientInitialize();
        
        initialized = true;
    }

    //Save the config every 2 minutes
    if ((int)Util::GetTime() % 120 == 0)
        ConfigHandler::SaveConfig("immediateConfig", false);

    static double lastTime = 0.0;
    double currentTime = Util::GetTime();
    double deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    HWND hwnd = WindowFromDC(hdc);
    POINT current;
    GetCursorPos(&current);
    ScreenToClient(hwnd, &current);
    Util::cursorPosX = current.x;
    Util::cursorPosY = current.y;

    if (((oldWindowWidth != Util::app->Engine->Window->WindowWidth) || (oldWindowHeight != Util::app->Engine->Window->WindowHeight))) {
        FrameBuffers::resizeAll(Util::app->Engine->Window->WindowWidth, Util::app->Engine->Window->WindowHeight);
    }
        
    oldWindowWidth = Util::app->Engine->Window->WindowWidth;
    oldWindowHeight = Util::app->Engine->Window->WindowHeight;

    if (!Util::orthoProjMatInitialized) {
        Util::orthoProjMat = Matrix4x4::Orthographic(0.0f, Util::app->Engine->Window->WindowWidth, Util::app->Engine->Window->WindowHeight, 0.0f, -1.0f, 1.0f);
        Util::orthoProjMatInitialized = true;
    }

    if (!uninjecting)
        uninjecting = InputSystem::IsKeyPressed(SDL_SCANCODE_END);

    if (uninjecting) {
        MH_DisableHook(MH_ALL_HOOKS);
        Util::free_console();
        return Hooks::oWglSwapBuffers(hdc);
    }

    menu->Run(deltaTime);

    EventRegister::Render2DEvent.Invoke();

    InputSystem::Reset();


    return Hooks::oWglSwapBuffers(hdc);
}
#pragma runtime_checks("", restore)
#pragma optimize("", on)