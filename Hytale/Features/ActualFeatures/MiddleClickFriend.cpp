/*
 * Copyright (c) FishPlusPlus.
 */
#include "MiddleClickFriend.h"
#include "../ConfigHandler.h"

void MiddleClickFriend::OnFrame() {
	if (!InputSystem::mousePressed[SDL_BUTTON_MIDDLE])
		return;

	GameInstance* gameInstance = Util::getGameInstance();
	InteractionModule* interactionModule = gameInstance->InteractionModule;

	

	Entity* entityLookat = interactionModule->TargetedBlockRaycastHit.TargetedEntity;
	if (!entityLookat)
		return;
	
	if (!entityLookat->IsAPlayer())
		return;
	
	if (!entityLookat->IsFriend())
		ConfigHandler::SaveFriend(entityLookat->Name->getString().c_str());
	if (entityLookat->IsFriend())
		ConfigHandler::RemoveFriend(entityLookat->Name->getString().c_str());
}

void MiddleClickFriend::Initialize() {
	Util::log("Initialized Middle Click Friend feature");
	RegisterEvent(this);
}