/*
 * Copyright (c) FishPlusPlus.
 */
#include "../Hooks.h"

#include "Features/ActualFeatures/MainMenuEffect.h"
#include "Features/FeatureHandler.h"

#pragma optimize("", off)
#pragma runtime_checks("", off)
__declspec(safebuffers) __declspec(noinline)
void __fastcall Hooks::hkMainMenuPostEffectRendererDraw(void* instance, int a2, int a3, int a4, int a5, int a6, void* a7) {
	Hooks::oMainMenuPostEffectRendererDraw(instance, a2, a3, a4, a5, a6, a7);

	if (!initialized)
		return;

	if (Util::app->Stage != AppStage::MainMenu)
		return;

	const auto& effect = FeatureHandler::GetFeatureFromName<MainMenuEffect>("Main Menu");

	if (!effect->IsActive())
		return;

	Renderer2D::menuShaders->Square(Vector2(0, 0), (float)Util::app->Engine->Window->WindowWidth, (float)Util::app->Engine->Window->WindowHeight, Color::Normalize(50, 50, 255, 100));
	Renderer2D::menuShaders->Render();
}
#pragma runtime_checks("", restore)
#pragma optimize("", on)