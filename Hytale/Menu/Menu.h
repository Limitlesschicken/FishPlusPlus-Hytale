#pragma once
#include <Windows.h>

#include "Component.h"

class Menu {
public:
	Menu(HDC hdc);

	void Run(double deltaTime);

	static bool isMenuOpen();

	static inline std::unique_ptr<Component> mainComponent;
private:

	void OnMenuOpen();
	void OnMenuClose();

	void ListenOpenInput();

	inline static bool m_open = true;
};