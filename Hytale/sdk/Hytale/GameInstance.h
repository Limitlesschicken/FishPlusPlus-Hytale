/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include "sdk/BaseDataTypes/HytaleString.h"
#include "sdk/BaseDataTypes/Array.h"
#include "sdk/BaseDataTypes/Dictionary.h"
#include "sdk/BaseDataTypes/ConcurrentDictionary.h"
#include "Entity.h"
#include "Camera.h"
#include "Time.h"
#include "EntityStoreModule.h"
#include "InventoryModule.h"
#include "CameraModule.h"
#include "Engine.h"
#include "SceneRenderer.h"
#include "Chat.h"
#include "CharacterControllerModule.h"
#include "MapModule.h"
#include "InteractionModule.h"
#include "ItemLibraryModule.h"

enum class GameMode : uint8_t {
	Adventure = 0,
	Creative  = 1,
};

class GameInstance {
public:
	char pad_0[0x0A];                                   //0x0
	GameMode GameMode;                                  //0xA  low byte of <GameMode>k__BackingField (0=Adventure, 1=Creative)
	char pad_0B[0x05];                                  //0xB
	Entity* Player;                                     //0x10
	void* App;                                          //0x18 HytaleClient.Application.App
	Engine* Engine;                                     //0x20 HytaleClient.Core.Engine
	Chat* Chat;                                         //0x28 HytaleClient.InGame.Chat
	void* Notifications;                                //0x30 HytaleClient.InGame.Notifications
	void* HitDetection;                                 //0x38 HytaleClient.InGame.HitDetection
	void* VoiceManager;                                 //0x40 HytaleClient.Audio.Voice.VoiceManager
	void* SocialScope;									//0x48 System.Action`1[HytaleClient.Social.SocialScope]
	void* ServerSettings;								//0x50 HytaleClient.Data.ServerSettings
	void* ConnectionToServer;							//0x58 HytaleClient.Networking.Connection.ConnectionToServer
	void* PacketHandler;                                //0x60 HytaleClient.Networking.PacketHandler
	void* Stopwatch;                                    //0x68 System.Diagnostics.Stopwatch
	void* List;                                         //0x70 System.Collections.Generic.List`1[System.ValueTuple`2[HytaleClient.Data.InputSystem.InputAction,System.Boolean]]
	void* some_String;                                  //0x78 System.String
	SceneRenderer* SceneRenderer;                       //0x80 HytaleClient.Graphics.SceneRenderer
	char pad_80[0x88];                                  //0x88 
	Time* TimeModule;                                   //0x110 HytaleClient.InGame.Modules.TimeModule
	void* AudioModule;                                  //0x118 HytaleClient.InGame.Modules.Audio.AudioModule
	MapModule* MapModule;								//0x120 HytaleClient.InGame.Modules.Map.MapModule
	ItemLibraryModule* ItemLibraryModule;               //0x128 HytaleClient.InGame.Modules.ItemLibraryModule
	CharacterControllerModule* CharacterControllerModule; //0x130 HytaleClient.InGame.Modules.CharacterController.CharacterControllerModule
	CameraModule* CameraModule;                         //0x138 HytaleClient.InGame.Modules.Camera.CameraModule
	void* CollisionModule;                              //0x140 HytaleClient.InGame.Modules.Collision.CollisionModule
	EntityStoreModule* EntityStoreModule;               //0x148 HytaleClient.InGame.Modules.Entities.EntityStoreModule
	InventoryModule* InventoryModule;                   //0x150 HytaleClient.InGame.Modules.InventoryModule
	InteractionModule* InteractionModule;               //0x158 HytaleClient.InGame.Modules.Interaction.InteractionModule
	void* BuilderToolsModule;                           //0x160 HytaleClient.InGame.Modules.BuilderTools.BuilderToolsModule
	void* MachinimaModule;                              //0x168 HytaleClient.InGame.Modules.Machinima.MachinimaModule
	void* FXModule;                                     //0x170 HytaleClient.InGame.Modules.FXModule
	void* TrailStoreModule;                             //0x178 HytaleClient.InGame.Modules.Trails.TrailStoreModule
	void* ParticleSystemStoreModule;                    //0x180 HytaleClient.InGame.Modules.Particles.ParticleSystemStoreModule
	void* ScreenEffectStoreModule;                      //0x188 HytaleClient.InGame.Modules.ScreenEffectStoreModule
	void* WeatherModule;                                //0x190 HytaleClient.InGame.Modules.WeatherModule
	void* AmbienceFXModule;                             //0x198 HytaleClient.InGame.Modules.AmbienceFX.AmbienceFXModule
	void* DamageEffectModule;                           //0x1A0 HytaleClient.InGame.Modules.DamageEffectModule
	void* SleepModule;                                  //0x1A8 HytaleClient.InGame.Modules.Entities.SleepModule
	void* ClientFeatureModule;                          //0x1B0 HytaleClient.InGame.Modules.ClientFeatureModule
	void* DebugStatusBarModule;                         //0x1B8 HytaleClient.InGame.Modules.DebugStatusBarModule
	void* ProfilingModule;                              //0x1C0 HytaleClient.InGame.Modules.Profiling.ProfilingModule
	void* FpsOverlayModule;                             //0x1C8 HytaleClient.InGame.Modules.Fps.FpsOverlayModule
	void* AudioDebugModule;                             //0x1D0 HytaleClient.Audio.Debugger.AudioDebugModule
	void* ShortcutsModule;                              //0x1D8 HytaleClient.InGame.Modules.Shortcuts.ShortcutsModule
	void* InterfaceRenderPreviewModule;                 //0x1E0 HytaleClient.InGame.Modules.InterfaceRenderPreview.InterfaceRenderPreviewModul
	void* WorldMapModule;                               //0x1E8 HytaleClient.InGame.Modules.WorldMap.WorldMapModule
	void* DebugDisplayModule;                           //0x1F0 HytaleClient.InGame.Modules.DebugDisplayModule
	void* NetworkModule;                                //0x1F8 HytaleClient.InGame.Modules.NetworkModule
	void* MovementSoundModule;                          //0x200 HytaleClient.InGame.Modules.MovementSoundModule
	void* AutoCameraModule;                             //0x208 HytaleClient.InGame.Modules.AutoCameraModule
	void* DebugCommandsModule;                          //0x210 HytaleClient.InGame.Modules.DebugCommandsModule
	void* ConcurrentDictionary;                         //0x218 System.Collections.Concurrent.ConcurrentDictionary`2[System.String,System.String]
	char pad_218[0x28];                                 //0x220
	float ResolutionScaleMin;                           //0x248
	float ResolutionScaleMax;                           //0x24C
	char pad_248[0x34];                                 //0x250
	float underwaterCausticsIntensity;                  //0x27C
	float underwaterCausticsScale;                      //0x280
	float underwaterCausticsDistortion;                 //0x284
	float cloudsUVMotionScale;                          //0x288
	float cloudsUVMotionStrength;                       //0x28C
	float cloudsShadowsIntensity;                       //0x290
	float cloudsShadowsScale;                           //0x294
	float cloudsShadowsBlurriness;                      //0x298
	float cloudsShadowsSpeed;                           //0x29C
	float UnderwaterBloomIntensity;                     //0x2A0
	float UnderwaterBloomPower;                         //0x2A4
	float DefaultBloomIntensity;                        //0x2A8
	float DefaultBloomPower;                            //0x2AC
	char pad_2A8[0x50];                                 //0x2B0
	Vector3 FoliageInteractionParams;                   //0x300
	char pad_304[0x80];                                 //0x30C
};