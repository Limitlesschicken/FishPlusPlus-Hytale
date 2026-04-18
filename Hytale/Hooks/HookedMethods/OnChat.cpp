/*
 * Copyright (c) FishPlusPlus.
 */
#include "../Hooks.h"

#include "Features/ConfigHandler.h"
#include "Features/FeatureHandler.h"
#include "Features/ActualFeatures/RemoteChest.h"

#pragma optimize("", off)
#pragma runtime_checks("", off)

void tpCommand(std::string command) {
    std::istringstream iss(command);
    float x;
    float y;
    float z;
    if (iss >> x >> y >> z) {
        HookData::queueTeleport = true;
        HookData::teleportTarget = Vector3(x, y, z);
    }
}

void configCommand(std::string command) {
	if (command.starts_with("save ")) {
		std::string name = command.substr(5);
		ConfigHandler::SaveConfig(name, true);
	}
	else if (command.starts_with("load ")) {
		std::string name = command.substr(5);
		ConfigHandler::LoadConfig(name, true);
	}
}

void renameCommand(std::string command) {
	RemoteChest* remoteChest = (RemoteChest*)FeatureHandler::GetFeatureFromName("RemoteChest");
	if (!remoteChest) {
		Util::log("RemoteChest feature not found");
		return;
	}

	if (remoteChest->selectedChestIndex == -1 || remoteChest->selectedChestIndex >= (int)remoteChest->savedChests.size()) {
		Util::log("No chest selected. Use Next/Prev Chest keybinds to select a chest first.");
		return;
	}

	std::string newName = command;
	if (newName.empty()) {
		Util::log("Usage: !rename <new name>");
		return;
	}

	remoteChest->savedChests[remoteChest->selectedChestIndex].name = newName;
	remoteChest->SaveChests();
	Util::log("Renamed chest to: %s", newName.c_str());
}

void deleteChestCommand() {
	RemoteChest* remoteChest = (RemoteChest*) FeatureHandler::GetFeatureFromName("RemoteChest");
	if (!remoteChest) {
		Util::log("RemoteChest feature not found");
		return;
	}
	remoteChest->RemoveCurrentChest();
}

__declspec(safebuffers) __declspec(noinline)
void __fastcall Hooks::hkOnChat(uint64_t instance, HytaleString* chatString) {
    std::string message = chatString->getString();
    //Util::log("Chat message: %s", message.c_str());

    if (!message.starts_with('!')) {
        Hooks::oOnChat(instance, chatString);
        return;
    }

	std::string command = message.substr(1);

	if (command.starts_with("tp "))
		tpCommand(command.substr(3));

	else if (command.starts_with("config "))
		configCommand(command.substr(7));

	else if (command.starts_with("rename "))
		renameCommand(command.substr(7));

	else if (command.starts_with("delchest"))
		deleteChestCommand();
}
#pragma runtime_checks("", restore)
#pragma optimize("", on)