#include "Hooks.h"

#include "GLAD/glad.h"

#include <iostream>
#include <memory>
#include <Windows.h>

#include "../sdk/App.h"
#include "../sdk/DefaultMovementController.h"
#include "../FeatureDispatcher/FeatureDispatcher.h"
#include "../Memory/Memory.h"
#include "../external/minhook/minhook.h"
#include "../Util/InputSystem.h"

#include "../Menu/Menu.h"

#include "../Util/Util.h"

#include "../Renderer/Mesh.h"
#include "../Renderer/Shaders.h"
#include "../Renderer/Renderer2D.h"
#include "../Renderer/Renderer3D.h"
#include "../Renderer/FontRenderer/Fonts.h"

#include "../Math/Matrix4x4.h"

typedef int SDL_bool;
#define SDL_TRUE  1
#define SDL_FALSE 0

static bool initialized = false;

static std::unique_ptr<Menu> menu;

static void* GetAnyGLFuncAddress(const char* name)
{
    void* p = (void*)wglGetProcAddress(name);
    if (p == nullptr || p == (void*)0x1 || p == (void*)0x2 ||
        p == (void*)0x3 || p == (void*)-1)
    {
        static HMODULE module = LoadLibraryA("opengl32.dll");
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

        Renderer2D::colored = std::make_unique<Renderer2D>();

        menu = std::make_unique<Menu>(hdc);

        FeatureDispatcher::initFeatures();

        initialized = true;
    }

    HWND hwnd = WindowFromDC(hdc);
    RECT r;
    POINT current;
    GetCursorPos(&current);
    ScreenToClient(hwnd, &current);
    GetClientRect(hwnd, &r);
    Util::windowWidth = r.right - r.left;
    Util::windowHeight = r.bottom - r.top;
    Util::cursorPosX = current.x;
    Util::cursorPosY = current.y;
    
    Renderer3D renderer3d = Renderer3D();
    renderer3d.BoxLines(Vector3(0, 80, 0), Vector3(1, 1, 1), Color::From255(50, 50, 255, 50), Color::From255(50, 50, 255, 255));
    renderer3d.BoxLines(Vector3(5, 80, 0), Vector3(1, 2, 1), Color::From255(50, 50, 255, 50), Color::From255(50, 50, 255, 255));
    renderer3d.Render();

    Render3DEvent render3DEvent(renderer3d);
    FeatureDispatcher::DispatchEvent(render3DEvent);

    Fonts::Figtree->RenderText(std::format("App: 0x{:x}", reinterpret_cast<uintptr_t>(Util::app)), 0.0f, 10.0f, 0.5f, Color::White());
    Fonts::Figtree->RenderText(std::format("AppInGame: 0x{:x}", reinterpret_cast<uintptr_t>(Util::app->appInGame)), 0.0f, 20.0f, 0.5f, Color::White());
    Fonts::Figtree->RenderText(std::format("GameInstance: 0x{:x}", reinterpret_cast<uintptr_t>(Util::getGameInstance())), 0.0f, 30.0f, 0.5f, Color::White());
    Fonts::Figtree->RenderText(std::format("LocalPlayer: 0x{:x}", reinterpret_cast<uintptr_t>(Util::getLocalPlayer())), 0.0f, 40.0f, 0.5f, Color::White());

    Fonts::Figtree->RenderText(std::format("Fish++ Hytale by LimitlessChicken aka milaq", reinterpret_cast<uintptr_t>(Util::app)), 500.0f, 10.0f, 0.5f, Color::White());

    menu->Run();

    InputSystem::keysPressed.clear();
    InputSystem::keysDepressed.clear();

    return oWglSwapBuffers(hdc);
}

typedef __int64(__fastcall* PlayerMove)(__int64 instance, Vector3 offset);
PlayerMove oPlayerMove = nullptr;
__int64 __fastcall HGameTickType(__int64 instance, Vector3 offset) {
    DefaultMovementController* dmc = (DefaultMovementController*)instance;
    Util::dmc = dmc;
    MoveCycleEvent event(*dmc, offset);
    FeatureDispatcher::DispatchEvent(event);

    return oPlayerMove(instance, offset);
}

typedef __int64(__fastcall* setUniformBuffers)(__int64 instance);
setUniformBuffers oSetUniformBuffers = nullptr;
__int64 __fastcall HSetUniformBuffers(__int64 instance) {
    __int64 matrix = *(__int64*)(instance + 0x300);
    
    Matrix4x4 viewProjMat = *(Matrix4x4*)(instance + 0x300);

    Util::viewProjMat = viewProjMat;

    return oSetUniformBuffers(instance);
}

typedef __int64(__fastcall* InGameApp)(__int64 a1);
InGameApp oInGameApp = nullptr;
__int64 __fastcall HInGameApp(__int64 a1) {
    Util::app = (App*)a1;
    return oInGameApp(a1);
}

typedef __int64*(__fastcall* OnUserInput)(__int64 a1, int* a2);
OnUserInput oOnUserInput = nullptr;
__int64* __fastcall HOnUserInput(__int64 a1, int* a2) {
    if (Menu::isMenuOpen()) {
        Input* input = (Input*)a1;
        input->isMouseLocked = true;
    }
        
    SDL_Scancode key = (SDL_Scancode)a2[6];

    if (*a2 == 768) {
        if (*((bool*)a2 + 37))
            return oOnUserInput(a1, a2);

        InputSystem::keysPressed.insert(key);
        InputSystem::keysHeld.insert(key);
        InputSystem::keysUnheld.erase(key);
    }

    if (*a2 == 769) {
        InputSystem::keysHeld.erase(key);
        InputSystem::keysUnheld.insert(key);
        InputSystem::keysDepressed.insert(key);
    }

    return oOnUserInput(a1, a2);
}

bool Hooks::CreateHooks() {
    if (MH_Initialize() != MH_OK) {
        std::cout << "Failed to initialize minhook";
        return false;
    }
    
    HMODULE hOpenGL = GetModuleHandleA("opengl32.dll");
    if (!hOpenGL || hOpenGL == 0)
        hOpenGL = LoadLibraryA("opengl32.dll");
    void* pWglSwapBuffers = GetProcAddress(hOpenGL, "wglSwapBuffers");
    if (MH_CreateHook(pWglSwapBuffers, &HWglSwapBuffers, reinterpret_cast<LPVOID*>(&oWglSwapBuffers)) != MH_OK) {
        std::cout << "Failed to create hook HWglSwapBuffers";
        return false;
    }
    
    //const char* pattern = "41 57 41 56 41 55 41 54 57 56 55 53 48 81 EC ? ? ? ? 0F 29 B4 24 ? ? ? ? 0F 29 BC 24 ? ? ? ? 44 0F 29 84 24 ? ? ? ? 0F 57 E4 0F 29 64 24";
    const char* playerMovePattern = "55 41 57 41 56 41 55 41 54 57 56 53 48 81 EC ? ? ? ? 0F 29 B4 24 ? ? ? ? 0F 29 BC 24 ? ? ? ? 44 0F 29 84 24 ? ? ? ? 48 8D AC 24 ? ? ? ? 33 C0 48 89 85 ? ? ? ? 0F 57 E4 48 B8";
    std::uint8_t* playerMoveAddress = PatternScan(playerMovePattern);
    if (MH_CreateHook(playerMoveAddress, &HGameTickType, reinterpret_cast<LPVOID*>(&oPlayerMove)) != MH_OK) {
        std::cout << "Failed to create hook HPlayerMove";
        return false;
    }

    const char* setUniformBuffersPattern = "55 41 57 41 56 41 55 41 54 57 56 53 48 81 EC ? ? ? ? 0F 29 B4 24 ? ? ? ? 0F 29 BC 24 ? ? ? ? 48 8D AC 24 ? ? ? ? 48 8B D9 48 89 5D";
    uint8_t* setUniformBuffersAddress = PatternScan(setUniformBuffersPattern);
    if (MH_CreateHook(setUniformBuffersAddress, &HSetUniformBuffers, reinterpret_cast<LPVOID*>(&oSetUniformBuffers)) != MH_OK) {
        std::cout << "Failed to create hook HSetUniformBuffers";
        return false;
    }

    const char* ingameApp = "55 41 57 41 56 41 55 41 54 57 56 53 48 81 EC ? ? ? ? 48 8D AC 24 ? ? ? ? 33 C0 48 89 45 ? 0F 57 E4 0F 29 65 ? 48 89 45 ? 48 8B D9 48 8B 4B ? 48 8B 49";
    uint8_t* ingameAppAddress = PatternScan(ingameApp);
    if (MH_CreateHook(ingameAppAddress, &HInGameApp, reinterpret_cast<LPVOID*>(&oInGameApp)) != MH_OK) {
        std::cout << "Failed to create hook HInGameApp";
        return false;
    }
    
    const char* onUserInputPattern = "41 57 41 56 41 55 41 54 57 56 55 53 48 83 EC ? 33 C0 48 89 44 24 ? 0F 57 E4 0F 29 64 24 ? 0F 29 64 24 ? 0F 29 64 24 ? 48 89 44 24 ? 48 8B D9 48 8B F2 8B 3E";
    uint8_t* onUserInputAddress = PatternScan(onUserInputPattern);
    if (MH_CreateHook(onUserInputAddress, &HOnUserInput, reinterpret_cast<LPVOID*>(&oOnUserInput)) != MH_OK) {
        std::cout << "Failed to create hook HOnUserInput";
        return false;
    }
    
    MH_EnableHook(MH_ALL_HOOKS);
    return true;
}
