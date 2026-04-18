/*
 * Copyright (c) FishPlusPlus.
 */
#include "RemoteChest.h"
#include "core.h"
#include "Features/Settings/KeybindSetting.h"
#include "sdk/Packets/SyncInteractionChains.h"
#include "external/nlohman/json.hpp"
#include <fstream>
#include <filesystem>

using json = nlohmann::json;

RemoteChest::RemoteChest() : Feature("RemoteChest") {
	this->ShowTarget = this->RegisterSetting<ToggleSetting>("Highlight Chest", true);
	this->ShowNames = this->RegisterSetting<ToggleSetting>("Show Names", true);
	this->MaxDistance = this->RegisterSetting<SliderSetting>("Max Distance", 50.f, 5.f, 200.f);
	this->SaveChestKeybind = this->RegisterSetting<KeybindSetting>("Save Chest", SDL_SCANCODE_UNKNOWN);
	this->OpenSelectedChestKeybind = this->RegisterSetting<KeybindSetting>("Open Selected", SDL_SCANCODE_UNKNOWN);
	this->NextChestKeybind = this->RegisterSetting<KeybindSetting>("Next Chest", SDL_SCANCODE_UNKNOWN);
	this->PrevChestKeybind = this->RegisterSetting<KeybindSetting>("Prev Chest", SDL_SCANCODE_UNKNOWN);
}

void RemoteChest::OnRender3D(Renderer3D& renderer3D) {
	GameInstance* gameInstance = Util::getGameInstance();
	InteractionModule* interactionModule = gameInstance->InteractionModule;
	Entity* localPlayer = Util::getLocalPlayer();
	if (!localPlayer)
		return;

	Vector3 playerPos = localPlayer->RenderPos;
	float maxDist = this->MaxDistance->GetValue();

	if (interactionModule->lookingAtBlock && interactionModule->TargetedBlockName) {
		std::string blockName = interactionModule->TargetedBlockName->getString();

		if (blockName.find("Chest") != std::string::npos) {
			Vector3 targetPos = interactionModule->TargetedBlockRaycastHit.BlockPosition;

			if (this->ShowTarget->GetValue()) 
				renderer3D.BoxOutline(targetPos, Vector3(1, 1, 1), Color(255, 255, 0, 255));

			if (InputSystem::IsKeyPressed(this->SaveChestKeybind->GetValue()))
				AddOrRemoveChest(targetPos, blockName);
		}
	}

	if (InputSystem::IsKeyPressed(this->NextChestKeybind->GetValue()))
		SelectNextChest();

	if (InputSystem::IsKeyPressed(this->PrevChestKeybind->GetValue()))
		SelectPrevChest();

	if (InputSystem::IsKeyPressed(this->OpenSelectedChestKeybind->GetValue()))
		OpenSelectedChest();

	for (size_t i = 0; i < savedChests.size(); i++) {
		SavedChest& chest = savedChests[i];
		Vector3 chestCenter(chest.position.x + 0.5f, chest.position.y + 0.5f, chest.position.z + 0.5f);

		float dx = chestCenter.x - playerPos.x;
		float dy = chestCenter.y - playerPos.y;
		float dz = chestCenter.z - playerPos.z;
		float dist = sqrtf(dx * dx + dy * dy + dz * dz);

		if (dist > maxDist)
			continue;

		Color chestColor = chest.selected ? Color(0, 255, 0, 255) : Color(255, 0, 255, 255);

		if (this->ShowTarget->GetValue() && dist <= maxDist)
			renderer3D.BoxOutline(Vector3(chest.position.x, chest.position.y, chest.position.z), Vector3(1, 1, 1), chestColor);

		if (this->ShowNames->GetValue() && dist <= maxDist) {
			Vector2 screenPos;
			std::string text = Util::string_format("%s (%.1fm)", chest.name.c_str(), dist);
			if (Util::WorldToScreen(chestCenter, screenPos)) {
				Fonts::Figtree->RenderText(text, screenPos.x - Fonts::Figtree->getWidth(text) / 2, screenPos.y, 1, chest.selected ? Color(0, 255, 0, 255) : Color::White());
			}
		}
	}

	if (selectedChestIndex != -1 && selectedChestIndex < (int)savedChests.size()) {
		SavedChest& selected = savedChests[selectedChestIndex];
		std::string displayText = Util::string_format("Selected: %s", selected.name.c_str());
		Fonts::Figtree->RenderText(displayText, 10, 100, 1, Color(0, 255, 0, 255));
	}
}

bool RemoteChest::CanExecute() {
	if (Util::app->Stage != AppStage::InGame)
		return false;
	return true;
}

void RemoteChest::Initialize() {
	Util::log("Initialized RemoteChest feature");
	RegisterEvent(this);
	LoadChests();
}

void RemoteChest::AddOrRemoveChest(const Vector3& pos, const std::string& blockName) {
	int existingIndex = FindChestIndex(pos);

	if (existingIndex != -1) {
		if (selectedChestIndex == existingIndex) {
			savedChests[selectedChestIndex].selected = false;
			selectedChestIndex = -1;
		}
		savedChests.erase(savedChests.begin() + existingIndex);
		if (selectedChestIndex > existingIndex)
			selectedChestIndex--;
		Util::log("Removed chest at (%.1f, %.1f, %.1f)", pos.x, pos.y, pos.z);
	} else {
		std::string chestName = Util::string_format("Chest #%d", (int)savedChests.size() + 1);
		savedChests.push_back(SavedChest(pos, chestName));
		Util::log("Added chest at (%.1f, %.1f, %.1f)", pos.x, pos.y, pos.z);
	}
	SaveChests();
}

int RemoteChest::FindChestIndex(const Vector3& pos) {
	for (size_t i = 0; i < savedChests.size(); i++) {
		if (savedChests[i].position.x == pos.x && savedChests[i].position.y == pos.y && savedChests[i].position.z == pos.z)
			return (int)i;
	}
	return -1;
}

void RemoteChest::SelectNextChest() {
	if (savedChests.empty())
		return;

	if (savedChests.size() == 1) {
		selectedChestIndex = 0;
		savedChests[0].selected = true;
		return;
	}

	if (selectedChestIndex != -1)
		savedChests[selectedChestIndex].selected = false;

	selectedChestIndex = (selectedChestIndex + 1) % (int)savedChests.size();
	savedChests[selectedChestIndex].selected = true;
}

void RemoteChest::SelectPrevChest() {
	if (savedChests.empty())
		return;

	if (savedChests.size() == 1) {
		selectedChestIndex = 0;
		savedChests[0].selected = true;
		return;
	}

	if (selectedChestIndex != -1)
		savedChests[selectedChestIndex].selected = false;

	selectedChestIndex--;
	if (selectedChestIndex < 0) 
		selectedChestIndex = (int)savedChests.size() - 1;
	savedChests[selectedChestIndex].selected = true;
}

void RemoteChest::OpenSelectedChest() {
	if (selectedChestIndex == -1 || selectedChestIndex >= (int)savedChests.size())
		return;

	Vector3 chestPos = savedChests[selectedChestIndex].position;
	SyncInteractionChainsPacket::SendOpenContainer(chestPos);
	Util::log("Opening chest: %s at (%.1f, %.1f, %.1f)", savedChests[selectedChestIndex].name.c_str(), chestPos.x, chestPos.y, chestPos.z);
}

void RemoteChest::SaveChests() {
	std::filesystem::path configPath = Globals::paths->ClientGameDirectory->getString();
	std::filesystem::path fishDir = configPath / "Fish++";

	if (!std::filesystem::exists(fishDir))
		std::filesystem::create_directory(fishDir);

	std::filesystem::path chestsFile = fishDir / "remote_chests.json";
	std::ofstream file(chestsFile);
	if (!file.is_open())
		return;

	json data;
	data["selectedIndex"] = selectedChestIndex;
	data["chests"] = json::array();

	for (const auto& chest : savedChests) {
		json chestData;
		chestData["name"] = chest.name;
		chestData["x"] = chest.position.x;
		chestData["y"] = chest.position.y;
		chestData["z"] = chest.position.z;
		data["chests"].push_back(chestData);
	}

	file << data.dump(4);
}

void RemoteChest::LoadChests() {
	std::filesystem::path configPath = Globals::paths->ClientGameDirectory->getString();
	std::filesystem::path chestsFile = configPath / "Fish++" / "remote_chests.json";

	if (!std::filesystem::exists(chestsFile)) {
		Util::log("No saved chests file found");
		return;
	}

	std::ifstream file(chestsFile);
	if (!file.is_open()) {
		Util::log("Failed to load chests from file");
		return;
	}

	try {
		json data = json::parse(file);
		savedChests.clear();

		for (const auto& chestData : data["chests"]) {
			Vector3 pos(chestData["x"].get<float>(), chestData["y"].get<float>(), chestData["z"].get<float>());
			std::string name = chestData["name"].get<std::string>();
			savedChests.push_back(SavedChest(pos, name));
		}

		selectedChestIndex = data.value("selectedIndex", -1);
		if (selectedChestIndex >= 0 && selectedChestIndex < (int)savedChests.size())
			savedChests[selectedChestIndex].selected = true;
		else
			selectedChestIndex = -1;

		Util::log("Loaded %d chests from file", (int)savedChests.size());
	} catch (const std::exception& e) {
		Util::log("Error parsing chests file: %s", e.what());
	}
}


