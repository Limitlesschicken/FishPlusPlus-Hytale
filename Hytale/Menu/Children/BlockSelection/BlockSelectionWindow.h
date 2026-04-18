/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <Menu/Component.h>
#include "Features/Settings/BlockSelection/BlockSelection.h"
#include "Features/Setting.h"

class SingleSelection;
class SearchBar;
class GlobalCategory;
class ApplyButton;

class BlockSelectionScreen : public Component {
public:
	BlockSelectionScreen(Setting<std::vector<BlockResult>>* setting);

	void RenderList(double deltaTime);

	void Render(double deltaTime) override;
	void Update(float mouseX, float mouseY) override;

	void SyncToSetting();

	void MouseClicked(float mouseX, float mouseY, int vk);
	void MouseScrolled(int amount) override;

private:
	float scrollOffset = 0.0f;
	float scrollSensitivity = 20.0f;

	Setting<std::vector<BlockResult>>* setting;

	std::vector<SingleSelection> selections;
	std::unique_ptr<SearchBar> searchBar;
	std::unique_ptr<GlobalCategory> globalCategory;
	std::unique_ptr<ApplyButton> applyButton;
};

class ApplyButton : public Component {
public:
	ApplyButton() {};

	void Render(double deltaTime) override;
	void Update(float mouseX, float mouseY) override;

	void MouseClicked(float mouseX, float mouseY, int vk);

	bool clicked = false;
private:
	bool hovered = false;
};

class GlobalCategory : public Component {
public:
	GlobalCategory(std::string& defaultCategory, std::vector<std::string>& categories) : defaultCategory(defaultCategory), categories(categories) {
		this->selectedCategory = defaultCategory;
	};

	void Render(double deltaTime) override;
	void Update(float mouseX, float mouseY) override;

	void MouseClicked(float mouseX, float mouseY, int vk);

	std::string selectedCategory;
private:
	std::string defaultCategory;
	std::vector<std::string> categories;
	bool hovered = false;
};

class SearchBar : public Component {
public:
	SearchBar() {};

	void Render(double deltaTime) override;
	void Update(float mouseX, float mouseY) override;

	void MouseClicked(float mouseX, float mouseY, int vk);

	std::string currentTyped = "";
	bool justTyped = false;
private:
	bool hovered = false;
	bool typing = false;
	
};

class SingleSelection : public Component {
public:
	SingleSelection(BlockSelection selection, std::vector<std::string>& categories) : selection(selection), categories(categories) {
		this->SetWidth(1000.0f);
		this->SetHeight(30.0f);
	};

	void Render(double deltaTime) override;
	void Update(float mouseX, float mouseY) override;

	void MouseClicked(float mouseX, float mouseY, int vk);

	bool hovered = false;
	BlockSelection selection;

private:
	std::vector<std::string> categories;
	
	float hoverAlpha = 0.0f;
};