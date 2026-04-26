/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include <Windows.h>

#include "Component.h"
class Menu {
public:
	Menu();

	void Run(double deltaTime);
	void ListenForKeybinds();

	static bool isMenuOpen();
	static void HandleMouse();

	static void SetMainComponent(Component* component);

	static inline std::unique_ptr<Component> mainComponent;
	static inline std::unique_ptr<Component> hudComponent;
	static inline std::unique_ptr<Component> hudTabComponent;

	static inline Component* currentComponent ;
	static inline Component* prevComponent;

	inline static bool m_justOpened = true;
	inline static bool m_justClosed = false;
private:

	void OnMenuOpen();
	void OnMenuClose();

	void ListenOpenInput();
	inline static bool m_open = false;
};