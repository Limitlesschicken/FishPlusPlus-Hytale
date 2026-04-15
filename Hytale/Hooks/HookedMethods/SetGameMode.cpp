/*
 * Copyright (c) FishPlusPlus.
 */
#include "../Hooks.h"
#include "../../Features/FeatureHandler.h"

void __fastcall Hooks::hkSetGameMode(GameInstance* self, int gameMode, bool executeCommand) {
    // When Gamemode feature is active, block server-side resets to Adventure
    Feature* feature = FeatureHandler::GetFeatureFromName("Gamemode");
    if (feature && feature->IsActive() && gameMode == (int)GameMode::Adventure) {
        Util::log("Gamemode: blocked server reset to Adventure\n");
        return;
    }

    Hooks::oSetGameMode(self, gameMode, executeCommand);
}
