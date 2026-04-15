#include "Gamemode.h"
#include "Core.h"

Gamemode::Gamemode() : Feature("Gamemode") {
}

void Gamemode::Initialize() {
    RegisterEvent(this);
    Util::log("Initialized Gamemode feature\n");
}

// Write the gamemode byte directly — bypasses SetGameMode's side-effects
// (InGameView callbacks etc.) which crash if called during initialization.
// Safe because we only write when the value actually differs.
static void WriteGameMode(GameInstance* gi, GameMode mode) {
    gi->GameMode = mode;
}

void Gamemode::OnActivate() {
    GameInstance* gi = Util::getGameInstance();
    if (!gi) return;
    WriteGameMode(gi, GameMode::Creative);
    Util::log("Gamemode: set Creative\n");
}

void Gamemode::OnDeactivate() {
    GameInstance* gi = Util::getGameInstance();
    if (!gi) return;
    WriteGameMode(gi, GameMode::Adventure);
    Util::log("Gamemode: set Adventure\n");
}

// Re-applies on every tick in case the server overwrites the byte
void Gamemode::OnMoveCycle(DefaultMovementController* dmc, Vector3& offset) {
    GameInstance* gi = Util::getGameInstance();
    if (!gi) return;
    if (gi->GameMode != GameMode::Creative)
        WriteGameMode(gi, GameMode::Creative);
}

bool Gamemode::CanExecute() {
    return Util::isFullyInitialized();
}
