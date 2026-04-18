#include "NoDeath.h"

void NoDeath::Initialize() {
	Util::log("Initialized NoDeath feature\n");
	RegisterEvent(this);
}

void NoDeath::OnPacketRecieved(Object* packet, PacketIndex& index, bool& cancel) { 
	if (index == PlayAnimation_S2C) {
		HytaleString* animName = *(HytaleString**)((uint64_t)packet + 0x10);
		uint64_t animHash = Hash::Hash(animName->getString());
		constexpr uint64_t laydownHash = "Laydown"_hash;
		constexpr uint64_t sleepHash = "Sleep"_hash;
		constexpr uint64_t deathHash = "Death"_hash;
		if (animHash == laydownHash || animHash == sleepHash || animHash == deathHash)
			cancel = true;
	}else if (index == CustomPage_S2C) {
		HytaleString* pageName = *(HytaleString**)((uint64_t) packet + 0x8);
		constexpr uint64_t respawnPageHash = "com.hypixel.hytale.server.core.entity.entities.player.pages.RespawnPage"_hash;
		if (Hash::Hash(pageName->getString()) == respawnPageHash)
			cancel = true;
	}
}

bool NoDeath::CanExecute() {
	return Util::isFullyInitialized();
}