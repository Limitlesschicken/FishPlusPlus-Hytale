/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include <Windows.h>

#include "Component.h"
class Menu {
public:
	Menu(HDC hdc);

	void Run(double deltaTime);
	void ListenForKeybinds();

	static bool isMenuOpen();
	static void HandleMouse();

	static inline std::unique_ptr<Component> mainComponent;

	inline static bool m_justOpened = false;
	inline static bool m_justClosed = false;
private:

	void OnMenuOpen();
	void OnMenuClose();

	void ListenOpenInput();

	
	inline static bool m_open = true;
};