/*
 * Copyright (c) FishPlusPlus.
 */
#include "Menu.h"

#include "../Util/Util.h"
#include "../Util/InputSystem.h"

#include "FeatureDispatcher/FeatureDispatcher.h"


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


Menu::Menu(HDC hdc) {
    HWND hwnd = WindowFromDC(hdc);
    g_hWnd = hwnd;

    oWndProc = (WNDPROC)SetWindowLongPtr(
        g_hWnd,
        GWLP_WNDPROC,
        (LONG_PTR)WndProc
    );

    mainComponent = std::make_unique<Component>();
}

void Menu::ListenOpenInput() {
    if (InputSystem::IsKeyPressed(SDL_SCANCODE_INSERT)) {
        m_open = !m_open;
        if (m_open)
            OnMenuOpen();
        else
            OnMenuClose();
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

    if (lbuttonDown) {
        float deltaX = Util::cursorPosX - prevXPos;
        float deltaY = Util::cursorPosY - prevYPos;
        component->MouseDragged(Util::cursorPosX, Util::cursorPosY, VK_LBUTTON, Util::cursorPosX - prevXPos, Util::cursorPosY - prevYPos);
    }

    component->Update(Util::cursorPosX, Util::cursorPosY);
    component->Render(deltaTime);
}

void Menu::Run(double deltaTime) {
    ListenOpenInput();
    ListenForKeybinds();

    if (!Menu::isMenuOpen())
        return;

    bool lbuttonDown = GetAsyncKeyState(VK_LBUTTON);
    bool rbuttonDown = GetAsyncKeyState(VK_RBUTTON);

    CallComponentFuncs(deltaTime, mainComponent.get());
    lbuttonWasDown = lbuttonDown;
    rbuttonWasDown = rbuttonDown;
    prevXPos = Util::cursorPosX;
    prevYPos = Util::cursorPosY;

    mainComponent->Update(Util::cursorPosX, Util::cursorPosY);
    mainComponent->Render(deltaTime);
}

void Menu::ListenForKeybinds() {
    if (!Util::ShouldInteractWithGame())
        return;

    for (auto& feature : FeatureDispatcher::features) {
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