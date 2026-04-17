/*
 * Copyright (c) FishPlusPlus.
 */
#include "BlockESP.h"

#include "core.h"

BlockESP::BlockESP() : Feature("BlockESP") {

	this->blockSetting = this->RegisterSetting<BlockSetting>("BlockSetting");

	this->radius   = this->RegisterSetting<SliderSetting>("Radius", 50.f, 5.f, 500.f);
	this->showName = this->RegisterSetting<ToggleSetting>("Show Name", false);
	this->showChests = this->RegisterSetting<ToggleSetting>("Show Chests", true);
	this->showBenches = this->RegisterSetting<ToggleSetting>("Show Benches", true);
	this->showAdamantite = this->RegisterSetting<ToggleSetting>("Show Adamantite", false);
	this->showGold = this->RegisterSetting<ToggleSetting>("Show Gold", false);
	this->showSilver = this->RegisterSetting<ToggleSetting>("Show Silver", false);
	this->showIron = this->RegisterSetting<ToggleSetting>("Show Iron", false);
	this->showCopper = this->RegisterSetting<ToggleSetting>("Show Copper", false);
	this->showMithril = this->RegisterSetting<ToggleSetting>("Show Mithril", false);
	this->showCobalt = this->RegisterSetting<ToggleSetting>("Show Cobalt", false);
	this->showThorium = this->RegisterSetting<ToggleSetting>("Show Thorium", false);
	this->showTreasure = this->RegisterSetting<ToggleSetting>("Show Treasure", true);
	this->showCrystals = this->RegisterSetting<ToggleSetting>("Show Crystals", true);
	this->showGems = this->RegisterSetting<ToggleSetting>("Show Gems", true);
	this->refreshList = this->RegisterSetting<ToggleSetting>("Refresh List", false);
}

void BlockESP::OnRender3D(Renderer3D& renderer3D) {
	Entity* localPlayer = Util::getLocalPlayer();
	Vector3 playerPos = localPlayer->RenderPos;

	SDK::filteredBlockMutex.lock();
	for (const auto& block : SDK::filteredBlocks) {
		Vector3 blockCenter(block.position.x + 0.5f, block.position.y + 0.5f, block.position.z + 0.5f);
		float dx = blockCenter.x - playerPos.x;
		float dy = blockCenter.y - playerPos.y;
		float dz = blockCenter.z - playerPos.z;
		float distSq = dx * dx + dy * dy + dz * dz;

		if (distSq > this->radius->GetValue() * this->radius->GetValue())
			continue;

		renderer3D.BoxOutline(
			Vector3((int)block.position.x, (int)block.position.y, (int)block.position.z),
			Vector3(1, 1, 1),
			block.color
		);

		if (this->showName->GetValue()) {
			Vector2 screenPos;
			std::string text = Util::string_format("%s (%.1fm)", block.displayName.c_str(), sqrtf(distSq));
			if (Util::WorldToScreen(blockCenter, screenPos)) {
				Fonts::Figtree->RenderText(
					text,
					screenPos.x - Fonts::Figtree->getWidth(text) / 2,
					screenPos.y,
					1,
					Color::White()
				);
			}
		}
	}
	SDK::filteredBlockMutex.unlock();
}

bool BlockESP::CanExecute() {
	if (Util::app->Stage != AppStage::InGame)
		return false;
	return true;
}

void BlockESP::Initialize() {
	Util::log("Initialized BlockESP feature");
	RegisterEvent(this);
}

bool BlockESP::IsBlockImportant(int settingID) {
	switch (settingID) {
		case 0: return this->showChests->GetValue();
		case 1: return this->showBenches->GetValue();
		case 2: return this->showAdamantite->GetValue();
		case 3: return this->showGold->GetValue();
		case 4: return this->showSilver->GetValue();
		case 5: return this->showIron->GetValue();
		case 6: return this->showCopper->GetValue();
		case 7: return this->showMithril->GetValue();
		case 8: return this->showCobalt->GetValue();
		case 9: return this->showThorium->GetValue();
		case 10: return this->showTreasure->GetValue();
		case 11: return this->showCrystals->GetValue();
		case 12: return this->showGems->GetValue();
		default: return false;
	}
}

