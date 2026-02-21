/*
 * Copyright (c) FishPlusPlus.
 */
#include "Core.h"
#include "Hooks.h"

typedef int SDL_bool;
#define SDL_TRUE  1
#define SDL_FALSE 0

static bool initialized = false;

static std::unique_ptr<Menu> menu;

#define CREATE_HOOK(name, pattern) \
std::uint8_t* name##Address = PatternScan(pattern);\
if (MH_CreateHook(name##Address, &H##name, reinterpret_cast<LPVOID*>(&o##name)) != MH_OK) {\
    std::cout << "failed to create hook: " << #name << "\n";\
    return false;\
}\
                                    

static void* GetAnyGLFuncAddress(const char* name)
{
    void* p = (void*)wglGetProcAddress(name);
    if (p == nullptr || p == (void*)0x1 || p == (void*)0x2 ||
        p == (void*)0x3 || p == (void*)-1)
    {
		static HMODULE module = GetModuleHandleA("opengl32.dll");
        if (!module)
            module = LoadLibraryA("opengl32.dll");

        p = (void*)GetProcAddress(module, name);
    }
    return p;
}


typedef BOOL(WINAPI* wglSwapBuffers_t)(HDC hdc);
wglSwapBuffers_t oWglSwapBuffers = nullptr;
BOOL WINAPI HWglSwapBuffers(HDC hdc) {
    if (!initialized) {

        if (!gladLoadGLLoader((GLADloadproc)GetAnyGLFuncAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }
        Shaders::initShaders();
        Fonts::initFonts();

        Renderer2D::InitRenderer();

        menu = std::make_unique<Menu>(hdc);

        FeatureDispatcher::initFeatures();

        initialized = true;
    }

    static double lastTime = 0.0;
    double currentTime = Util::GetTime();
    double deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    HWND hwnd = WindowFromDC(hdc);
    RECT r;
    POINT current;
    GetCursorPos(&current);
    ScreenToClient(hwnd, &current);
    Util::cursorPosX = current.x;
    Util::cursorPosY = current.y;

    SDK::Main();

    if (Util::IsValidPtr(Util::app)) {
        Renderer3D renderer3D;
        Render3DEvent render3DEvent(renderer3D);
        FeatureDispatcher::DispatchEvent(render3DEvent);
        renderer3D.Render();
    }

    if (Util::IsValidPtr(Util::app)) {
        Fonts::Figtree->RenderText(std::format("App: 0x{:x}", reinterpret_cast<uintptr_t>(Util::app)), 0.0f, 10.0f, 0.5f, Color::White());
        Fonts::Figtree->RenderText(std::format("AppInGame: 0x{:x}", reinterpret_cast<uintptr_t>(Util::app->appInGame)), 0.0f, 20.0f, 0.5f, Color::White());
        Fonts::Figtree->RenderText(std::format("GameInstance: 0x{:x}", reinterpret_cast<uintptr_t>(Util::getGameInstance())), 0.0f, 30.0f, 0.5f, Color::White());
        Fonts::Figtree->RenderText(std::format("LocalPlayer: 0x{:x}", reinterpret_cast<uintptr_t>(Util::getLocalPlayer())), 0.0f, 40.0f, 0.5f, Color::White());
        Fonts::Figtree->RenderText(std::format("DMC: 0x{:x}", reinterpret_cast<uintptr_t>(Util::GetMovementController())), 0.0f, 50.0f, 0.5f, Color::White());
    }

    Fonts::Figtree->RenderText(std::format("Fish++ Hytale by LimitlessChicken aka milaq", reinterpret_cast<uintptr_t>(Util::app)), 500.0f, 10.0f, 0.5f, Color::White());

    menu->Run(deltaTime);

    InputSystem::keysPressed.clear();
    InputSystem::keysDepressed.clear();

    return oWglSwapBuffers(hdc);
}

__int64 __fastcall HDoMoveCycle(__int64 thisptr, Vector3* offset) {
    DefaultMovementController* dmc = (DefaultMovementController*)thisptr;
    if (Util::IsValidPtr(dmc)) {
        MoveCycleEvent event(*dmc, *offset);
        FeatureDispatcher::DispatchEvent(event);
    }

    return Hooks::oDoMoveCycle(thisptr, offset);
}

__int64 __fastcall HHandleScreenShotting(__int64 thisptr) {
    
    Util::app = (App*)thisptr;
    
    
    if (Menu::isMenuOpen() && Menu::m_justOpened) {
        Hooks::oSetCursorHidden((__int64)Util::app->Engine->Window, false);
        Menu::m_justOpened = false;
    }

    if (!Menu::isMenuOpen() && Menu::m_justClosed) {
        Util::app->appInGame->UpdateInputStates(true);
            
        Menu::m_justClosed = false;
    }

    return Hooks::oHandleScreenShotting(thisptr);
}


__int64* __fastcall HOnUserInput(__int64 thisptr, int* a2) {
    SDL_Scancode key = (SDL_Scancode)(a2[6]);
     
    if (*a2 == 768) {
        if (*((bool*)a2 + 37))
            return Hooks::oOnUserInput(thisptr, a2);

        InputSystem::keysPressed.insert(key);
        InputSystem::keysHeld.insert(key);
        InputSystem::keysUnheld.erase(key);
    }

    if (*(int*)a2 == 769) {
        InputSystem::keysHeld.erase(key);
        InputSystem::keysUnheld.insert(key);
        InputSystem::keysDepressed.insert(key);
    }

    return Hooks::oOnUserInput(thisptr, a2);
}

void __fastcall HSetCursorHidden(__int64 thisptr, char hidden) {
    if (Menu::isMenuOpen()) {
        return;
    }
        
    Hooks::oSetCursorHidden(thisptr, hidden);
}

void __fastcall HWeatherUpdate(__int64 thisptr, float deltaTime) {

    Hooks::oWeatherUpdate(thisptr, deltaTime);

    float* vec3 = (float*)((uintptr_t)thisptr + 0x364);

    //vec3[0] = 2.0f;

    //vec32[0] = 2.0f;

}

void __fastcall HSetActiveHotbarSlot(__int64 thisptr, unsigned int slot, bool triggerInteraction) {
    Hooks::oSetActiveHotbarSlot(thisptr, slot, triggerInteraction);
}

bool Hooks::CreateHooks() {
    if (MH_Initialize() != MH_OK) {
        std::cout << "Failed to initialize minhook";
        return false;
    }

    void* pWglSwapBuffers = GetProcAddress(GetModuleHandleA("opengl32.dll"), "wglSwapBuffers");
    if (MH_CreateHook(pWglSwapBuffers, &HWglSwapBuffers, reinterpret_cast<LPVOID*>(&oWglSwapBuffers)) != MH_OK) {
        std::cout << "Failed to create hook HWglSwapBuffers";
        return false;
    }

    CREATE_HOOK(DoMoveCycle, "55 41 57 41 56 41 55 41 54 57 56 53 48 81 EC ? ? ? ? 0F 29 B4 24 ? ? ? ? 0F 29 BC 24 ? ? ? ? 44 0F 29 84 24 ? ? ? ? 48 8D AC 24 ? ? ? ? 33 C0 48 89 85 ? ? ? ? 0F 57 E4 48 B8");
    CREATE_HOOK(HandleScreenShotting, "55 41 57 41 56 41 55 41 54 57 56 53 48 81 EC ? ? ? ? 48 8D AC 24 ? ? ? ? 33 C0 48 89 45 ? 0F 57 E4 0F 29 65 ? 48 89 45 ? 48 8B D9 48 8B 4B ? 48 8B 49");
    CREATE_HOOK(OnUserInput, "41 57 41 56 41 55 41 54 57 56 55 53 48 83 EC ? 33 C0 48 89 44 24 ? 0F 57 E4 0F 29 64 24 ? 0F 29 64 24 ? 0F 29 64 24 ? 48 89 44 24 ? 48 8B D9 48 8B F2 8B 3E");
    CREATE_HOOK(SetCursorHidden, "55 57 56 53 48 83 EC ? 48 8D 6C 24 ? 33 C0 48 89 45 ? 48 89 45 ? 48 8B D9 8B F2");
    //CREATE_HOOK(UpdateInputStates, "57 56 53 48 83 EC ? 48 8B D9 8B F2 48 8B 4B ? 48 85 C9 0F 84");
    //CREATE_HOOK(SetActiveHotbarSlot, "55 41 56 57 56 53 48 83 EC ? 48 8D 6C 24 ? 48 8B D9 8B F2 48 83 7B");
    //CREATE_HOOK(WeatherUpdate, "57 56 55 53 48 83 EC ? 0F 29 74 24 ? 48 8B D9 48 8B F2 48 8B 4B ? 48 8B 89 ? ? ? ? 48 8B 79 ? 80 BB ? ? ? ? ? 74 ? 80 7B ? ? 0F 85 ? ? ? ? 48 8B CF 4C 8D 1D ? ? ? ? 41 FF 13 85 C0 0F 85 ? ? ? ? 0F B6 83 ? ? ? ? 88 83 ? ? ? ? F3 0F 10 76 ? 0F 16 F6 0F 12 36 0F 57 C0 0F 28 CE 0F C6 C8 ? 0F 28 C6 0F C6 C1 ? 0F 59 C0 0F 28 C8 0F C6 C8 ? 0F 58 C8 0F 28 C1 0F C6 C1 ? 0F 58 C1 F3 0F 51 C0 F3 0F 59 05 ? ? ? ? F3 0F 5A C0 E8 ? ? ? ? 0F 28 C8 F2 0F C2 C8 07 66 0F 54 C8 BA ? ? ? ? F2 0F 2C C9 66 0F 2E 05 ? ? ? ? 0F 42 D1 8B F2 0F 57 C0 F3 0F 2A C6 0F C6 C0 ? 0F 5E F0 85 F6 7E ? 8B EE 0F 29 74 24 ? 48 8D 54 24 ? 48 8B CB E8 ? ? ? ? FF CD 75 ? 85 F6 0F 85");
    


    MH_EnableHook(MH_ALL_HOOKS);
    return true;
}
