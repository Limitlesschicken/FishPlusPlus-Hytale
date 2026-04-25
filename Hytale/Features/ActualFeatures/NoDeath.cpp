#include "NoDeath.h"

void NoDeath::Initialize() {
	Util::log("Initialized NoDeath feature");
	RegisterEvent(this);
}

void NoDeath::OnPacketRecieved(Object* packet, PacketIndex& index, bool& cancel) { 
	if (index == PlayAnimation_S2C) {
		HytaleString* animName = *(HytaleString**)((uint64_t)packet + 0x10);
		if (!animName)
			return;
		uint64_t animHash = Hash::Hash(animName->getString());
		if (animHash == "Laydown"_hash || animHash == "Sleep"_hash || animHash == "Death"_hash)
			cancel = true;
	}else if (index == CustomPage_S2C) {
		HytaleString* pageName = *(HytaleString**)((uint64_t) packet + 0x8);
		if (Hash::Hash(pageName->getString()) == "com.hypixel.hytale.server.core.entity.entities.player.pages.RespawnPage"_hash)
			cancel = true;
	}
}

bool NoDeath::CanExecute() {
	return Util::isFullyInitialized();
}