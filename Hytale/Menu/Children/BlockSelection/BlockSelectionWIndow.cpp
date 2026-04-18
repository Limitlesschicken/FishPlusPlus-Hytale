/*
 * Copyright (c) FishPlusPlus.
 */

#include "BlockSelectionWindow.h"

#include "../../Style.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/FontRenderer/Fonts.h"
#include "sdk/Hytale/MapModule.h"
#include <Features/Settings/BlockSelection/BlockSelection.h>
#include <Features/Settings/BlockSelection/BlockSetting.h>

BlockSelectionScreen::BlockSelectionScreen(Setting<std::vector<BlockResult>>* setting) {
	auto* s = static_cast<BlockSetting*>(setting);
	this->setting = s;
	for (const OfflineBlock& block : VANILLA_BLOCKS) {
		this->selections.emplace_back(BlockSelection((std::string)block.name, s->defaultCategory), s->categories);
	}

	this->searchBar = std::make_unique<SearchBar>();
	this->globalCategory = std::make_unique<GlobalCategory>(s->defaultCategory, s->categories);
	this->applyButton = std::make_unique<ApplyButton>();

	this->SetX(710.0f);
	this->SetY(80.0f);
	this->SetWidth(1000.0f);
	this->SetHeight(600.0f);

}

void BlockSelectionScreen::RenderList(double deltaTime) {
	if (scrollOffset >= 0)
		scrollOffset = 0;

	if (this->searchBar->justTyped)
		scrollOffset = 0;

	float baseListOffset = 30.0f;
	float yOffset = scrollOffset;

	std::string query = searchBar->currentTyped;

	for (SingleSelection& selection : this->selections) {

		if (!query.empty()) {
			std::string name = selection.selection.name;
			if (name.find(query) == std::string::npos) {
				continue;
			}
		}

		if (this->applyButton->clicked) {
			selection.selection.category = this->globalCategory->selectedCategory;
		}

		float yPos = this->GetY() + yOffset;

		selection.SetY(yPos + baseListOffset);
		selection.SetX(this->GetX());

		if (yPos < this->GetY() - 20) {
			yOffset += selection.GetHeight();
			continue;
		}

		if (yPos > this->GetY() + this->GetHeight())
			continue;

		if (yOffset >= this->GetHeight())
			continue;

		selection.Render(deltaTime);

		yOffset += selection.GetHeight();
	}

	if (this->applyButton->clicked){
		this->applyButton->clicked = false;
		SyncToSetting();
	}
}


void BlockSelectionScreen::SyncToSetting() {
	
	auto* s = static_cast<BlockSetting*>(setting);

	std::vector<BlockResult> results;
	results.reserve(selections.size());

	for (const auto& sel : selections) {

		if (sel.selection.category == s->defaultCategory)
			continue;

		results.push_back({
			Hash::HashCI(sel.selection.name.c_str()),
			sel.selection.category
			});
	}

	this->setting->SetValue(results);
	
}

void BlockSelectionScreen::Render(double deltaTime) {


	Renderer2D::colored->Square(Vector2(this->GetX(), this->GetY()), this->GetWidth(), this->GetHeight() + 30.0f, Color::Normalize(Style::tabBgColor));
	//header
	Renderer2D::colored->Square(Vector2(this->GetX(), this->GetY()), this->GetWidth(), 30.0f, Color::Normalize(Style::headerColor));
	Renderer2D::colored->Render();

	Renderer2D::colored->BeginScissor(this->GetX(), this->GetY() + 30.0f, this->GetWidth(), this->GetHeight());
	RenderList(deltaTime);
	Renderer2D::colored->EndScissor();

	this->searchBar->Render(deltaTime);
	this->globalCategory->Render(deltaTime);
	this->applyButton->Render(deltaTime);
}

void  BlockSelectionScreen::MouseScrolled(int amount) {
	scrollOffset += amount * scrollSensitivity;
}

void BlockSelectionScreen::Update(float mouseX, float mouseY) {
	for (SingleSelection& selection : this->selections) {
		selection.Update(mouseX, mouseY);
		if (mouseY <= this->y + 30.0f)
			selection.hovered = false;
	}

	this->searchBar->SetX(this->x);
	this->searchBar->SetY(this->y);
	this->searchBar->SetWidth(this->width - 300.0f);
	this->searchBar->SetHeight(30.0f);
	this->searchBar->Update(mouseX, mouseY);

	this->globalCategory->SetX(this->x + 700.0f);
	this->globalCategory->SetY(this->y);
	this->globalCategory->SetWidth(150.0f);
	this->globalCategory->SetHeight(30.0f);
	this->globalCategory->Update(mouseX, mouseY);

	this->applyButton->SetX(this->x + 850.0f);
	this->applyButton->SetY(this->y);
	this->applyButton->SetWidth(150.0f);
	this->applyButton->SetHeight(30.0f);
	this->applyButton->Update(mouseX, mouseY);
}

void BlockSelectionScreen::MouseClicked(float mouseX, float mouseY, int vk) {
	for (SingleSelection& selection : this->selections) {
		selection.MouseClicked(mouseX, mouseY, vk);
	}
	searchBar->MouseClicked(mouseX, mouseY, vk);
	globalCategory->MouseClicked(mouseX, mouseY, vk);
	applyButton->MouseClicked(mouseX, mouseY, vk);
}

void SearchBar::Render(double deltaTime) {


	if (this->typing) {
		if (this->currentTyped.empty())
			Fonts::Figtree->RenderText("...", 710, this->y + Fonts::Figtree->getHeight() / 4, 1, Color::White());	
		else
			Fonts::Figtree->RenderText(this->currentTyped, 710, this->y + Fonts::Figtree->getHeight() / 4, 1, Color::White());
	} else {
		if (this->currentTyped.empty())
			Fonts::Figtree->RenderText("Search Bar", 710, this->y + Fonts::Figtree->getHeight() / 4, 1, Color::White());
		else
			Fonts::Figtree->RenderText(this->currentTyped, 710, this->y + Fonts::Figtree->getHeight() / 4, 1, Color::White());
	}
}
void SearchBar::Update(float mouseX, float mouseY) {
	this->hovered = this->IsHovered(mouseX, mouseY);

	justTyped = false;
	for (SDL_Scancode scancode : InputSystem::keysPressed) {
		if(!typing)
			continue;

		justTyped = true;

		if (scancode == SDL_SCANCODE_BACKSPACE) {
			if (!currentTyped.empty())
				currentTyped.pop_back();
			continue;
		}

		char c = InputSystem::ScancodeToChar(scancode, InputSystem::shiftHeld);
		if (c != 0) {
			currentTyped.push_back(c);
		}
	}
}
void SearchBar::MouseClicked(float mouseX, float mouseY, int vk) {
	if (!this->hovered)
		return;

	if (vk != VK_LBUTTON)
		return;

	this->typing = !this->typing;
}

void RemoveSubstring(std::string& str, const std::string& sub) {
	size_t pos;
	while ((pos = str.find(sub)) != std::string::npos) {
		str.erase(pos, sub.length());
	}
}

void SingleSelection::Render(double deltaTime) {
	double fastDeltaTime = deltaTime * 20.0;

	this->hoverAlpha += (this->hovered ? 50.0f : -50.0f) * (float)fastDeltaTime;
	if (this->hoverAlpha > Style::featureHoverColor.a)
		this->hoverAlpha = Style::featureHoverColor.a;
	if (this->hoverAlpha < 0)
		this->hoverAlpha = 0;

	Color topColor = Color(Style::featureHoverColor.r, Style::featureHoverColor.g, Style::featureHoverColor.b, this->hoverAlpha);
	Color bottomColor = Color(Style::featureHoverGradColor.r, Style::featureHoverGradColor.g, Style::featureHoverGradColor.b, this->hoverAlpha);

	Renderer2D::colored->SquareMultiColor(Vector2(x, y), width, height,
		Color::Normalize(topColor), Color::Normalize(topColor),
		Color::Normalize(bottomColor), Color::Normalize(bottomColor));
	Renderer2D::colored->Render();

	std::string nameCopy = this->selection.name;


	RemoveSubstring(nameCopy, "Definitions");
	RemoveSubstring(nameCopy, "State");

	Fonts::Figtree->RenderText(nameCopy, 710, this->y + Fonts::Figtree->getHeight() / 4, 1, Color::White());

	Fonts::Figtree->RenderText(this->selection.category, x + width - Fonts::Figtree->getWidth(this->selection.category) - 6.0f, y + Style::settingsNamePadding.y, 1.0f, Color::Normalize(Color::White()));
}
void SingleSelection::Update(float mouseX, float mouseY) {
	this->hovered = this->IsHovered(mouseX, mouseY);
}

void SingleSelection::MouseClicked(float mouseX, float mouseY, int vk) {
	if (!this->hovered)
		return;

	if (vk != VK_LBUTTON)
		return;

	int currentCategoryIndex = 0;
	for (int i = 0; i < this->categories.size(); i++) {
		if (this->categories[i] == this->selection.category) {
			currentCategoryIndex = i;
			break;
		}
	}
	currentCategoryIndex = (currentCategoryIndex + 1) % this->categories.size();
	
	this->selection.category = this->categories.at(currentCategoryIndex);

	//this->screen->SyncToSetting();
}

void ApplyButton::Render(double deltaTime) {
	Renderer2D::colored->Square(Vector2(x, y), width, height, Color::Normalize(Color(20, 20, 20, 255)));
	Renderer2D::colored->Render();
	Fonts::Figtree->RenderText("Apply all", (this->x + this->width / 2) - Fonts::Figtree->getWidth("Apply all") / 2, this->y + Fonts::Figtree->getHeight() / 4, 1, Color::White());
}
void ApplyButton::Update(float mouseX, float mouseY) {
	this->hovered = this->IsHovered(mouseX, mouseY);
}

void ApplyButton::MouseClicked(float mouseX, float mouseY, int vk) {
	if (!this->hovered)
		return;

	if (vk != VK_LBUTTON)
		return;

	clicked = true;
}


void  GlobalCategory::Render(double deltaTime) {
	Renderer2D::colored->Square(Vector2(x, y), width, height, Color::Normalize(Color(10, 10, 10, 255)));
	Renderer2D::colored->Render();
	Fonts::Figtree->RenderText(this->selectedCategory, (this->x + this->width / 2) - Fonts::Figtree->getWidth(this->selectedCategory) / 2, this->y + Fonts::Figtree->getHeight() / 4, 1, Color::White());
}
void  GlobalCategory::Update(float mouseX, float mouseY) {
	this->hovered = this->IsHovered(mouseX, mouseY);
}
void  GlobalCategory::MouseClicked(float mouseX, float mouseY, int vk) {
	if (!this->hovered)
		return;

	if (vk != VK_LBUTTON)
		return;

	int currentCategoryIndex = 0;
	for (int i = 0; i < this->categories.size(); i++) {
		if (this->categories[i] == this->selectedCategory) {
			currentCategoryIndex = i;
			break;
		}
	}
	currentCategoryIndex = (currentCategoryIndex + 1) % this->categories.size();

	this->selectedCategory = this->categories.at(currentCategoryIndex);
}
