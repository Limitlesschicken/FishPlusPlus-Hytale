/*
 * Copyright (c) FishPlusPlus.
 */
#include "ConfigHandler.h"

#include <filesystem>

#include "Core.h"

#include "external/nlohman/json.hpp"
#include "Features/FeatureHandler.h"
#include "Features/Feature.h"
#include "Settings/ToggleSetting.h"
#include "Settings/SliderSetting.h"
#include "Settings/MultiSetting.h"
#include "Settings/ColorSetting.h"
#include "Settings/RecursiveSetting.h"
#include "Settings/KeybindSetting.h"

using json = nlohmann::json;

void ConfigHandler::CreateFishDirectory() {
	std::filesystem::path configPath = Globals::paths->ClientGameDirectory->getString();
	std::filesystem::create_directory(configPath / "Fish++");
	std::filesystem::create_directory(configPath / "Fish++" / "configs");
	Util::log("Created Fish++ directory in client game path");
}

bool ConfigHandler::FishDirectoryExists() {
	std::filesystem::path configPath = Globals::paths->ClientGameDirectory->getString();
	return std::filesystem::exists(configPath / "Fish++") && std::filesystem::exists(configPath / "Fish++" / "configs");
}

void addSetting(nlohmann::json& json, ISetting* setting) {
	if (auto toggleSetting = dynamic_cast<ToggleSetting*>(setting))
		json[setting->GetName()] = toggleSetting->GetValue();
	else if (auto multiSetting = dynamic_cast<MultiSetting*>(setting))
		json[setting->GetName()] = multiSetting->GetValue();
	else if (auto sliderSetting = dynamic_cast<SliderSetting*>(setting))
		json[setting->GetName()] = sliderSetting->GetValue();
	else if (auto keybindSetting = dynamic_cast<KeybindSetting*>(setting))
		json[setting->GetName()] = keybindSetting->GetValue();
	else if (auto colorSetting = dynamic_cast<ColorSetting*>(setting)) {
		Color c = colorSetting->GetValue();
		json[setting->GetName()] = { {"r", c.r}, {"g", c.g}, {"b", c.b}, {"a", c.a}};
	}
	else if (auto recursiveSetting = dynamic_cast<RecursiveSetting*>(setting)) {
		json[setting->GetName()]["enabled"] = recursiveSetting->GetValue();
		for (auto& subSetting : recursiveSetting->GetSettings()) {
			addSetting(json[setting->GetName()], subSetting.get());
		}
	}
}

void ConfigHandler::SaveConfig(std::string name, bool inConfigDirectory) {
	if (!FishDirectoryExists()) {
		CreateFishDirectory();
	}

	std::filesystem::path configPath = Globals::paths->ClientGameDirectory->getString();
	std::filesystem::path base = configPath / "Fish++";
	name += ".json";
	std::ofstream configFile(inConfigDirectory ? base / "configs" / name : base / name);
	if (!configFile.is_open()) {
		Util::log("Failed to open config file for writing");
		return;
	}
	json config;
	for (auto& feature : FeatureHandler::features) {
		auto& jsonCategory = config[feature->GetCategory()];
		auto& jsonFeature = jsonCategory[feature->GetName()];

		jsonFeature["enabled"] = feature->IsActive();
		jsonFeature["keybind"] = feature->GetKeybind();

		for (auto& setting : feature->GetSettings()) {
			addSetting(jsonFeature, setting.get());
		}
	}

	configFile << config.dump(4);
	
}

void loadSetting(json settingData, ISetting* setting) {
	if (auto toggleSetting = dynamic_cast<ToggleSetting*>(setting))
		toggleSetting->SetValue(settingData.get<bool>());
	else if (auto multiSetting = dynamic_cast<MultiSetting*>(setting))
		multiSetting->SetValue(settingData.get<int>());
	else if (auto sliderSetting = dynamic_cast<SliderSetting*>(setting))
		sliderSetting->SetValue(settingData.get<float>());
	else if (auto sliderSetting = dynamic_cast<KeybindSetting*>(setting))
		sliderSetting->SetValue(settingData.get<SDL_Scancode>());
	else if (auto colorSetting = dynamic_cast<ColorSetting*>(setting)) {
		colorSetting->SetValue(Color(
			settingData["r"].get<float>(),
			settingData["g"].get<float>(),
			settingData["b"].get<float>(),
			settingData["a"].get<float>()
		));
	}
	else if (auto recursiveSetting = dynamic_cast<RecursiveSetting*>(setting)) {
		recursiveSetting->SetValue(settingData.value("enabled", false));
		for (auto& [subSettingName, subSettingData] : settingData.items()) {
			if (subSettingName == "enabled")
				continue;
			ISetting* subSetting = recursiveSetting->GetSettingFromName(subSettingName);
			if (!subSetting)
				continue;
			loadSetting(subSettingData, subSetting);
		}
	}
}

void ConfigHandler::LoadConfig(std::string name, bool inConfigDirectory) {
	std::filesystem::path configPath = Globals::paths->ClientGameDirectory->getString();
	std::filesystem::path base = configPath / "Fish++";
	name += ".json";
	std::ifstream configFile(inConfigDirectory ? base / "configs" / name : base / name);

	if (!configFile.is_open()) {
		Util::log("Failed to open config file for reading");
		return;
	}
	json config;
	config = json::parse(configFile);
	for (auto& [categoryName, categoryData] : config.items()) {

		for (auto& [featureName, featureData] : categoryData.items()) {
			Feature* feature = FeatureHandler::GetFeatureFromName(featureName);
			if (!feature) {
				Util::log("Warning: Feature '%s' found in config but not registered in cheat. Skipping.", featureName.c_str());
				continue;
			}
			feature->setActive(featureData.value("enabled", false));
			feature->SetKeybind(featureData.value("keybind", SDL_SCANCODE_UNKNOWN));
			KeybindSetting* keybindSetting = static_cast<KeybindSetting*>(feature->GetSettingFromName("Keybind"));
			if (keybindSetting)
				keybindSetting->SetValue(featureData.value("keybind", SDL_SCANCODE_UNKNOWN));

			for (auto& [settingName, settingData] : featureData.items()) {
				if (settingName == "enabled")
					continue;
				if (settingName == "keybind")
					continue;

				ISetting* setting = feature->GetSettingFromName(settingName);
				if (!setting)
					continue;

				loadSetting(settingData, setting);
			}
		}
	}
}