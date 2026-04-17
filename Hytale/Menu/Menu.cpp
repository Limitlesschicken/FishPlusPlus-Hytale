/*
 * Copyright (c) FishPlusPlus.
 */
#include "Menu.h"

#include "../Util/Util.h"
#include "../Util/InputSystem.h"

#include "Features/FeatureHandler.h"

static bool lbuttonWasDown = false;
static bool rbuttonWasDown = false;

static float prevXPos = Util::cursorPosX;
static float prevYPos = Util::cursorPosY;

static HWND g_hWnd = nullptr;
static WNDPROC oWndProc = nullptr;
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    if (!Menu::isMenuOpen())
        return oWndProc(hWnd, msg, wParam, lParam);

    if (msg == WM_MOUSEMOVE ||
        msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP ||
        msg == WM_RBUTTONDOWN || msg == WM_RBUTTONUP ||
        msg == WM_MOUSEWHEEL)
    {
        return false;
    }

    return oWndProc(hWnd, msg, wParam, lParam);
}

Menu::Menu() {
    mainComponent = std::make_unique<Component>();
    hudComponent = std::make_unique<Component>();

    currentComponent = mainComponent.get();
}

void Menu::ListenOpenInput() {
    if (InputSystem::IsKeyPressed(SDL_SCANCODE_INSERT)) {
        this->currentComponent = mainComponent.get();
        m_open = !m_open;
        if (m_open)
            OnMenuOpen();
        else
          OnMenuClose();
    }

    if (InputSystem::IsKeyPressed(SDL_SCANCODE_ESCAPE) && this->m_open) {
        if (!this->prevComponent) {
            m_open = false;
            OnMenuClose();
        } else {
            currentComponent = prevComponent;
            prevComponent = nullptr;
        }
    }
}

void Menu::OnMenuOpen() {
    m_justOpened = true;
}

void Menu::OnMenuClose() {
    m_justClosed = true;
}

void CallComponentFuncs(double deltaTime, Component* component) {
    bool lbuttonDown = GetAsyncKeyState(VK_LBUTTON);
    bool rbuttonDown = GetAsyncKeyState(VK_RBUTTON);

    if (lbuttonDown && !lbuttonWasDown)
        component->MouseClicked(Util::cursorPosX, Util::cursorPosY, VK_LBUTTON);
    if (rbuttonDown && !rbuttonWasDown)
        component->MouseClicked(Util::cursorPosX, Util::cursorPosY, VK_RBUTTON);
    if (!lbuttonDown && lbuttonWasDown)
        component->MouseReleased(Util::cursorPosX, Util::cursorPosY, VK_LBUTTON);
    if (!rbuttonDown && rbuttonWasDown)
        component->MouseReleased(Util::cursorPosX, Util::cursorPosY, VK_RBUTTON);
    ///TODO: add pos clamp to stop dragging components out of bounds. Like below
    /*
        void GuiWindow::onDrag(int32_t deltaX, int32_t deltaY) {
            Vec2 windowSize = SDK::getWindow()->getWindowSize();

            float x = pos.x + (float)deltaX;
            float y = pos.y + (float)deltaY;

            float totalHeight = HEADER_HEIGHT + (collapsed ? 0.f : size.y);
            //clamps pos to screen bounds
            x = std::clamp(x, 0.f, (float)windowSize.x - size.x);
            y = std::clamp(y, 0.f, (float)windowSize.y - totalHeight);

            pos.x = x;
            pos.y = y;
        }
    */
    if (lbuttonDown) {
        float deltaX = Util::cursorPosX - prevXPos;
        float deltaY = Util::cursorPosY - prevYPos;
        component->MouseDragged(Util::cursorPosX, Util::cursorPosY, VK_LBUTTON, Util::cursorPosX - prevXPos, Util::cursorPosY - prevYPos);
    }

	if (Menu::m_justOpened)
        component->MenuOpened();
    if (Menu::m_justClosed)
		component->MenuClosed();

    component->Update(Util::cursorPosX, Util::cursorPosY);
    component->Render(deltaTime);
}

void RenderDebugText() {
    bool dbgText = false;
    if (!dbgText)
        return;
    Fonts::Figtree->RenderText(std::format("App: 0x{:x}", reinterpret_cast<uintptr_t>(Util::app)), 0.0f, 10.0f, 0.5f, Color::White());
    Fonts::Figtree->RenderText(std::format("AppInGame: 0x{:x}", reinterpret_cast<uintptr_t>(Util::app->appInGame)), 0.0f, 20.0f, 0.5f, Color::White());
    Fonts::Figtree->RenderText(std::format("GameInstance: 0x{:x}", reinterpret_cast<uintptr_t>(Util::getGameInstance())), 0.0f, 30.0f, 0.5f, Color::White());
    Fonts::Figtree->RenderText(std::format("LocalPlayer: 0x{:x}", reinterpret_cast<uintptr_t>(Util::getLocalPlayer())), 0.0f, 40.0f, 0.5f, Color::White());
    Fonts::Figtree->RenderText(std::format("DMC: 0x{:x}", reinterpret_cast<uintptr_t>(Util::GetMovementController())), 0.0f, 50.0f, 0.5f, Color::White());
    Fonts::Figtree->RenderText(std::format("OptionsHelper: 0x{:x}", reinterpret_cast<uintptr_t>(Globals::optionsHelper)), 0.0f, 60.0f, 0.5f, Color::White());
}

void Menu::Run(double deltaTime) {
    ListenOpenInput();
    ListenForKeybinds();

    RenderDebugText();

    bool lbuttonDown = GetAsyncKeyState(VK_LBUTTON);
    bool rbuttonDown = GetAsyncKeyState(VK_RBUTTON);

    //CallComponentFuncs(deltaTime, hudComponent.get());

    if (Menu::isMenuOpen()) {
        if (this->currentComponent) {
            CallComponentFuncs(deltaTime, this->currentComponent);
        }
        Menu::m_justOpened = false;
    }

    


    lbuttonWasDown = lbuttonDown;
    rbuttonWasDown = rbuttonDown;
    prevXPos = Util::cursorPosX;
    prevYPos = Util::cursorPosY;
    
}

void Menu::ListenForKeybinds() {
    if (!Util::ShouldInteractWithGame())
        return;

    for (auto& feature : FeatureHandler::features) {
        if (InputSystem::IsKeyPressed(feature->GetKeybind()))
            feature->ToggleState();
    }
}

bool Menu::isMenuOpen() {
    return m_open;
}

void Menu::HandleMouse() {
    if (!Util::app->appInGame)
        return;
    
}

void Menu::SetMainComponent(Component* component) {
    prevComponent = currentComponent;
    currentComponent = component;
}