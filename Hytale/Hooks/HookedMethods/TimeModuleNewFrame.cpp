/*
 * Copyright (c) FishPlusPlus.
 */
#include "../Hooks.h"
#include "Events/EventRegister.h"
#include "Features/FeatureHandler.h"
#include "Features/ActualFeatures/WorldModulate.h"

#pragma optimize("", off)
#pragma runtime_checks("", off)
__declspec(safebuffers) __declspec(noinline)
void __fastcall Hooks::hkTimeModuleNewFrame(void* instance, float deltaTime) {
	Hooks::oTimeModuleNewFrame(instance, deltaTime);
    if (!Util::app)
        return;
    if (Util::app->Stage != AppStage::InGame)
        return;

    const auto& worldModulate = FeatureHandler::GetFeatureFromName<WorldModulate>("WorldModulate");
    if (!worldModulate)
        return;
    if (!worldModulate->IsActive())
        return;

    RecursiveSetting* timeChanger = static_cast<RecursiveSetting*>(worldModulate->GetSettingFromName("Time Changer"));

    if (timeChanger->GetValue()) {
        float time = static_cast<SliderSetting*>(timeChanger->GetSettingFromName("Time"))->GetValue();
        Util::getGameInstance()->TimeModule->WorldTime = time;
    }
}
#pragma runtime_checks("", restore)
#pragma optimize("", on)