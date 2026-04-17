/*
 * Copyright (c) FishPlusPlus.
 */
#include "WorldModulate.h"
#include "core.h"

#include "Events/EventRegister.h"

WorldModulate::WorldModulate() : Feature("WorldModulate") {
    noFog = this->RegisterSetting<ToggleSetting>("No Fog", false);

    fogChanger = this->RegisterSetting<RecursiveSetting>("Fog Changer", false); {
        fogColor = fogChanger->RegisterSetting<ColorSetting>("Color", Color::Red());
        fogStart = fogChanger->RegisterSetting<SliderSetting>("Start", 100.0f, 0.0f, 200.0f);
        fogEnd = fogChanger->RegisterSetting<SliderSetting>("End", 100.0f, 0.0f, 200.0f);
    }

    timeChanger = this->RegisterSetting<RecursiveSetting>("Time Changer", false); {
        time = timeChanger->RegisterSetting<SliderSetting>("Time", 12.0f, 0.0f, 24.0f);
    }
}

bool WorldModulate::CanExecute() {
    if (Util::app->Stage != AppStage::InGame)
        return false;;
    return true;
}


void WorldModulate::Initialize() {
    Util::log("Initialized WorldModulate feature");
    RegisterEvent(this);
}
