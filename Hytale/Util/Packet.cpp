#include "Packet.h"
#include "core.h"

void Packets::SendPacketImmediate(void* packet) {
	using m_SendPacketImmediate = void(*)(void*, void*);
	static m_SendPacketImmediate SendPacketImmediate_method{ };
	if (!SendPacketImmediate_method)
		SendPacketImmediate_method = reinterpret_cast<m_SendPacketImmediate>(SM::SendPacketImmediateAddress);
	App* app = Util::app;
	if (!app || !app->Engine || app->Stage != AppStage::InGame || !app->appInGame->gameInstance->ConnectionToServer) {
		Util::log("Invalid app or connection pointer");
		return;
	}
	SendPacketImmediate_method(app->appInGame->gameInstance->ConnectionToServer, packet);
}