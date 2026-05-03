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

void rtpCommand(std::string command) {
	std::istringstream iss(command);
	float x;
	float y;
	float z;
	if (iss >> x >> y >> z) {
		Entity* player = Util::getLocalPlayer();
		HookData::queueTeleport = true;
		HookData::teleportTarget = Vector3(player->Position.x + x, player->Position.y + y, player->Position.z + z);
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
	const auto& remoteChest = FeatureHandler::GetFeatureFromName<RemoteChest>("RemoteChest");

	if (remoteChest->selectedChestIndex == -1 || remoteChest->selectedChestIndex >= (int)remoteChest->savedChests.size()) {
		Util::log("No chest selected. Use Next/Prev Chest keybinds to select a chest first.");
		return;
	}

	std::string newName = command;
	if (newName.empty())
		return;

	remoteChest->savedChests[remoteChest->selectedChestIndex].name = newName;
	remoteChest->SaveChests();
	Util::log("Renamed chest to: %s", newName.c_str());
}

void deleteChestCommand() {
	const auto& remoteChest = FeatureHandler::GetFeatureFromName<RemoteChest>("RemoteChest");
	remoteChest->RemoveCurrentChest();
}

void saveChestsCommand(std::string command) {
	const auto& remoteChest = FeatureHandler::GetFeatureFromName<RemoteChest>("RemoteChest");

	std::string profileName = command;
	if (profileName.empty()) 
		return;

	remoteChest->SaveChests(profileName);
}

void loadChestsCommand(std::string command) {
	const auto& remoteChest = FeatureHandler::GetFeatureFromName<RemoteChest>("RemoteChest");

	std::string profileName = command;
	if (profileName.empty())
		return;

	remoteChest->LoadChests(profileName);
}

void listChestsCommand() {
	const auto& remoteChest = FeatureHandler::GetFeatureFromName<RemoteChest>("RemoteChest");

	remoteChest->ShowChestLists();
}

void clearChestsCommand() {
	const auto& remoteChest = FeatureHandler::GetFeatureFromName<RemoteChest>("RemoteChest");

	remoteChest->ClearChests();
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

	if (command.starts_with("rtp "))
		rtpCommand(command.substr(4));

	else if (command.starts_with("config "))
		configCommand(command.substr(7));

	else if (command.starts_with("rename "))
		renameCommand(command.substr(7));

	else if (command.starts_with("delchest"))
		deleteChestCommand();

	else if (command.starts_with("savechests "))
		saveChestsCommand(command.substr(11));

	else if (command.starts_with("loadchests "))
		loadChestsCommand(command.substr(11));

	else if (command.starts_with("listchests"))
		listChestsCommand();

	else if (command.starts_with("clearchests"))
		clearChestsCommand();
}
#pragma runtime_checks("", restore)
#pragma optimize("", on)