/*
 * Copyright (c) FishPlusPlus.
 */
#include "Gamemode.h"
#include "Core.h"

Gamemode::Gamemode() : Feature("Gamemode") {
}

void Gamemode::Initialize() {
    RegisterEvent(this);
    Util::log("Initialized Gamemode feature");
}

static void WriteGameMode(GameInstance* gi, GameMode mode) {
    gi->GameMode = mode;
}

void Gamemode::OnActivate() {
    GameInstance* gi = Util::getGameInstance();
    if (!gi) return;
    WriteGameMode(gi, GameMode::Creative);
    Util::log("Gamemode: set Creative");
}

void Gamemode::OnDeactivate() {
    GameInstance* gi = Util::getGameInstance();
    if (!gi) return;
    WriteGameMode(gi, GameMode::Adventure);
    Util::log("Gamemode: set Adventure");
}

void Gamemode::OnMoveCycle(DefaultMovementController* dmc, Vector3& offset) {
    GameInstance* gi = Util::getGameInstance();
    if (!gi) return;
    if (gi->GameMode != GameMode::Creative)
        WriteGameMode(gi, GameMode::Creative);
}

bool Gamemode::CanExecute() {
    return Util::isFullyInitialized();
}
