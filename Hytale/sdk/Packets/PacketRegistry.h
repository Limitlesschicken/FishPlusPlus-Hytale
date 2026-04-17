/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include <cstdint>
#include <algorithm>

#include "sdk/BaseDataTypes/Object.h"

// Auto-generated packet definitions from dumper
// Total packets found: 279

enum class PacketDirection : int32_t {
	ServerBound = 0,
	ClientBound = 1,
	Bidirectional = 2
};

struct PacketEntry {
	uint64_t methodTableOffset;
	PacketDirection direction;
};

enum PacketIndex : size_t {
	Unkown = -1,
	ClientDisconnect_C2S = 0,                                  // ID: 1 [Server-Bound]
	ServerDisconnect_S2C = 1,                                  // ID: 2 [Client-Bound]
	Ping_S2C = 2,                                              // ID: 3 [Client-Bound]
	Pong_C2S = 3,                                              // ID: 4 [Server-Bound]
	AuthGrant_S2C = 4,                                         // ID: 11 [Client-Bound]
	AuthToken_C2S = 5,                                         // ID: 12 [Server-Bound]
	ServerAuthToken_S2C = 6,                                   // ID: 13 [Client-Bound]
	ConnectAccept_S2C = 7,                                     // ID: 14 [Client-Bound]
	PasswordResponse_C2S = 8,                                  // ID: 15 [Server-Bound]
	PasswordAccepted_S2C = 9,                                  // ID: 16 [Client-Bound]
	PasswordRejected_S2C = 10,                                 // ID: 17 [Client-Bound]
	ClientReferral_S2C = 11,                                   // ID: 18 [Client-Bound]
	WorldSettings_S2C = 12,                                    // ID: 20 [Client-Bound]
	WorldLoadProgress_S2C = 13,                                // ID: 21 [Client-Bound]
	WorldLoadFinished_S2C = 14,                                // ID: 22 [Client-Bound]
	RequestAssets_C2S = 15,                                    // ID: 23 [Server-Bound]
	AssetInitialize_S2C = 16,                                  // ID: 24 [Client-Bound]
	AssetPart_S2C = 17,                                        // ID: 25 [Client-Bound]
	AssetFinalize_S2C = 18,                                    // ID: 26 [Client-Bound]
	RemoveAssets_S2C = 19,                                     // ID: 27 [Client-Bound]
	RequestCommonAssetsRebuild_S2C = 20,                       // ID: 28 [Client-Bound]
	SetUpdateRate_S2C = 21,                                    // ID: 29 [Client-Bound]
	SetTimeDilation_S2C = 22,                                  // ID: 30 [Client-Bound]
	UpdateFeatures_S2C = 23,                                   // ID: 31 [Client-Bound]
	ViewRadius_BI = 24,                                        // ID: 32 [Bidirectional]
	PlayerOptions_C2S = 25,                                    // ID: 33 [Server-Bound]
	ServerTags_S2C = 26,                                       // ID: 34 [Client-Bound]
	UpdateBlockTypes_S2C = 27,                                 // ID: 40 [Client-Bound]
	UpdateBlockHitboxes_S2C = 28,                              // ID: 41 [Client-Bound]
	UpdateBlockSoundSets_S2C = 29,                             // ID: 42 [Client-Bound]
	UpdateItemSoundSets_S2C = 30,                              // ID: 43 [Client-Bound]
	UpdateBlockParticleSets_S2C = 31,                          // ID: 44 [Client-Bound]
	UpdateBlockBreakingDecals_S2C = 32,                        // ID: 45 [Client-Bound]
	UpdateBlockSets_S2C = 33,                                  // ID: 46 [Client-Bound]
	UpdateWeathers_S2C = 34,                                   // ID: 47 [Client-Bound]
	UpdateTrails_S2C = 35,                                     // ID: 48 [Client-Bound]
	UpdateParticleSystems_S2C = 36,                            // ID: 49 [Client-Bound]
	UpdateParticleSpawners_S2C = 37,                           // ID: 50 [Client-Bound]
	UpdateEntityEffects_S2C = 38,                              // ID: 51 [Client-Bound]
	UpdateItemPlayerAnimations_S2C = 39,                       // ID: 52 [Client-Bound]
	UpdateModelvfxs_S2C = 40,                                  // ID: 53 [Client-Bound]
	UpdateItems_S2C = 41,                                      // ID: 54 [Client-Bound]
	UpdateItemQualities_S2C = 42,                              // ID: 55 [Client-Bound]
	UpdateItemCategories_S2C = 43,                             // ID: 56 [Client-Bound]
	UpdateItemReticles_S2C = 44,                               // ID: 57 [Client-Bound]
	UpdateFieldcraftCategories_S2C = 45,                       // ID: 58 [Client-Bound]
	UpdateResourceTypes_S2C = 46,                              // ID: 59 [Client-Bound]
	UpdateRecipes_S2C = 47,                                    // ID: 60 [Client-Bound]
	UpdateEnvironments_S2C = 48,                               // ID: 61 [Client-Bound]
	UpdateAmbienceFX_S2C = 49,                                 // ID: 62 [Client-Bound]
	UpdateFluidFX_S2C = 50,                                    // ID: 63 [Client-Bound]
	UpdateTranslations_S2C = 51,                               // ID: 64 [Client-Bound]
	UpdateSoundEvents_S2C = 52,                                // ID: 65 [Client-Bound]
	UpdateInteractions_S2C = 53,                               // ID: 66 [Client-Bound]
	UpdateRootInteractions_S2C = 54,                           // ID: 67 [Client-Bound]
	UpdateUnarmedInteractions_S2C = 55,                        // ID: 68 [Client-Bound]
	TrackOrUpdateObjective_S2C = 56,                           // ID: 69 [Client-Bound]
	UntrackObjective_S2C = 57,                                 // ID: 70 [Client-Bound]
	UpdateObjectiveTask_S2C = 58,                              // ID: 71 [Client-Bound]
	UpdateEntityStatTypes_S2C = 59,                            // ID: 72 [Client-Bound]
	UpdateEntityUIComponents_S2C = 60,                         // ID: 73 [Client-Bound]
	UpdateHitboxCollisionConfig_S2C = 61,                      // ID: 74 [Client-Bound]
	UpdateRepulsionConfig_S2C = 62,                            // ID: 75 [Client-Bound]
	UpdateViewBobbing_S2C = 63,                                // ID: 76 [Client-Bound]
	UpdateCameraShake_S2C = 64,                                // ID: 77 [Client-Bound]
	UpdateBlockGroups_S2C = 65,                                // ID: 78 [Client-Bound]
	UpdateSoundSets_S2C = 66,                                  // ID: 79 [Client-Bound]
	UpdateAudioCategories_S2C = 67,                            // ID: 80 [Client-Bound]
	UpdateReverbEffects_S2C = 68,                              // ID: 81 [Client-Bound]
	UpdateEqualizerEffects_S2C = 69,                           // ID: 82 [Client-Bound]
	UpdateFluids_S2C = 70,                                     // ID: 83 [Client-Bound]
	UpdateTagPatterns_S2C = 71,                                // ID: 84 [Client-Bound]
	UpdateProjectileConfigs_S2C = 72,                          // ID: 85 [Client-Bound]
	UpdateEmotes_S2C = 73,                                     // ID: 86 [Client-Bound]
	SetClientId_S2C = 74,                                      // ID: 100 [Client-Bound]
	SetGameMode_S2C = 75,                                      // ID: 101 [Client-Bound]
	SetMovementStates_S2C = 76,                                // ID: 102 [Client-Bound]
	SetBlockPlacementOverride_S2C = 77,                        // ID: 103 [Client-Bound]
	JoinWorld_S2C = 78,                                        // ID: 104 [Client-Bound]
	ClientReady_C2S = 79,                                      // ID: 105 [Server-Bound]
	LoadHotbar_C2S = 80,                                       // ID: 106 [Server-Bound]
	SaveHotbar_C2S = 81,                                       // ID: 107 [Server-Bound]
	ClientMovement_C2S = 82,                                   // ID: 108 [Server-Bound]
	ClientTeleport_S2C = 83,                                   // ID: 109 [Client-Bound]
	UpdateMovementSettings_S2C = 84,                           // ID: 110 [Client-Bound]
	MouseInteraction_C2S = 85,                                 // ID: 111 [Server-Bound]
	DamageInfo_S2C = 86,                                       // ID: 112 [Client-Bound]
	ReticleEvent_S2C = 87,                                     // ID: 113 [Client-Bound]
	DisplayDebug_S2C = 88,                                     // ID: 114 [Client-Bound]
	ClearDebugShapes_S2C = 89,                                 // ID: 115 [Client-Bound]
	SyncPlayerPreferences_C2S = 90,                            // ID: 116 [Server-Bound]
	ClientPlaceBlock_C2S = 91,                                 // ID: 117 [Server-Bound]
	UpdateMemoriesFeatureStatus_S2C = 92,                      // ID: 118 [Client-Bound]
	RemoveMapMarker_C2S = 93,                                  // ID: 119 [Server-Bound]
	SetChunk_S2C = 94,                                         // ID: 131 [Client-Bound]
	SetChunkHeightmap_S2C = 95,                                // ID: 132 [Client-Bound]
	SetChunkTintmap_S2C = 96,                                  // ID: 133 [Client-Bound]
	SetChunkEnvironments_S2C = 97,                             // ID: 134 [Client-Bound]
	UnloadChunk_S2C = 98,                                      // ID: 135 [Client-Bound]
	SetFluids_S2C = 99,                                        // ID: 136 [Client-Bound]
	ServerSetBlock_S2C = 100,                                  // ID: 140 [Client-Bound]
	ServerSetBlocks_S2C = 101,                                 // ID: 141 [Client-Bound]
	ServerSetFluid_S2C = 102,                                  // ID: 142 [Client-Bound]
	ServerSetFluids_S2C = 103,                                 // ID: 143 [Client-Bound]
	UpdateBlockDamage_S2C = 104,                               // ID: 144 [Client-Bound]
	UpdateTimeSettings_S2C = 105,                              // ID: 145 [Client-Bound]
	UpdateTime_S2C = 106,                                      // ID: 146 [Client-Bound]
	UpdateEditorTimeOverride_S2C = 107,                        // ID: 147 [Client-Bound]
	ClearEditorTimeOverride_S2C = 108,                         // ID: 148 [Client-Bound]
	UpdateWeather_S2C = 109,                                   // ID: 149 [Client-Bound]
	UpdateEditorWeatherOverride_S2C = 110,                     // ID: 150 [Client-Bound]
	UpdateEnvironmentMusic_S2C = 111,                          // ID: 151 [Client-Bound]
	SpawnParticleSystem_S2C = 112,                             // ID: 152 [Client-Bound]
	SpawnBlockParticleSystem_S2C = 113,                        // ID: 153 [Client-Bound]
	PlaySoundEvent2D_S2C = 114,                                // ID: 154 [Client-Bound]
	PlaySoundEvent3D_S2C = 115,                                // ID: 155 [Client-Bound]
	PlaySoundEventEntity_S2C = 116,                            // ID: 156 [Client-Bound]
	UpdateSleepState_S2C = 117,                                // ID: 157 [Client-Bound]
	SetPaused_C2S = 118,                                       // ID: 158 [Server-Bound]
	ServerSetPaused_S2C = 119,                                 // ID: 159 [Client-Bound]
	SetEntitySeed_S2C = 120,                                   // ID: 160 [Client-Bound]
	EntityUpdates_S2C = 121,                                   // ID: 161 [Client-Bound]
	PlayAnimation_S2C = 122,                                   // ID: 162 [Client-Bound]
	ChangeVelocity_S2C = 123,                                  // ID: 163 [Client-Bound]
	ApplyKnockback_S2C = 124,                                  // ID: 164 [Client-Bound]
	SpawnModelParticles_S2C = 125,                             // ID: 165 [Client-Bound]
	MountMovement_C2S = 126,                                   // ID: 166 [Server-Bound]
	PlayEmote_C2S = 127,                                       // ID: 167 [Server-Bound]
	UpdatePlayerInventory_S2C = 128,                           // ID: 170 [Client-Bound]
	SetCreativeItem_C2S = 129,                                 // ID: 171 [Server-Bound]
	DropCreativeItem_C2S = 130,                                // ID: 172 [Server-Bound]
	SmartGiveCreativeItem_C2S = 131,                           // ID: 173 [Server-Bound]
	DropItemStack_C2S = 132,                                   // ID: 174 [Server-Bound]
	MoveItemStack_C2S = 133,                                   // ID: 175 [Server-Bound]
	SmartMoveItemStack_BI = 134,                               // ID: 176 [Bidirectional]
	SetActiveSlot_BI = 135,                                    // ID: 177 [Bidirectional]
	SwitchHotbarBlockSet_C2S = 136,                            // ID: 178 [Server-Bound]
	InventoryAction_C2S = 137,                                 // ID: 179 [Server-Bound]
	OpenWindow_S2C = 138,                                      // ID: 200 [Client-Bound]
	UpdateWindow_S2C = 139,                                    // ID: 201 [Client-Bound]
	CloseWindow_BI = 140,                                      // ID: 202 [Bidirectional]
	SendWindowAction_C2S = 141,                                // ID: 203 [Server-Bound]
	ClientOpenWindow_C2S = 142,                                // ID: 204 [Server-Bound]
	ServerMessage_S2C = 143,                                   // ID: 210 [Client-Bound]
	ChatMessage_C2S = 144,                                     // ID: 211 [Server-Bound]
	Notification_S2C = 145,                                    // ID: 212 [Client-Bound]
	KillFeedMessage_S2C = 146,                                 // ID: 213 [Client-Bound]
	ShowEventTitle_S2C = 147,                                  // ID: 214 [Client-Bound]
	HideEventTitle_S2C = 148,                                  // ID: 215 [Client-Bound]
	SetPage_S2C = 149,                                         // ID: 216 [Client-Bound]
	CustomHud_S2C = 150,                                       // ID: 217 [Client-Bound]
	CustomPage_S2C = 151,                                      // ID: 218 [Client-Bound]
	CustomPageEvent_C2S = 152,                                 // ID: 219 [Server-Bound]
	EditorBlocksChange_S2C = 153,                              // ID: 222 [Client-Bound]
	ServerInfo_S2C = 154,                                      // ID: 223 [Client-Bound]
	AddToServerPlayerList_S2C = 155,                           // ID: 224 [Client-Bound]
	RemoveFromServerPlayerList_S2C = 156,                      // ID: 225 [Client-Bound]
	UpdateServerPlayerList_S2C = 157,                          // ID: 226 [Client-Bound]
	UpdateServerPlayerListPing_S2C = 158,                      // ID: 227 [Client-Bound]
	UpdateKnownRecipes_S2C = 159,                              // ID: 228 [Client-Bound]
	UpdatePortal_S2C = 160,                                    // ID: 229 [Client-Bound]
	UpdateVisibleHudComponents_S2C = 161,                      // ID: 230 [Client-Bound]
	ResetUserInterfaceState_S2C = 162,                         // ID: 231 [Client-Bound]
	UpdateLanguage_C2S = 163,                                  // ID: 232 [Server-Bound]
	WorldSavingStatus_S2C = 164,                               // ID: 233 [Client-Bound]
	OpenChatWithCommand_S2C = 165,                             // ID: 234 [Client-Bound]
	UpdateAnchorUI_S2C = 166,                                  // ID: 235 [Client-Bound]
	UpdateWorldMapSettings_S2C = 167,                          // ID: 240 [Client-Bound]
	UpdateWorldMap_S2C = 168,                                  // ID: 241 [Client-Bound]
	ClearWorldMap_S2C = 169,                                   // ID: 242 [Client-Bound]
	UpdateWorldMapVisible_C2S = 170,                           // ID: 243 [Server-Bound]
	TeleportToWorldMapMarker_C2S = 171,                        // ID: 244 [Server-Bound]
	TeleportToWorldMapPosition_C2S = 172,                      // ID: 245 [Server-Bound]
	CreateUserMarker_C2S = 173,                                // ID: 246 [Server-Bound]
	RequestServerAccess_S2C = 174,                             // ID: 250 [Client-Bound]
	UpdateServerAccess_C2S = 175,                              // ID: 251 [Server-Bound]
	SetServerAccess_C2S = 176,                                 // ID: 252 [Server-Bound]
	RequestMachinimaActorModel_C2S = 177,                      // ID: 260 [Server-Bound]
	SetMachinimaActorModel_S2C = 178,                          // ID: 261 [Client-Bound]
	UpdateMachinimaScene_BI = 179,                             // ID: 262 [Bidirectional]
	SetServerCamera_S2C = 180,                                 // ID: 280 [Client-Bound]
	CameraShakeEffect_S2C = 181,                               // ID: 281 [Client-Bound]
	RequestFlyCameraMode_C2S = 182,                            // ID: 282 [Server-Bound]
	SetFlyCameraMode_S2C = 183,                                // ID: 283 [Client-Bound]
	SyncInteractionChains_BI = 184,                            // ID: 290 [Bidirectional]
	CancelInteractionChain_S2C = 185,                          // ID: 291 [Client-Bound]
	PlayInteractionFor_S2C = 186,                              // ID: 292 [Client-Bound]
	MountNPC_S2C = 187,                                        // ID: 293 [Client-Bound]
	DismountNPC_BI = 188,                                      // ID: 294 [Bidirectional]
	FailureReply_BI = 189,                                     // ID: 300 [Bidirectional]
	SuccessReply_BI = 190,                                     // ID: 301 [Bidirectional]
	AssetEditorInitialize_C2S = 191,                           // ID: 302 [Server-Bound]
	AssetEditorAuthorization_S2C = 192,                        // ID: 303 [Client-Bound]
	AssetEditorCapabilities_S2C = 193,                         // ID: 304 [Client-Bound]
	AssetEditorSetupSchemas_S2C = 194,                         // ID: 305 [Client-Bound]
	AssetEditorSetupAssetTypes_S2C = 195,                      // ID: 306 [Client-Bound]
	AssetEditorCreateDirectory_C2S = 196,                      // ID: 307 [Server-Bound]
	AssetEditorDeleteDirectory_C2S = 197,                      // ID: 308 [Server-Bound]
	AssetEditorRenameDirectory_C2S = 198,                      // ID: 309 [Server-Bound]
	AssetEditorFetchAsset_C2S = 199,                           // ID: 310 [Server-Bound]
	AssetEditorFetchJsonAssetWithParents_C2S = 200,            // ID: 311 [Server-Bound]
	AssetEditorFetchAssetReply_S2C = 201,                      // ID: 312 [Client-Bound]
	AssetEditorFetchJsonAssetWithParentsReply_S2C = 202,       // ID: 313 [Client-Bound]
	AssetEditorAssetPackSetup_S2C = 203,                       // ID: 314 [Client-Bound]
	AssetEditorUpdateAssetPack_BI = 204,                       // ID: 315 [Bidirectional]
	AssetEditorCreateAssetPack_C2S = 205,                      // ID: 316 [Server-Bound]
	AssetEditorDeleteAssetPack_BI = 206,                       // ID: 317 [Bidirectional]
	AssetEditorAssetListSetup_S2C = 207,                       // ID: 319 [Client-Bound]
	AssetEditorAssetListUpdate_S2C = 208,                      // ID: 320 [Client-Bound]
	AssetEditorRequestChildrenList_C2S = 209,                  // ID: 321 [Server-Bound]
	AssetEditorRequestChildrenListReply_S2C = 210,             // ID: 322 [Client-Bound]
	AssetEditorUpdateJsonAsset_C2S = 211,                      // ID: 323 [Server-Bound]
	AssetEditorUpdateAsset_C2S = 212,                          // ID: 324 [Server-Bound]
	AssetEditorJsonAssetUpdated_S2C = 213,                     // ID: 325 [Client-Bound]
	AssetEditorAssetUpdated_S2C = 214,                         // ID: 326 [Client-Bound]
	AssetEditorCreateAsset_C2S = 215,                          // ID: 327 [Server-Bound]
	AssetEditorRenameAsset_C2S = 216,                          // ID: 328 [Server-Bound]
	AssetEditorDeleteAsset_C2S = 217,                          // ID: 329 [Server-Bound]
	AssetEditorDiscardChanges_C2S = 218,                       // ID: 330 [Server-Bound]
	AssetEditorFetchAutoCompleteData_C2S = 219,                // ID: 331 [Server-Bound]
	AssetEditorFetchAutoCompleteDataReply_S2C = 220,           // ID: 332 [Client-Bound]
	AssetEditorRequestDataset_C2S = 221,                       // ID: 333 [Server-Bound]
	AssetEditorRequestDatasetReply_S2C = 222,                  // ID: 334 [Client-Bound]
	AssetEditorActivateButton_C2S = 223,                       // ID: 335 [Server-Bound]
	AssetEditorSelectAsset_C2S = 224,                          // ID: 336 [Server-Bound]
	AssetEditorPopupNotification_S2C = 225,                    // ID: 337 [Client-Bound]
	AssetEditorFetchLastModifiedAssets_C2S = 226,              // ID: 338 [Server-Bound]
	AssetEditorLastModifiedAssets_S2C = 227,                   // ID: 339 [Client-Bound]
	AssetEditorModifiedAssetsCount_S2C = 228,                  // ID: 340 [Client-Bound]
	AssetEditorSubscribeModifiedAssetsChanges_C2S = 229,       // ID: 341 [Server-Bound]
	AssetEditorExportAssets_C2S = 230,                         // ID: 342 [Server-Bound]
	AssetEditorExportAssetInitialize_S2C = 231,                // ID: 343 [Client-Bound]
	AssetEditorExportAssetPart_S2C = 232,                      // ID: 344 [Client-Bound]
	AssetEditorExportAssetFinalize_S2C = 233,                  // ID: 345 [Client-Bound]
	AssetEditorExportDeleteAssets_S2C = 234,                   // ID: 346 [Client-Bound]
	AssetEditorExportComplete_S2C = 235,                       // ID: 347 [Client-Bound]
	AssetEditorUndoChanges_C2S = 236,                          // ID: 349 [Server-Bound]
	AssetEditorRedoChanges_C2S = 237,                          // ID: 350 [Server-Bound]
	AssetEditorUndoRedoReply_S2C = 238,                        // ID: 351 [Client-Bound]
	AssetEditorSetGameTime_C2S = 239,                          // ID: 352 [Server-Bound]
	AssetEditorUpdateSecondsPerGameDay_S2C = 240,              // ID: 353 [Client-Bound]
	AssetEditorUpdateWeatherPreviewLock_C2S = 241,             // ID: 354 [Server-Bound]
	AssetEditorUpdateModelPreview_S2C = 242,                   // ID: 355 [Client-Bound]
	AssetEditorModsDirectories_S2C = 243,                      // ID: 356 [Client-Bound]
	UpdateSunSettings_S2C = 244,                               // ID: 360 [Client-Bound]
	UpdatePostFxSettings_S2C = 245,                            // ID: 361 [Client-Bound]
	PlaySoundEventLocalPlayer_S2C = 246,                       // ID: 362 [Client-Bound]
	BuilderToolArgUpdate_C2S = 247,                            // ID: 400 [Server-Bound]
	BuilderToolEntityAction_C2S = 248,                         // ID: 401 [Server-Bound]
	BuilderToolSetEntityTransform_C2S = 249,                   // ID: 402 [Server-Bound]
	BuilderToolExtrudeAction_C2S = 250,                        // ID: 403 [Server-Bound]
	BuilderToolStackArea_C2S = 251,                            // ID: 404 [Server-Bound]
	BuilderToolSelectionTransform_C2S = 252,                   // ID: 405 [Server-Bound]
	BuilderToolRotateClipboard_C2S = 253,                      // ID: 406 [Server-Bound]
	BuilderToolPasteClipboard_C2S = 254,                       // ID: 407 [Server-Bound]
	BuilderToolSetTransformationModeState_C2S = 255,           // ID: 408 [Server-Bound]
	BuilderToolSelectionUpdate_C2S = 256,                      // ID: 409 [Server-Bound]
	BuilderToolSelectionToolAskForClipboard_C2S = 257,         // ID: 410 [Server-Bound]
	BuilderToolSelectionToolReplyWithClipboard_S2C = 258,      // ID: 411 [Client-Bound]
	BuilderToolGeneralAction_C2S = 259,                        // ID: 412 [Server-Bound]
	BuilderToolOnUseInteraction_C2S = 260,                     // ID: 413 [Server-Bound]
	BuilderToolLineAction_C2S = 261,                           // ID: 414 [Server-Bound]
	BuilderToolShowAnchor_S2C = 262,                           // ID: 415 [Client-Bound]
	BuilderToolHideAnchors_S2C = 263,                          // ID: 416 [Client-Bound]
	PrefabUnselectPrefab_C2S = 264,                            // ID: 417 [Server-Bound]
	BuilderToolsSetSoundSet_S2C = 265,                         // ID: 418 [Client-Bound]
	BuilderToolLaserPointer_S2C = 266,                         // ID: 419 [Client-Bound] 
	BuilderToolSetEntityScale_C2S = 267,                       // ID: 420 [Server-Bound]
	BuilderToolSetEntityPickupEnabled_C2S = 268,               // ID: 421 [Server-Bound]
	BuilderToolSetEntityLight_C2S = 269,                       // ID: 422 [Server-Bound]
	BuilderToolSetNPCDebug_C2S = 270,                          // ID: 423 [Server-Bound]
	BuilderToolSetEntityCollision_C2S = 271,                   // ID: 425 [Server-Bound]
	PrefabSetAnchor_C2S = 272,                                 // ID: 426 [Server-Bound]
	BuilderToolResetClipboardRotation_C2S = 273,               // ID: 427 [Server-Bound]
	VoiceData_C2S = 274,                                       // ID: 450 [Server-Bound]
	RelayedVoiceData_S2C = 275,                                // ID: 451 [Client-Bound]
	VoiceConfig_S2C = 276,                                     // ID: 452 [Client-Bound]
	StreamOpen_C2S = 277,                                      // ID: 460 [Server-Bound]
	StreamOpenResponse_S2C = 278                               // ID: 461 [Client-Bound]
};

// Packet registry (method table offset from game base + direction)
constexpr PacketEntry PACKET_REGISTRY[] = {
	{ 0x1B1F390, PacketDirection::ServerBound },
	{ 0x1B1F410, PacketDirection::ClientBound },
	{ 0x1B1F490, PacketDirection::ClientBound },
	{ 0x1B1F510, PacketDirection::ServerBound },
	{ 0x1B20B60, PacketDirection::ClientBound },
	{ 0x1B20BE0, PacketDirection::ServerBound },
	{ 0x1B20C60, PacketDirection::ClientBound },
	{ 0x1B20CE0, PacketDirection::ClientBound },
	{ 0x1B20D60, PacketDirection::ServerBound },
	{ 0x1B20DC8, PacketDirection::ClientBound },
	{ 0x1B20E48, PacketDirection::ClientBound },
	{ 0x1B20EC8, PacketDirection::ClientBound },
	{ 0x1B1BFE0, PacketDirection::ClientBound },
	{ 0x1B1C060, PacketDirection::ClientBound },
	{ 0x1B1C0C8, PacketDirection::ClientBound },
	{ 0x1B1C148, PacketDirection::ServerBound },
	{ 0x1B1C1C8, PacketDirection::ClientBound },
	{ 0x1B1C248, PacketDirection::ClientBound },
	{ 0x1B1C2B0, PacketDirection::ClientBound },
	{ 0x1B1C330, PacketDirection::ClientBound },
	{ 0x1B1C398, PacketDirection::ClientBound },
	{ 0x1B1C400, PacketDirection::ClientBound },
	{ 0x1B1C468, PacketDirection::ClientBound },
	{ 0x1B1C4E8, PacketDirection::ClientBound },
	{ 0x1B1C550, PacketDirection::Bidirectional },
	{ 0x1B1C5D8, PacketDirection::ServerBound },
	{ 0x1B1C658, PacketDirection::ClientBound },
	{ 0x1B20F48, PacketDirection::ClientBound },
	{ 0x1B20FC8, PacketDirection::ClientBound },
	{ 0x1B21048, PacketDirection::ClientBound },
	{ 0x1B210C8, PacketDirection::ClientBound },
	{ 0x1B21148, PacketDirection::ClientBound },
	{ 0x1B211C8, PacketDirection::ClientBound },
	{ 0x1B21248, PacketDirection::ClientBound },
	{ 0x1B212C8, PacketDirection::ClientBound },
	{ 0x1B21348, PacketDirection::ClientBound },
	{ 0x1B213C8, PacketDirection::ClientBound },
	{ 0x1B21448, PacketDirection::ClientBound },
	{ 0x1B214C8, PacketDirection::ClientBound },
	{ 0x1B21548, PacketDirection::ClientBound },
	{ 0x1B215C8, PacketDirection::ClientBound },
	{ 0x1B21648, PacketDirection::ClientBound },
	{ 0x1B216C8, PacketDirection::ClientBound },
	{ 0x1B21748, PacketDirection::ClientBound },
	{ 0x1B217C8, PacketDirection::ClientBound },
	{ 0x1B21848, PacketDirection::ClientBound },
	{ 0x1B218C8, PacketDirection::ClientBound },
	{ 0x1B21948, PacketDirection::ClientBound },
	{ 0x1B219C8, PacketDirection::ClientBound },
	{ 0x1B21A48, PacketDirection::ClientBound },
	{ 0x1B21AC8, PacketDirection::ClientBound },
	{ 0x1B21B48, PacketDirection::ClientBound },
	{ 0x1B21BC8, PacketDirection::ClientBound },
	{ 0x1B21C48, PacketDirection::ClientBound },
	{ 0x1B21CC8, PacketDirection::ClientBound },
	{ 0x1B21D48, PacketDirection::ClientBound },
	{ 0x1B21DC8, PacketDirection::ClientBound },
	{ 0x1B21E30, PacketDirection::ClientBound },
	{ 0x1B21EB0, PacketDirection::ClientBound },
	{ 0x1B21F30, PacketDirection::ClientBound },
	{ 0x1B21FB0, PacketDirection::ClientBound },
	{ 0x1B22030, PacketDirection::ClientBound },
	{ 0x1B220B0, PacketDirection::ClientBound },
	{ 0x1B22130, PacketDirection::ClientBound },
	{ 0x1B221B0, PacketDirection::ClientBound },
	{ 0x1B22230, PacketDirection::ClientBound },
	{ 0x1B222B0, PacketDirection::ClientBound },
	{ 0x1B22330, PacketDirection::ClientBound },
	{ 0x1B223B0, PacketDirection::ClientBound },
	{ 0x1B22430, PacketDirection::ClientBound },
	{ 0x1B224B0, PacketDirection::ClientBound },
	{ 0x1B22530, PacketDirection::ClientBound },
	{ 0x1B225B0, PacketDirection::ClientBound },
	{ 0x1B22630, PacketDirection::ClientBound },
	{ 0x1B1C888, PacketDirection::ClientBound },
	{ 0x1B1C8F0, PacketDirection::ClientBound },
	{ 0x1B1C970, PacketDirection::ClientBound },
	{ 0x1B1C9D8, PacketDirection::ClientBound },
	{ 0x1B1CA40, PacketDirection::ClientBound },
	{ 0x1B1CAA8, PacketDirection::ServerBound },
	{ 0x1B1CB10, PacketDirection::ServerBound },
	{ 0x1B1CB78, PacketDirection::ServerBound },
	{ 0x1B1CBF8, PacketDirection::ServerBound },
	{ 0x1B1CC78, PacketDirection::ClientBound },
	{ 0x1B1CCF8, PacketDirection::ClientBound },
	{ 0x1B1CD78, PacketDirection::ServerBound },
	{ 0x1B1CDF8, PacketDirection::ClientBound },
	{ 0x1B1CE60, PacketDirection::ClientBound },
	{ 0x1B1CEE0, PacketDirection::ClientBound },
	{ 0x1B1CF48, PacketDirection::ClientBound },
	{ 0x1B1CFB0, PacketDirection::ServerBound },
	{ 0x1B1D030, PacketDirection::ServerBound },
	{ 0x1B1D098, PacketDirection::ClientBound },
	{ 0x1B1D118, PacketDirection::ServerBound },
	{ 0x1B1A008, PacketDirection::ClientBound },
	{ 0x1B1A088, PacketDirection::ClientBound },
	{ 0x1B1A108, PacketDirection::ClientBound },
	{ 0x1B1A188, PacketDirection::ClientBound },
	{ 0x1B1A1F0, PacketDirection::ClientBound },
	{ 0x1B1A270, PacketDirection::ClientBound },
	{ 0x1B1A468, PacketDirection::ClientBound },
	{ 0x1B1A4E8, PacketDirection::ClientBound },
	{ 0x1B1A550, PacketDirection::ClientBound },
	{ 0x1B1A5D0, PacketDirection::ClientBound },
	{ 0x1B1A650, PacketDirection::ClientBound },
	{ 0x1B1A6B8, PacketDirection::ClientBound },
	{ 0x1B1A738, PacketDirection::ClientBound },
	{ 0x1B1A7B8, PacketDirection::ClientBound },
	{ 0x1B1A820, PacketDirection::ClientBound },
	{ 0x1B1A888, PacketDirection::ClientBound },
	{ 0x1B1A8F0, PacketDirection::ClientBound },
	{ 0x1B1A958, PacketDirection::ClientBound },
	{ 0x1B1A9D8, PacketDirection::ClientBound },
	{ 0x1B1AA58, PacketDirection::ClientBound },
	{ 0x1B1AAC0, PacketDirection::ClientBound },
	{ 0x1B1AB40, PacketDirection::ClientBound },
	{ 0x1B1ABA8, PacketDirection::ClientBound },
	{ 0x1B1AC28, PacketDirection::ClientBound },
	{ 0x1B1AC90, PacketDirection::ServerBound },
	{ 0x1B1ACF8, PacketDirection::ClientBound },
	{ 0x1B1ED48, PacketDirection::ClientBound },
	{ 0x1B1EDC8, PacketDirection::ClientBound },
	{ 0x1B1EE48, PacketDirection::ClientBound },
	{ 0x1B1EEC8, PacketDirection::ClientBound },
	{ 0x1B1EF48, PacketDirection::ClientBound },
	{ 0x1B1EFC8, PacketDirection::ClientBound },
	{ 0x1B1F048, PacketDirection::ServerBound },
	{ 0x1B1F0C8, PacketDirection::ServerBound },
	{ 0x1B1D380, PacketDirection::ClientBound },
	{ 0x1B1D400, PacketDirection::ServerBound },
	{ 0x1B1D480, PacketDirection::ServerBound },
	{ 0x1B1D500, PacketDirection::ServerBound },
	{ 0x1B1D568, PacketDirection::ServerBound },
	{ 0x1B1D5D0, PacketDirection::ServerBound },
	{ 0x1B1D638, PacketDirection::Bidirectional },
	{ 0x1B1D6A8, PacketDirection::Bidirectional },
	{ 0x1B1D730, PacketDirection::ServerBound },
	{ 0x1B1D798, PacketDirection::ServerBound },
	{ 0x1B1BA18, PacketDirection::ClientBound },
	{ 0x1B1BA98, PacketDirection::ClientBound },
	{ 0x1B1BB00, PacketDirection::Bidirectional },
	{ 0x1B1BB88, PacketDirection::ServerBound },
	{ 0x1B1BBF0, PacketDirection::ServerBound },
	{ 0x1B1DEF0, PacketDirection::ClientBound },
	{ 0x1B1DF70, PacketDirection::ServerBound },
	{ 0x1B1DFF0, PacketDirection::ClientBound },
	{ 0x1B1E070, PacketDirection::ClientBound },
	{ 0x1B1E0F0, PacketDirection::ClientBound },
	{ 0x1B1E158, PacketDirection::ClientBound },
	{ 0x1B1E1C0, PacketDirection::ClientBound },
	{ 0x1B1E240, PacketDirection::ClientBound },
	{ 0x1B1E2C0, PacketDirection::ClientBound },
	{ 0x1B1E340, PacketDirection::ServerBound },
	{ 0x1B1E3C0, PacketDirection::ClientBound },
	{ 0x1B1E440, PacketDirection::ClientBound },
	{ 0x1B1E4C0, PacketDirection::ClientBound },
	{ 0x1B1E540, PacketDirection::ClientBound },
	{ 0x1B1E5C0, PacketDirection::ClientBound },
	{ 0x1B1E640, PacketDirection::ClientBound },
	{ 0x1B1E6C0, PacketDirection::ClientBound },
	{ 0x1B1E740, PacketDirection::ClientBound },
	{ 0x1B1E7C0, PacketDirection::ClientBound },
	{ 0x1B1E828, PacketDirection::ClientBound },
	{ 0x1B1E8A8, PacketDirection::ServerBound },
	{ 0x1B1E910, PacketDirection::ClientBound },
	{ 0x1B1E990, PacketDirection::ClientBound },
	{ 0x1B1EA10, PacketDirection::ClientBound },
	{ 0x1B1B2E0, PacketDirection::ClientBound },
	{ 0x1B1B360, PacketDirection::ClientBound },
	{ 0x1B1B3C8, PacketDirection::ClientBound },
	{ 0x1B1B430, PacketDirection::ServerBound },
	{ 0x1B1B4B0, PacketDirection::ServerBound },
	{ 0x1B1B518, PacketDirection::ServerBound },
	{ 0x1B1B598, PacketDirection::ServerBound },
	{ 0x1B1C720, PacketDirection::ClientBound },
	{ 0x1B1C7A0, PacketDirection::ServerBound },
	{ 0x1B1C820, PacketDirection::ServerBound },
	{ 0x1B1D1F8, PacketDirection::ServerBound },
	{ 0x1B1D278, PacketDirection::ClientBound },
	{ 0x1B1D2F8, PacketDirection::Bidirectional },
	{ 0x1B1F590, PacketDirection::ClientBound },
	{ 0x1B1F5F8, PacketDirection::ClientBound },
	{ 0x1B1F660, PacketDirection::ServerBound },
	{ 0x1B1F6C8, PacketDirection::ClientBound },
	{ 0x1B1EA90, PacketDirection::Bidirectional },
	{ 0x1B1EB88, PacketDirection::ClientBound },
	{ 0x1B1EC08, PacketDirection::ClientBound },
	{ 0x1B1EC70, PacketDirection::ClientBound },
	{ 0x1B1ECD8, PacketDirection::Bidirectional },
	{ 0x1B22D00, PacketDirection::Bidirectional },
	{ 0x1B22D88, PacketDirection::Bidirectional },
	{ 0x1B22DF8, PacketDirection::ServerBound },
	{ 0x1B22E60, PacketDirection::ClientBound },
	{ 0x1B22EC8, PacketDirection::ClientBound },
	{ 0x1B22F48, PacketDirection::ClientBound },
	{ 0x1B22FC8, PacketDirection::ClientBound },
	{ 0x1B23048, PacketDirection::ServerBound },
	{ 0x1B230C8, PacketDirection::ServerBound },
	{ 0x1B23148, PacketDirection::ServerBound },
	{ 0x1B231C8, PacketDirection::ServerBound },
	{ 0x1B23248, PacketDirection::ServerBound },
	{ 0x1B232C8, PacketDirection::ClientBound },
	{ 0x1B23348, PacketDirection::ClientBound },
	{ 0x1B233C8, PacketDirection::ClientBound },
	{ 0x1B23448, PacketDirection::Bidirectional },
	{ 0x1B234D0, PacketDirection::ServerBound },
	{ 0x1B23550, PacketDirection::Bidirectional },
	{ 0x1B235D8, PacketDirection::ClientBound },
	{ 0x1B23658, PacketDirection::ClientBound },
	{ 0x1B236D8, PacketDirection::ServerBound },
	{ 0x1B23758, PacketDirection::ClientBound },
	{ 0x1B237D8, PacketDirection::ServerBound },
	{ 0x1B23858, PacketDirection::ServerBound },
	{ 0x1B238D8, PacketDirection::ClientBound },
	{ 0x1B23958, PacketDirection::ClientBound },
	{ 0x1B239D8, PacketDirection::ServerBound },
	{ 0x1B23A58, PacketDirection::ServerBound },
	{ 0x1B23AD8, PacketDirection::ServerBound },
	{ 0x1B23B58, PacketDirection::ServerBound },
	{ 0x1B23BD8, PacketDirection::ServerBound },
	{ 0x1B23C58, PacketDirection::ClientBound },
	{ 0x1B23CD8, PacketDirection::ServerBound },
	{ 0x1B23D58, PacketDirection::ClientBound },
	{ 0x1B23DD8, PacketDirection::ServerBound },
	{ 0x1B23E58, PacketDirection::ServerBound },
	{ 0x1B23ED8, PacketDirection::ClientBound },
	{ 0x1B23F40, PacketDirection::ServerBound },
	{ 0x1B23FC0, PacketDirection::ClientBound },
	{ 0x1B24028, PacketDirection::ClientBound },
	{ 0x1B24090, PacketDirection::ServerBound },
	{ 0x1B24110, PacketDirection::ServerBound },
	{ 0x1B24190, PacketDirection::ClientBound },
	{ 0x1B24210, PacketDirection::ClientBound },
	{ 0x1B24278, PacketDirection::ClientBound },
	{ 0x1B242F8, PacketDirection::ClientBound },
	{ 0x1B24378, PacketDirection::ClientBound },
	{ 0x1B24458, PacketDirection::ServerBound },
	{ 0x1B244D8, PacketDirection::ServerBound },
	{ 0x1B24558, PacketDirection::ClientBound },
	{ 0x1B245D8, PacketDirection::ServerBound },
	{ 0x1B24640, PacketDirection::ClientBound },
	{ 0x1B246A8, PacketDirection::ServerBound },
	{ 0x1B24728, PacketDirection::ClientBound },
	{ 0x1B247A8, PacketDirection::ClientBound },
	{ 0x1B1ADC8, PacketDirection::ClientBound },
	{ 0x1B1AE30, PacketDirection::ClientBound },
	{ 0x1B1AD60, PacketDirection::ClientBound },
	{ 0x1B1FF50, PacketDirection::ServerBound },
	{ 0x1B1FFB8, PacketDirection::ServerBound },
	{ 0x1B20038, PacketDirection::ServerBound },
	{ 0x1B200A0, PacketDirection::ServerBound },
	{ 0x1B20120, PacketDirection::ServerBound },
	{ 0x1B201A0, PacketDirection::ServerBound },
	{ 0x1B20208, PacketDirection::ServerBound },
	{ 0x1B20270, PacketDirection::ServerBound },
	{ 0x1B202D8, PacketDirection::ServerBound },
	{ 0x1B20340, PacketDirection::ServerBound },
	{ 0x1B203A8, PacketDirection::ServerBound },
	{ 0x1B20498, PacketDirection::ClientBound },
	{ 0x1B20500, PacketDirection::ServerBound },
	{ 0x1B20568, PacketDirection::ServerBound },
	{ 0x1B205D0, PacketDirection::ServerBound },
	{ 0x1B20638, PacketDirection::ClientBound },
	{ 0x1B206A0, PacketDirection::ClientBound },
	{ 0x1B20708, PacketDirection::ServerBound },
	{ 0x1B20770, PacketDirection::ClientBound },
	{ 0x1B207D8, PacketDirection::ClientBound },
	{ 0x1B20840, PacketDirection::ServerBound },
	{ 0x1B208A8, PacketDirection::ServerBound },
	{ 0x1B20928, PacketDirection::ServerBound },
	{ 0x1B20990, PacketDirection::ServerBound },
	{ 0x1B20A10, PacketDirection::ServerBound },
	{ 0x1B20A78, PacketDirection::ServerBound },
	{ 0x1B20AE0, PacketDirection::ServerBound },
	{ 0x1B1BCD0, PacketDirection::ServerBound },
	{ 0x1B1BD50, PacketDirection::ClientBound },
	{ 0x1B1BDB8, PacketDirection::ClientBound },
	{ 0x1B1BE80, PacketDirection::ServerBound },
	{ 0x1B1BF00, PacketDirection::ClientBound }
};

constexpr size_t PACKET_COUNT = sizeof(PACKET_REGISTRY) / sizeof(PacketEntry);

inline uint64_t GetPacketMethodTable(PacketIndex index) {
	if (index < PACKET_COUNT) {
		return gameBase + PACKET_REGISTRY[index].methodTableOffset;
	}
	return 0;
}

inline PacketDirection GetPacketDirection(PacketIndex index) {
	if (index < PACKET_COUNT) {
		return PACKET_REGISTRY[index].direction;
	}
	return PacketDirection::ServerBound; // Default fallback
}

template<typename T>
inline T CreatePacket(PacketIndex index) {
	return API::RHPNewFast<T>(GetPacketMethodTable(index));
}

inline PacketIndex GetPacketIndex(Object* packet) {
	for (int i = 0; i < sizeof(PACKET_REGISTRY) / sizeof(PacketEntry); i++) {
		PacketEntry entry = PACKET_REGISTRY[i];
		if ((uint64_t)packet->methodTable - gameBase == entry.methodTableOffset)
			return (PacketIndex)i;
	}
	return Unkown;
}