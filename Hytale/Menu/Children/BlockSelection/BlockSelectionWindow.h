/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <Menu/Component.h>
#include "Features/Settings/BlockSelection/BlockSelection.h"
#include "Features/Setting.h"

class SingleSelection;
class SearchBar;

class BlockSelectionScreen : public Component {
public:
	BlockSelectionScreen(Setting<std::vector<BlockSelection>>* setting);

	void RenderList(double deltaTime);

	void Render(double deltaTime) override;
	void Update(float mouseX, float mouseY) override;

	void MouseClicked(float mouseX, float mouseY, int vk);
	void MouseScrolled(int amount) override;

private:
	float scrollOffset = 0.0f;
	float scrollSensitivity = 20.0f;

	std::vector<SingleSelection> selections;
	std::unique_ptr<SearchBar> searchBar;
};

class SearchBar : public Component {
public:
	SearchBar() {};

	void Render(double deltaTime) override;
	void Update(float mouseX, float mouseY) override;

	void MouseClicked(float mouseX, float mouseY, int vk);

private:
	bool hovered = false;
	bool typing = false;
	std::string currentTyped = "";
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
private:
	std::vector<std::string> categories;
	BlockSelection selection;

	
	float hoverAlpha = 0.0f;
};