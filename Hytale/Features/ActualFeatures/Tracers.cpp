/*
 * Copyright (c) FishPlusPlus.
 */
#include "Tracers.h"

Tracers::Tracers() : Feature("Tracers") {
	this->players = this->RegisterSetting<ToggleSetting>("Players", false);
	this->mobs = this->RegisterSetting<ToggleSetting>("Mobs", false);
	this->color = this->RegisterSetting<ColorSetting>("Color", Color(255.0f, 0.0f, 0.0f, 255.0f));
}

Vector3 GetForward(Vector3 rotDeg)
{
	float pitch = rotDeg.x * (3.14159265f / 180.0f);
	float yaw = -rotDeg.y * (3.14159265f / 180.0f);

	Vector3 forward;
	forward.x = cos(pitch) * sin(yaw);
	forward.y = sin(pitch);
	forward.z = -cos(pitch) * cos(yaw);

	return forward;
}

void Tracers::OnRender3D(Renderer3D& renderer3D) {
	SDK::global_mutex.lock();
	std::vector<EntityData> entities = SDK::entities;
	SDK::global_mutex.unlock();


	for (const auto& entity : entities) {
		ValidPtrLoop(entity.entityPtr);
		if (entity.isLocalPlayer)
			continue;

		if (entity.entityType != Entity::EntityType::Character)
			continue;

		if (entity.player && !this->players->GetValue())
			continue;

		if (!entity.player && !this->mobs->GetValue())
			continue;

		Vector3 drawPos = entity.entityPtr->RenderPos;
		
		//Really fucking stupid but i have no better way of getting smooth camera rotation no matter if you are third person or not
		Vector3 cameraRot = Vector3(0.0f, 0.0f, 0.0f);
		if (Util::getGameInstance()->CameraModule->thirdPerson)
			cameraRot = Util::getGameInstance()->CameraModule->Controller->ThirdPersonRot.RadToDeg();
		else
			cameraRot = Vector3(Util::getLocalPlayer()->pitchRad, Util::getLocalPlayer()->yawRad, 0.0f).RadToDeg();
		Vector3 forward = GetForward(cameraRot);

		

		Vector3 lineStart = Util::getCamera()->Position + (forward * 0.1f);

		renderer3D.Line(lineStart, drawPos, Color::Normalize(this->color->GetValue()));
	}
}

bool Tracers::CanExecute() {
	return Util::app->Stage == AppStage::InGame;
}

void Tracers::Initialize() {
	Util::log("Initialized Tracers feature");
	RegisterEvent(this);
}