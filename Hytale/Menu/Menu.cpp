#include "Menu.h"

#include "../Util/Util.h"
#include "../Util/InputSystem.h"

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

void Menu::m_listenOpenInput() {
    if (InputSystem::IsKeyPressed(SDL_SCANCODE_RCTRL))
       m_open = !m_open;
}

void Menu::Run() {
    m_listenOpenInput();

    Util::app->Input->isMouseLocked = false;

    if (!Menu::isMenuOpen())
        return;

    Util::app->Input->isMouseLocked = true;

    ClipCursor(NULL);
    SetCursor(LoadCursor(NULL, IDC_ARROW));

    bool lbuttonDown = GetAsyncKeyState(VK_LBUTTON);
    bool rbuttonDown = GetAsyncKeyState(VK_RBUTTON);

    if (lbuttonDown && !lbuttonWasDown)
        mainComponent->MouseClicked(Util::cursorPosX, Util::cursorPosY, VK_LBUTTON);
    if (rbuttonDown && !rbuttonWasDown)
        mainComponent->MouseClicked(Util::cursorPosX, Util::cursorPosY, VK_RBUTTON);
    if (!lbuttonDown && lbuttonWasDown)
        mainComponent->MouseReleased(Util::cursorPosX, Util::cursorPosY, VK_LBUTTON);
    if (!rbuttonDown && rbuttonWasDown)
        mainComponent->MouseReleased(Util::cursorPosX, Util::cursorPosY, VK_RBUTTON);

    if (lbuttonDown) {
        float deltaX = Util::cursorPosX - prevXPos;
        float deltaY = Util::cursorPosY - prevYPos;
        mainComponent->MouseDragged(Util::cursorPosX, Util::cursorPosY, VK_LBUTTON, Util::cursorPosX - prevXPos, Util::cursorPosY - prevYPos);
    }

    lbuttonWasDown = lbuttonDown;
    rbuttonWasDown = rbuttonDown;
    prevXPos = Util::cursorPosX;
    prevYPos = Util::cursorPosY;

    mainComponent->Update(Util::cursorPosX, Util::cursorPosY);
    mainComponent->Render();
}

bool Menu::isMenuOpen() {
    return m_open;
}