/*
 * Copyright (c) FishPlusPlus.
 */
#include "Gamemode.h"
#include "Core.h"

Gamemode::Gamemode() : Feature("Gamemode") {
}

void MemoryPatch(void* address, const uint8_t* patch, size_t size) {
	DWORD oldProtect;
	VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(address, patch, size);
	VirtualProtect(address, size, oldProtect, &oldProtect);
}

void Gamemode::OnMoveCycle(DefaultMovementController* dmc, Vector3& offset) {
	if (!Util::isFullyInitialized() || !Globals::pGamemodeInstance)
		return;

	void* targetObj = Globals::pGamemodeInstance;

	// 0: Adventure, 1: Creative
	uint8_t* gm = (uint8_t*)((uintptr_t)targetObj + 0x0A);
	
	if (this->IsActive()) {
		if (*gm != 1) {
			*gm = 1;
			Util::log("Set gamemode to Creative at 0x%llX\n", (uintptr_t)targetObj);
		}
	} else {
		if (*gm != 0) {
			*gm = 0;
			Util::log("Set gamemode to Adventure at 0x%llX\n", (uintptr_t)targetObj);
		}
	}
}

void Gamemode::OnDeactivate() {
	if (Globals::pGamemodeInstance) {
		void* targetObj = Globals::pGamemodeInstance;
		uint8_t* gm = (uint8_t*)((uintptr_t)targetObj + 0x0A);
		if (*gm != 0) {
			*gm = 0;
			Util::log("Deactivated: Set gamemode to Adventure at 0x%llX\n", (uintptr_t)targetObj);
		}
	}
}

void Gamemode::Initialize() {
	Util::log("Initialized Gamemode feature\n");
	RegisterEvent(this);
}

bool Gamemode::CanExecute() {
	return Util::isFullyInitialized();
}
