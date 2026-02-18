#pragma once

#include "Entity.h"
#include "Camera.h"
#include "Time.h"
#include "EntityStoreModule.h"
#include "InventoryModule.h"
#include "CameraModule.h"
#include "Engine.h"
#include "SceneRenderer.h"

class GameInstance
{
public:
	char pad_0000[16]; //0x0000
	Entity* Player; //0x0010
	Engine* Engine; //0x0018
	char pad_0020[80]; //0x0020
	SceneRenderer* SceneRenderer; //0x0070
	char pad_0078[136]; //0x0078
	Time* Time; //0x0100
	char pad_0108[8]; //0x0108
	StructBeforeCam* Camera; //0x0110
	char pad_0118[8]; //0x0118
	void* SomeKindOfPlayerPointer; //0x0120
	CameraModule* CameraModule; //0x0128
	char pad_0130[8]; //0x0130
	EntityStoreModule* EntityStoreModule; //0x0138
	InventoryModule* InventoryModule; //0x0140
	void* InteractionModule; //0x0148
	char pad_0150[48]; //0x0150
	void* WeatherModule; //0x0180
	void* AmbienceFXModule; //0x0188
	char pad_0190[168]; //0x0190
	float ResolutionScaleMin; //0x0238
	float ResolutionScaleMax; //0x023C
	char pad_0240[52]; //0x0240
	float underwaterCausticsIntensity; //0x0274
	float underwaterCausticsScale; //0x0278
	float underwaterCausticsDistortion; //0x027C
	float cloudsUVMotionScale; //0x0280
	float cloudsUVMotionStrength; //0x0284
	float cloudsShadowsIntensity; //0x0288
	float cloudsShadowsScale; //0x028C
	float cloudsShadowsBlurriness; //0x0290
	float cloudsShadowsSpeed; //0x0294
	float UnderwaterBloomIntensity; //0x0298
	float UnderwaterBloomPower; //0x029C
	float DefaultBloomIntensity; //0x02A0
	float DefaultBloomPower; //0x02A4
	char pad_02A8[80]; //0x02A8
	Vector3 FoliageInteractionParams; //0x02F8
	char pad_0304[128]; //0x0304

};