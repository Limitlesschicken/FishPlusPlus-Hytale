#pragma once
#include <Windows.h>

#include "Component.h"

class Menu {
public:
	Menu(HDC hdc);

	void Run();

	static bool isMenuOpen();

	static inline std::unique_ptr<Component> mainComponent;
private:

	void m_listenOpenInput();

	inline static bool m_open = true;
};