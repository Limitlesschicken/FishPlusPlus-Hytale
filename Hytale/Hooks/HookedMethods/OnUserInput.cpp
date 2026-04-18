/*
 * Copyright (c) FishPlusPlus.
 */
#include "../Hooks.h"

#pragma optimize("", off)
#pragma runtime_checks("", off)

__declspec(safebuffers) __declspec(noinline)
void __fastcall Hooks::hkOnUserInput(uint64_t instance, SDL_Event event) {
    Hooks::oOnUserInput(instance, event);

    if (event.type == SDL_MOUSEWHEEL) {
        InputSystem::inputMutex.lock();
        InputSystem::scrolled = true;
        InputSystem::scrollAmount = event.wheel.y;
        InputSystem::inputMutex.unlock();
    }

    if (event.type == SDL_KEYDOWN) {
        if (event.key.scancode == SDL_SCANCODE_LSHIFT || event.key.scancode == SDL_SCANCODE_RSHIFT)
            InputSystem::shiftHeld = true;
    }

    if (event.type == SDL_KEYUP) {
        if (event.key.scancode == SDL_SCANCODE_LSHIFT || event.key.scancode == SDL_SCANCODE_RSHIFT)
            InputSystem::shiftHeld = false;
    }

    if (event.type != SDL_KEYDOWN && event.type != SDL_KEYUP)
        return;

    SDL_Scancode key{ event.key.scancode };

    if (event.type == SDL_KEYDOWN) {
        if (event.key.repeat)
            return;

        InputSystem::inputMutex.lock();
        InputSystem::keysPressed.insert(key);
        InputSystem::keysHeld.insert(key);
        InputSystem::keysUnheld.erase(key);
        InputSystem::inputMutex.unlock();
    } else if (event.type == SDL_KEYUP) {
        InputSystem::inputMutex.lock();
        InputSystem::keysHeld.erase(key);
        InputSystem::keysUnheld.insert(key);
        InputSystem::keysDepressed.insert(key);
        InputSystem::inputMutex.unlock();
    }
}
#pragma runtime_checks("", restore)
#pragma optimize("", on)