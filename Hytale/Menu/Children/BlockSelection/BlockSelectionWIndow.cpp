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

BlockSelectionScreen::BlockSelectionScreen(Setting<std::vector<BlockSelection>>* setting) {
	for (const OfflineBlock& block : VANILLA_BLOCKS) {
		auto* s = static_cast<BlockSetting*>(setting);
		this->selections.emplace_back(BlockSelection((std::string)block.name, s->defaultCategory), s->categories);
	}

	this->searchBar = std::make_unique<SearchBar>();

	this->SetX(710.0f);
	this->SetY(80.0f);
	this->SetWidth(1000.0f);
	this->SetHeight(600.0f);
}

void BlockSelectionScreen::RenderList(double deltaTime) {
	if (scrollOffset >= 0)
		scrollOffset = 0;

	float baseListOffset = 30.0f;
	float yOffset = scrollOffset;

	for (SingleSelection& selection : this->selections) {

		float yPos = this->GetY() + yOffset;

		selection.SetY(yPos + baseListOffset);
		selection.SetX(this->GetX());

		if (yPos < this->GetY() - 20) {
			yOffset += selection.GetHeight();
			continue;
		}

		if (yPos > this->GetY() + this->GetHeight())
			break;

		if (yOffset >= this->GetHeight())
			break;

		selection.Render(deltaTime);

		yOffset += selection.GetHeight();
	}
}

void BlockSelectionScreen::Render(double deltaTime) {
	Renderer2D::colored->Square(Vector2(this->GetX(), this->GetY()), this->GetWidth(), this->GetHeight() + 30.0f, Color::Normalize(Style::tabBgColor));
	Renderer2D::colored->Render();

	Renderer2D::colored->BeginScissor(this->GetX(), this->GetY() + 30.0f, this->GetWidth(), this->GetHeight());
	RenderList(deltaTime);
	Renderer2D::colored->EndScissor();

	this->searchBar->Render(deltaTime);
}

void  BlockSelectionScreen::MouseScrolled(int amount) {
	scrollOffset += amount * scrollSensitivity;
}

void BlockSelectionScreen::Update(float mouseX, float mouseY) {
	for (SingleSelection& selection : this->selections) {
		selection.Update(mouseX, mouseY);
	}

	this->searchBar->SetX(this->x);
	this->searchBar->SetY(this->y);
	this->searchBar->SetWidth(this->width);
	this->searchBar->SetHeight(30.0f);
	this->searchBar->Update(mouseX, mouseY);
}

void BlockSelectionScreen::MouseClicked(float mouseX, float mouseY, int vk) {
	
	for (SingleSelection& selection : this->selections) {
		selection.MouseClicked(mouseX, mouseY, vk);
	}
	searchBar->MouseClicked(mouseX, mouseY, vk);
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

	for (SDL_Scancode scancode : InputSystem::keysPressed) {
		//this->currentTyped.append(scancode)
	}
	
}
void SearchBar::MouseClicked(float mouseX, float mouseY, int vk) {
	if (!this->hovered)
		return;

	if (vk != VK_LBUTTON)
		return;

	

	this->typing = !this->typing;
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

	Fonts::Figtree->RenderText(this->selection.name, 710, this->y + Fonts::Figtree->getHeight() / 4, 1, Color::White());

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
}