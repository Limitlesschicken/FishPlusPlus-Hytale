/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include <cstdint>
#include <algorithm>

#include "sdk/BaseDataTypes/Object.h"

 // Auto-generated packet definitions from dumper
 // Total packets found: 320

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
	UnknownPacket = -1,
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
	SetupFinalize_C2S = 25,                                    // ID: 33 [Server-Bound]
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
	TrackOrUpdateObjective_S2C = 55,                           // ID: 69 [Client-Bound]
	UntrackObjective_S2C = 56,                                 // ID: 70 [Client-Bound]
	UpdateObjectiveTask_S2C = 57,                              // ID: 71 [Client-Bound]
	UpdateEntityStatTypes_S2C = 58,                            // ID: 72 [Client-Bound]
	UpdateEntityUIComponents_S2C = 59,                         // ID: 73 [Client-Bound]
	UpdateHitboxCollisionConfig_S2C = 60,                      // ID: 74 [Client-Bound]
	UpdateRepulsionConfig_S2C = 61,                            // ID: 75 [Client-Bound]
	UpdateViewBobbing_S2C = 62,                                // ID: 76 [Client-Bound]
	UpdateCameraShake_S2C = 63,                                // ID: 77 [Client-Bound]
	UpdateBlockGroups_S2C = 64,                                // ID: 78 [Client-Bound]
	UpdateSoundSets_S2C = 65,                                  // ID: 79 [Client-Bound]
	UpdateAudioCategories_S2C = 66,                            // ID: 80 [Client-Bound]
	UpdateReverbEffects_S2C = 67,                              // ID: 81 [Client-Bound]
	UpdateEqualizerEffects_S2C = 68,                           // ID: 82 [Client-Bound]
	UpdateFluids_S2C = 69,                                     // ID: 83 [Client-Bound]
	UpdateTagPatterns_S2C = 70,                                // ID: 84 [Client-Bound]
	UpdateProjectileConfigs_S2C = 71,                          // ID: 85 [Client-Bound]
	UpdateEmotes_S2C = 72,                                     // ID: 86 [Client-Bound]
	UpdatePhysicalMaterials_S2C = 73,                          // ID: 87 [Client-Bound]
	UpdateMusicContainers_S2C = 74,                            // ID: 88 [Client-Bound]
	UpdateAudioStates_S2C = 75,                                // ID: 89 [Client-Bound]
	SetClientId_S2C = 76,                                      // ID: 100 [Client-Bound]
	SetGameMode_S2C = 77,                                      // ID: 101 [Client-Bound]
	SetMovementStates_S2C = 78,                                // ID: 102 [Client-Bound]
	SetBlockPlacementOverride_S2C = 79,                        // ID: 103 [Client-Bound]
	JoinWorld_S2C = 80,                                        // ID: 104 [Client-Bound]
	ClientReady_C2S = 81,                                      // ID: 105 [Server-Bound]
	LoadHotbar_C2S = 82,                                       // ID: 106 [Server-Bound]
	SaveHotbar_C2S = 83,                                       // ID: 107 [Server-Bound]
	ClientMovement_C2S = 84,                                   // ID: 108 [Server-Bound]
	ClientTeleport_S2C = 85,                                   // ID: 109 [Client-Bound]
	UpdateMovementSettings_S2C = 86,                           // ID: 110 [Client-Bound]
	MouseInteraction_C2S = 87,                                 // ID: 111 [Server-Bound]
	DamageInfo_S2C = 88,                                       // ID: 112 [Client-Bound]
	ReticleEvent_S2C = 89,                                     // ID: 113 [Client-Bound]
	DisplayDebug_S2C = 90,                                     // ID: 114 [Client-Bound]
	ClearDebugShapes_S2C = 91,                                 // ID: 115 [Client-Bound]
	SyncPlayerPreferences_C2S = 92,                            // ID: 116 [Server-Bound]
	ClientPlaceBlock_C2S = 93,                                 // ID: 117 [Server-Bound]
	UpdateMemoriesFeatureStatus_S2C = 94,                      // ID: 118 [Client-Bound]
	RemoveMapMarker_C2S = 95,                                  // ID: 119 [Server-Bound]
	UpdateMemoriesCount_S2C = 96,                              // ID: 120 [Client-Bound]
	SetChunk_S2C = 97,                                         // ID: 131 [Client-Bound]
	SetChunkHeightmap_S2C = 98,                                // ID: 132 [Client-Bound]
	SetChunkTintmap_S2C = 99,                                  // ID: 133 [Client-Bound]
	SetChunkEnvironments_S2C = 100,                            // ID: 134 [Client-Bound]
	UnloadChunk_S2C = 101,                                     // ID: 135 [Client-Bound]
	SetFluids_S2C = 102,                                       // ID: 136 [Client-Bound]
	ServerSetBlock_S2C = 103,                                  // ID: 140 [Client-Bound]
	ServerSetBlocks_S2C = 104,                                 // ID: 141 [Client-Bound]
	ServerSetFluid_S2C = 105,                                  // ID: 142 [Client-Bound]
	UpdateTimeSettings_S2C = 106,                              // ID: 145 [Client-Bound]
	UpdateTime_S2C = 107,                                      // ID: 146 [Client-Bound]
	ClearEditorTimeOverride_S2C = 108,                         // ID: 148 [Client-Bound]
	UpdateWeather_S2C = 109,                                   // ID: 149 [Client-Bound]
	UpdateEditorWeatherOverride_S2C = 110,                     // ID: 150 [Client-Bound]
	UpdateForcedMusic_BI = 111,                                // ID: 151 [Bidirectional]
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
	SetAudioState_S2C = 128,                                   // ID: 168 [Client-Bound]
	UpdatePlayerInventory_S2C = 129,                           // ID: 170 [Client-Bound]
	SetCreativeItem_C2S = 130,                                 // ID: 171 [Server-Bound]
	DropCreativeItem_C2S = 131,                                // ID: 172 [Server-Bound]
	SmartGiveCreativeItem_C2S = 132,                           // ID: 173 [Server-Bound]
	DropItemStack_C2S = 133,                                   // ID: 174 [Server-Bound]
	MoveItemStack_C2S = 134,                                   // ID: 175 [Server-Bound]
	SmartMoveItemStack_BI = 135,                               // ID: 176 [Bidirectional]
	SetActiveSlot_BI = 136,                                    // ID: 177 [Bidirectional]
	SwitchHotbarBlockSet_C2S = 137,                            // ID: 178 [Server-Bound]
	InventoryAction_C2S = 138,                                 // ID: 179 [Server-Bound]
	OpenWindow_S2C = 139,                                      // ID: 200 [Client-Bound]
	UpdateWindow_S2C = 140,                                    // ID: 201 [Client-Bound]
	CloseWindow_BI = 141,                                      // ID: 202 [Bidirectional]
	SendWindowAction_C2S = 142,                                // ID: 203 [Server-Bound]
	ClientOpenWindow_C2S = 143,                                // ID: 204 [Server-Bound]
	ServerMessage_S2C = 144,                                   // ID: 210 [Client-Bound]
	ChatMessage_C2S = 145,                                     // ID: 211 [Server-Bound]
	Notification_S2C = 146,                                    // ID: 212 [Client-Bound]
	KillFeedMessage_S2C = 147,                                 // ID: 213 [Client-Bound]
	ShowEventTitle_S2C = 148,                                  // ID: 214 [Client-Bound]
	HideEventTitle_S2C = 149,                                  // ID: 215 [Client-Bound]
	SetPage_S2C = 150,                                         // ID: 216 [Client-Bound]
	CustomHud_S2C = 151,                                       // ID: 217 [Client-Bound]
	CustomPage_S2C = 152,                                      // ID: 218 [Client-Bound]
	CustomPageEvent_C2S = 153,                                 // ID: 219 [Server-Bound]
	EditorBlocksChange_S2C = 154,                              // ID: 222 [Client-Bound]
	ServerInfo_S2C = 155,                                      // ID: 223 [Client-Bound]
	AddToServerPlayerList_S2C = 156,                           // ID: 224 [Client-Bound]
	RemoveFromServerPlayerList_S2C = 157,                      // ID: 225 [Client-Bound]
	UpdateServerPlayerList_S2C = 158,                          // ID: 226 [Client-Bound]
	UpdateServerPlayerListPing_S2C = 159,                      // ID: 227 [Client-Bound]
	UpdateKnownRecipes_S2C = 160,                              // ID: 228 [Client-Bound]
	UpdatePortal_S2C = 161,                                    // ID: 229 [Client-Bound]
	UpdateVisibleHudComponents_S2C = 162,                      // ID: 230 [Client-Bound]
	ResetUserInterfaceState_S2C = 163,                         // ID: 231 [Client-Bound]
	UpdateLanguage_C2S = 164,                                  // ID: 232 [Server-Bound]
	WorldSavingStatus_S2C = 165,                               // ID: 233 [Client-Bound]
	OpenChatWithCommand_S2C = 166,                             // ID: 234 [Client-Bound]
	UpdateAnchorUI_S2C = 167,                                  // ID: 235 [Client-Bound]
	CommandSuggestionsRequest_C2S = 168,                       // ID: 236 [Server-Bound]
	CommandSuggestionsResponse_S2C = 169,                      // ID: 237 [Client-Bound]
	CommandTreeSync_S2C = 170,                                 // ID: 238 [Client-Bound]
	ArgValuesRequest_C2S = 171,                                // ID: 239 [Server-Bound]
	UpdateWorldMapSettings_S2C = 172,                          // ID: 240 [Client-Bound]
	UpdateWorldMap_S2C = 173,                                  // ID: 241 [Client-Bound]
	ClearWorldMap_S2C = 174,                                   // ID: 242 [Client-Bound]
	UpdateWorldMapVisible_C2S = 175,                           // ID: 243 [Server-Bound]
	TeleportToWorldMapMarker_C2S = 176,                        // ID: 244 [Server-Bound]
	TeleportToWorldMapPosition_C2S = 177,                      // ID: 245 [Server-Bound]
	CreateUserMarker_C2S = 178,                                // ID: 246 [Server-Bound]
	ArgValuesResponse_S2C = 179,                               // ID: 247 [Client-Bound]
	ArgCacheInvalidation_S2C = 180,                            // ID: 248 [Client-Bound]
	RequestServerAccess_S2C = 181,                             // ID: 250 [Client-Bound]
	UpdateServerAccess_C2S = 182,                              // ID: 251 [Server-Bound]
	SetServerAccess_C2S = 183,                                 // ID: 252 [Server-Bound]
	RequestMachinimaActorModel_C2S = 184,                      // ID: 260 [Server-Bound]
	SetMachinimaActorModel_S2C = 185,                          // ID: 261 [Client-Bound]
	UpdateMachinimaScene_BI = 186,                             // ID: 262 [Bidirectional]
	SetServerCamera_S2C = 187,                                 // ID: 280 [Client-Bound]
	CameraShakeEffect_S2C = 188,                               // ID: 281 [Client-Bound]
	RequestFlyCameraMode_C2S = 189,                            // ID: 282 [Server-Bound]
	SetFlyCameraMode_S2C = 190,                                // ID: 283 [Client-Bound]
	SyncInteractionChains_BI = 191,                            // ID: 290 [Bidirectional]
	CancelInteractionChain_S2C = 192,                          // ID: 291 [Client-Bound]
	PlayInteractionFor_S2C = 193,                              // ID: 292 [Client-Bound]
	MountNPC_S2C = 194,                                        // ID: 293 [Client-Bound]
	DismountNPC_BI = 195,                                      // ID: 294 [Bidirectional]
	FailureReply_BI = 196,                                     // ID: 300 [Bidirectional]
	SuccessReply_BI = 197,                                     // ID: 301 [Bidirectional]
	AssetEditorInitialize_C2S = 198,                           // ID: 302 [Server-Bound]
	AssetEditorAuthorization_S2C = 199,                        // ID: 303 [Client-Bound]
	AssetEditorCapabilities_S2C = 200,                         // ID: 304 [Client-Bound]
	AssetEditorSetupSchemas_S2C = 201,                         // ID: 305 [Client-Bound]
	AssetEditorSetupAssetTypes_S2C = 202,                      // ID: 306 [Client-Bound]
	AssetEditorCreateDirectory_C2S = 203,                      // ID: 307 [Server-Bound]
	AssetEditorDeleteDirectory_C2S = 204,                      // ID: 308 [Server-Bound]
	AssetEditorRenameDirectory_C2S = 205,                      // ID: 309 [Server-Bound]
	AssetEditorFetchAsset_C2S = 206,                           // ID: 310 [Server-Bound]
	AssetEditorFetchJsonAssetWithParents_C2S = 207,            // ID: 311 [Server-Bound]
	AssetEditorFetchAssetReply_S2C = 208,                      // ID: 312 [Client-Bound]
	AssetEditorFetchJsonAssetWithParentsReply_S2C = 209,       // ID: 313 [Client-Bound]
	AssetEditorAssetPackSetup_S2C = 210,                       // ID: 314 [Client-Bound]
	AssetEditorUpdateAssetPack_BI = 211,                       // ID: 315 [Bidirectional]
	AssetEditorCreateAssetPack_C2S = 212,                      // ID: 316 [Server-Bound]
	AssetEditorDeleteAssetPack_BI = 213,                       // ID: 317 [Bidirectional]
	AssetEditorAssetListSetup_S2C = 214,                       // ID: 319 [Client-Bound]
	AssetEditorAssetListUpdate_S2C = 215,                      // ID: 320 [Client-Bound]
	AssetEditorRequestChildrenList_C2S = 216,                  // ID: 321 [Server-Bound]
	AssetEditorRequestChildrenListReply_S2C = 217,             // ID: 322 [Client-Bound]
	AssetEditorUpdateJsonAsset_C2S = 218,                      // ID: 323 [Server-Bound]
	AssetEditorUpdateAsset_C2S = 219,                          // ID: 324 [Server-Bound]
	AssetEditorJsonAssetUpdated_S2C = 220,                     // ID: 325 [Client-Bound]
	AssetEditorAssetUpdated_S2C = 221,                         // ID: 326 [Client-Bound]
	AssetEditorCreateAsset_C2S = 222,                          // ID: 327 [Server-Bound]
	AssetEditorRenameAsset_C2S = 223,                          // ID: 328 [Server-Bound]
	AssetEditorDeleteAsset_C2S = 224,                          // ID: 329 [Server-Bound]
	AssetEditorDiscardChanges_C2S = 225,                       // ID: 330 [Server-Bound]
	AssetEditorFetchAutoCompleteData_C2S = 226,                // ID: 331 [Server-Bound]
	AssetEditorFetchAutoCompleteDataReply_S2C = 227,           // ID: 332 [Client-Bound]
	AssetEditorRequestDataset_C2S = 228,                       // ID: 333 [Server-Bound]
	AssetEditorRequestDatasetReply_S2C = 229,                  // ID: 334 [Client-Bound]
	AssetEditorActivateButton_C2S = 230,                       // ID: 335 [Server-Bound]
	AssetEditorSelectAsset_C2S = 231,                          // ID: 336 [Server-Bound]
	AssetEditorPopupNotification_S2C = 232,                    // ID: 337 [Client-Bound]
	AssetEditorFetchLastModifiedAssets_C2S = 233,              // ID: 338 [Server-Bound]
	AssetEditorLastModifiedAssets_S2C = 234,                   // ID: 339 [Client-Bound]
	AssetEditorModifiedAssetsCount_S2C = 235,                  // ID: 340 [Client-Bound]
	AssetEditorSubscribeModifiedAssetsChanges_C2S = 236,       // ID: 341 [Server-Bound]
	AssetEditorExportAssets_C2S = 237,                         // ID: 342 [Server-Bound]
	AssetEditorExportAssetInitialize_S2C = 238,                // ID: 343 [Client-Bound]
	AssetEditorExportAssetPart_S2C = 239,                      // ID: 344 [Client-Bound]
	AssetEditorExportAssetFinalize_S2C = 240,                  // ID: 345 [Client-Bound]
	AssetEditorExportDeleteAssets_S2C = 241,                   // ID: 346 [Client-Bound]
	AssetEditorExportComplete_S2C = 242,                       // ID: 347 [Client-Bound]
	AssetEditorUndoChanges_C2S = 243,                          // ID: 349 [Server-Bound]
	AssetEditorRedoChanges_C2S = 244,                          // ID: 350 [Server-Bound]
	AssetEditorUndoRedoReply_S2C = 245,                        // ID: 351 [Client-Bound]
	AssetEditorSetGameTime_C2S = 246,                          // ID: 352 [Server-Bound]
	AssetEditorUpdateSecondsPerGameDay_S2C = 247,              // ID: 353 [Client-Bound]
	AssetEditorUpdateWeatherPreviewLock_C2S = 248,             // ID: 354 [Server-Bound]
	AssetEditorUpdateModelPreview_S2C = 249,                   // ID: 355 [Client-Bound]
	AssetEditorModsDirectories_S2C = 250,                      // ID: 356 [Client-Bound]
	UpdateSunSettings_S2C = 251,                               // ID: 360 [Client-Bound]
	UpdatePostFxSettings_S2C = 252,                            // ID: 361 [Client-Bound]
	PlaySoundEventLocalPlayer_S2C = 253,                       // ID: 362 [Client-Bound]
	InsecurePlayerOptions_C2S = 254,                           // ID: 363 [Server-Bound]
	RequestInsecurePlayerOptions_S2C = 255,                    // ID: 364 [Client-Bound]
	BuilderToolArgUpdate_C2S = 256,                            // ID: 400 [Server-Bound]
	BuilderToolEntityAction_C2S = 257,                         // ID: 401 [Server-Bound]
	BuilderToolSetEntityTransform_C2S = 258,                   // ID: 402 [Server-Bound]
	BuilderToolExtrudeAction_C2S = 259,                        // ID: 403 [Server-Bound]
	BuilderToolStackArea_C2S = 260,                            // ID: 404 [Server-Bound]
	BuilderToolSelectionTransform_C2S = 261,                   // ID: 405 [Server-Bound]
	BuilderToolRotateClipboard_C2S = 262,                      // ID: 406 [Server-Bound]
	BuilderToolPasteClipboard_C2S = 263,                       // ID: 407 [Server-Bound]
	BuilderToolSetTransformationModeState_C2S = 264,           // ID: 408 [Server-Bound]
	BuilderToolSelectionUpdate_C2S = 265,                      // ID: 409 [Server-Bound]
	BuilderToolSelectionToolAskForClipboard_C2S = 266,         // ID: 410 [Server-Bound]
	BuilderToolSelectionToolReplyWithClipboard_S2C = 267,      // ID: 411 [Client-Bound]
	BuilderToolGeneralAction_C2S = 268,                        // ID: 412 [Server-Bound]
	BuilderToolOnUseInteraction_C2S = 269,                     // ID: 413 [Server-Bound]
	BuilderToolLineAction_C2S = 270,                           // ID: 414 [Server-Bound]
	BuilderToolShowAnchor_S2C = 271,                           // ID: 415 [Client-Bound]
	BuilderToolHideAnchors_S2C = 272,                          // ID: 416 [Client-Bound]
	PrefabUnselectPrefab_C2S = 273,                            // ID: 417 [Server-Bound]
	BuilderToolsSetSoundSet_S2C = 274,                         // ID: 418 [Client-Bound]
	BuilderToolLaserPointer_S2C = 275,                         // ID: 419 [Client-Bound]
	BuilderToolSetEntityScale_C2S = 276,                       // ID: 420 [Server-Bound]
	BuilderToolSetEntityPickupEnabled_C2S = 277,               // ID: 421 [Server-Bound]
	BuilderToolSetEntityLight_C2S = 278,                       // ID: 422 [Server-Bound]
	BuilderToolSetNPCDebug_C2S = 279,                          // ID: 423 [Server-Bound]
	BuilderToolSetEntityCollision_C2S = 280,                   // ID: 425 [Server-Bound]
	PrefabSetAnchor_C2S = 281,                                 // ID: 426 [Server-Bound]
	BuilderToolResetClipboardRotation_C2S = 282,               // ID: 427 [Server-Bound]
	BuilderToolRandomizeClipboard_C2S = 283,                   // ID: 428 [Server-Bound]
	BuilderToolPrefabPreview_S2C = 284,                        // ID: 429 [Client-Bound]
	BuilderToolGMaskPreset_S2C = 285,                          // ID: 430 [Client-Bound]
	BuilderToolGMaskPresetLoadResponse_C2S = 286,              // ID: 431 [Server-Bound]
	RelayedVoiceData_S2C = 287,                                // ID: 451 [Client-Bound]
	VoiceConfig_S2C = 288,                                     // ID: 452 [Client-Bound]
	StreamOpen_C2S = 289,                                      // ID: 460 [Server-Bound]
	StreamOpenResponse_S2C = 290,                              // ID: 461 [Client-Bound]
	UpdateTriggerVolumeDisplay_S2C = 291,                      // ID: 470 [Client-Bound]
	AddOrUpdateTriggerVolumeDisplay_S2C = 292,                 // ID: 471 [Client-Bound]
	RemoveTriggerVolumeDisplay_S2C = 293,                      // ID: 472 [Client-Bound]
	TriggerVolumeToolCreate_C2S = 294,                         // ID: 480 [Server-Bound]
	TriggerVolumeToolMove_C2S = 295,                           // ID: 481 [Server-Bound]
	TriggerVolumeToolResize_C2S = 296,                         // ID: 482 [Server-Bound]
	TriggerVolumeToolDelete_C2S = 297,                         // ID: 483 [Server-Bound]
	TriggerVolumeToolEquip_C2S = 298,                          // ID: 484 [Server-Bound]
	TriggerVolumeToolCreateResponse_S2C = 299,                 // ID: 485 [Client-Bound]
	TriggerVolumeToolGroupCreate_C2S = 300,                    // ID: 486 [Server-Bound]
	TriggerVolumeToolGroupCreateResponse_S2C = 301,            // ID: 487 [Client-Bound]
	TriggerVolumeToolUngroup_C2S = 302,                        // ID: 488 [Server-Bound]
	TriggerVolumeToolGroupMove_C2S = 303,                      // ID: 489 [Server-Bound]
	TriggerVolumeToolSelect_C2S = 304,                         // ID: 490 [Server-Bound]
	TriggerVolumeToolMultiMove_C2S = 305,                      // ID: 491 [Server-Bound]
	TriggerVolumeToolSetColor_C2S = 306,                       // ID: 492 [Server-Bound]
	TriggerVolumeToolSetTargetTypes_C2S = 307,                 // ID: 493 [Server-Bound]
	TriggerVolumeToolSetKeepLoaded_C2S = 308,                  // ID: 500 [Server-Bound]
	TriggerVolumeToolSetCooldown_C2S = 309,                    // ID: 501 [Server-Bound]
	TriggerVolumeToolSetActivationDelay_C2S = 310,             // ID: 502 [Server-Bound]
	SelectionToolShowTriggerVolumes_C2S = 311,                 // ID: 503 [Server-Bound]
	TriggerVolumeToolSetCancelDelayedOnExit_C2S = 312,         // ID: 504 [Server-Bound]
	TriggerVolumeToolDuplicate_C2S = 313,                      // ID: 505 [Server-Bound]
	TriggerVolumeToolSetConditionTiming_C2S = 314,             // ID: 506 [Server-Bound]
	TriggerVolumeToolSelection_S2C = 315,                      // ID: 507 [Client-Bound]
	ShowTriggerVolumePastePrefabPreview_S2C = 316,             // ID: 508 [Client-Bound]
	HideTriggerVolumePastePrefabPreview_S2C = 317,             // ID: 509 [Client-Bound]
	UpdateServersideUIPage_S2C = 318,                          // ID: 1200 [Client-Bound]
	ExecuteServersidePageCommand_C2S = 319                     // ID: 1202 [Server-Bound]
};

// Packet registry (method table offset from game base + direction)
constexpr PacketEntry PACKET_REGISTRY[] = {
	{ 0x1F68D78, PacketDirection::ServerBound },
	{ 0x1F68DF8, PacketDirection::ClientBound },
	{ 0x1F68E78, PacketDirection::ClientBound },
	{ 0x1F68EF8, PacketDirection::ServerBound },
	{ 0x1F6A878, PacketDirection::ClientBound },
	{ 0x1F6A8F8, PacketDirection::ServerBound },
	{ 0x1F6A978, PacketDirection::ClientBound },
	{ 0x1F6A9F8, PacketDirection::ClientBound },
	{ 0x1F6AA78, PacketDirection::ServerBound },
	{ 0x1F6AAE0, PacketDirection::ClientBound },
	{ 0x1F6AB60, PacketDirection::ClientBound },
	{ 0x1F6ABE0, PacketDirection::ClientBound },
	{ 0x1F63C08, PacketDirection::ClientBound },
	{ 0x1F63C88, PacketDirection::ClientBound },
	{ 0x1F63CF0, PacketDirection::ClientBound },
	{ 0x1F63D70, PacketDirection::ServerBound },
	{ 0x1F63DF0, PacketDirection::ClientBound },
	{ 0x1F63E70, PacketDirection::ClientBound },
	{ 0x1F63ED8, PacketDirection::ClientBound },
	{ 0x1F63F58, PacketDirection::ClientBound },
	{ 0x1F63FC0, PacketDirection::ClientBound },
	{ 0x1F64028, PacketDirection::ClientBound },
	{ 0x1F64090, PacketDirection::ClientBound },
	{ 0x1F64110, PacketDirection::ClientBound },
	{ 0x1F64178, PacketDirection::Bidirectional },
	{ 0x1F641E8, PacketDirection::ServerBound },
	{ 0x1F64268, PacketDirection::ClientBound },
	{ 0x1F6AC60, PacketDirection::ClientBound },
	{ 0x1F6ACE0, PacketDirection::ClientBound },
	{ 0x1F6AD60, PacketDirection::ClientBound },
	{ 0x1F6ADE0, PacketDirection::ClientBound },
	{ 0x1F6AE60, PacketDirection::ClientBound },
	{ 0x1F6AEE0, PacketDirection::ClientBound },
	{ 0x1F6AF60, PacketDirection::ClientBound },
	{ 0x1F6AFE0, PacketDirection::ClientBound },
	{ 0x1F6B060, PacketDirection::ClientBound },
	{ 0x1F6B0E0, PacketDirection::ClientBound },
	{ 0x1F6B160, PacketDirection::ClientBound },
	{ 0x1F6B1E0, PacketDirection::ClientBound },
	{ 0x1F6B260, PacketDirection::ClientBound },
	{ 0x1F6B2E0, PacketDirection::ClientBound },
	{ 0x1F6B360, PacketDirection::ClientBound },
	{ 0x1F6B3E0, PacketDirection::ClientBound },
	{ 0x1F6B460, PacketDirection::ClientBound },
	{ 0x1F6B4E0, PacketDirection::ClientBound },
	{ 0x1F6B560, PacketDirection::ClientBound },
	{ 0x1F6B5E0, PacketDirection::ClientBound },
	{ 0x1F6B660, PacketDirection::ClientBound },
	{ 0x1F6B6E0, PacketDirection::ClientBound },
	{ 0x1F6B760, PacketDirection::ClientBound },
	{ 0x1F6B7E0, PacketDirection::ClientBound },
	{ 0x1F6B860, PacketDirection::ClientBound },
	{ 0x1F6B8E0, PacketDirection::ClientBound },
	{ 0x1F6B960, PacketDirection::ClientBound },
	{ 0x1F6B9E0, PacketDirection::ClientBound },
	{ 0x1F6BAE0, PacketDirection::ClientBound },
	{ 0x1F6BB48, PacketDirection::ClientBound },
	{ 0x1F6BBC8, PacketDirection::ClientBound },
	{ 0x1F6BC48, PacketDirection::ClientBound },
	{ 0x1F6BCC8, PacketDirection::ClientBound },
	{ 0x1F6BD48, PacketDirection::ClientBound },
	{ 0x1F6BDC8, PacketDirection::ClientBound },
	{ 0x1F6BE48, PacketDirection::ClientBound },
	{ 0x1F6BEC8, PacketDirection::ClientBound },
	{ 0x1F6BF48, PacketDirection::ClientBound },
	{ 0x1F6BFC8, PacketDirection::ClientBound },
	{ 0x1F6C048, PacketDirection::ClientBound },
	{ 0x1F6C0C8, PacketDirection::ClientBound },
	{ 0x1F6C148, PacketDirection::ClientBound },
	{ 0x1F6C1C8, PacketDirection::ClientBound },
	{ 0x1F6C248, PacketDirection::ClientBound },
	{ 0x1F6C2C8, PacketDirection::ClientBound },
	{ 0x1F6C348, PacketDirection::ClientBound },
	{ 0x1F6C3C8, PacketDirection::ClientBound },
	{ 0x1F6C448, PacketDirection::ClientBound },
	{ 0x1F6C4C8, PacketDirection::ClientBound },
	{ 0x1F64498, PacketDirection::ClientBound },
	{ 0x1F64500, PacketDirection::ClientBound },
	{ 0x1F64580, PacketDirection::ClientBound },
	{ 0x1F645E8, PacketDirection::ClientBound },
	{ 0x1F64650, PacketDirection::ClientBound },
	{ 0x1F646B8, PacketDirection::ServerBound },
	{ 0x1F64720, PacketDirection::ServerBound },
	{ 0x1F64788, PacketDirection::ServerBound },
	{ 0x1F64808, PacketDirection::ServerBound },
	{ 0x1F64888, PacketDirection::ClientBound },
	{ 0x1F64908, PacketDirection::ClientBound },
	{ 0x1F64988, PacketDirection::ServerBound },
	{ 0x1F64A08, PacketDirection::ClientBound },
	{ 0x1F64A70, PacketDirection::ClientBound },
	{ 0x1F64AF0, PacketDirection::ClientBound },
	{ 0x1F64B58, PacketDirection::ClientBound },
	{ 0x1F64BD8, PacketDirection::ServerBound },
	{ 0x1F64C58, PacketDirection::ServerBound },
	{ 0x1F64CC0, PacketDirection::ClientBound },
	{ 0x1F64D40, PacketDirection::ServerBound },
	{ 0x1F64DA8, PacketDirection::ClientBound },
	{ 0x1F61C38, PacketDirection::ClientBound },
	{ 0x1F61CB8, PacketDirection::ClientBound },
	{ 0x1F61D38, PacketDirection::ClientBound },
	{ 0x1F61DB8, PacketDirection::ClientBound },
	{ 0x1F61E20, PacketDirection::ClientBound },
	{ 0x1F61EA0, PacketDirection::ClientBound },
	{ 0x1F62098, PacketDirection::ClientBound },
	{ 0x1F62118, PacketDirection::ClientBound },
	{ 0x1F62180, PacketDirection::ClientBound },
	{ 0x1F622E8, PacketDirection::ClientBound },
	{ 0x1F62368, PacketDirection::ClientBound },
	{ 0x1F62450, PacketDirection::ClientBound },
	{ 0x1F624B8, PacketDirection::ClientBound },
	{ 0x1F62520, PacketDirection::ClientBound },
	{ 0x1F62588, PacketDirection::Bidirectional },
	{ 0x1F62608, PacketDirection::ClientBound },
	{ 0x1F62688, PacketDirection::ClientBound },
	{ 0x1F626F0, PacketDirection::ClientBound },
	{ 0x1F62770, PacketDirection::ClientBound },
	{ 0x1F627D8, PacketDirection::ClientBound },
	{ 0x1F62858, PacketDirection::ClientBound },
	{ 0x1F628C0, PacketDirection::ServerBound },
	{ 0x1F62928, PacketDirection::ClientBound },
	{ 0x1F68730, PacketDirection::ClientBound },
	{ 0x1F687B0, PacketDirection::ClientBound },
	{ 0x1F68830, PacketDirection::ClientBound },
	{ 0x1F688B0, PacketDirection::ClientBound },
	{ 0x1F68930, PacketDirection::ClientBound },
	{ 0x1F689B0, PacketDirection::ClientBound },
	{ 0x1F68A30, PacketDirection::ServerBound },
	{ 0x1F68AB0, PacketDirection::ServerBound },
	{ 0x1F62AE0, PacketDirection::ClientBound },
	{ 0x1F65E78, PacketDirection::ClientBound },
	{ 0x1F65EF8, PacketDirection::ServerBound },
	{ 0x1F65F78, PacketDirection::ServerBound },
	{ 0x1F65FF8, PacketDirection::ServerBound },
	{ 0x1F66060, PacketDirection::ServerBound },
	{ 0x1F660C8, PacketDirection::ServerBound },
	{ 0x1F66130, PacketDirection::Bidirectional },
	{ 0x1F661A0, PacketDirection::Bidirectional },
	{ 0x1F66228, PacketDirection::ServerBound },
	{ 0x1F66290, PacketDirection::ServerBound },
	{ 0x1F63640, PacketDirection::ClientBound },
	{ 0x1F636C0, PacketDirection::ClientBound },
	{ 0x1F63728, PacketDirection::Bidirectional },
	{ 0x1F637B0, PacketDirection::ServerBound },
	{ 0x1F63818, PacketDirection::ServerBound },
	{ 0x1F672A8, PacketDirection::ClientBound },
	{ 0x1F67328, PacketDirection::ServerBound },
	{ 0x1F673A8, PacketDirection::ClientBound },
	{ 0x1F67428, PacketDirection::ClientBound },
	{ 0x1F674A8, PacketDirection::ClientBound },
	{ 0x1F67510, PacketDirection::ClientBound },
	{ 0x1F67578, PacketDirection::ClientBound },
	{ 0x1F675F8, PacketDirection::ClientBound },
	{ 0x1F67678, PacketDirection::ClientBound },
	{ 0x1F676F8, PacketDirection::ServerBound },
	{ 0x1F67778, PacketDirection::ClientBound },
	{ 0x1F677F8, PacketDirection::ClientBound },
	{ 0x1F67878, PacketDirection::ClientBound },
	{ 0x1F678F8, PacketDirection::ClientBound },
	{ 0x1F67978, PacketDirection::ClientBound },
	{ 0x1F679F8, PacketDirection::ClientBound },
	{ 0x1F67A78, PacketDirection::ClientBound },
	{ 0x1F67AF8, PacketDirection::ClientBound },
	{ 0x1F67B78, PacketDirection::ClientBound },
	{ 0x1F67BE0, PacketDirection::ClientBound },
	{ 0x1F67C60, PacketDirection::ServerBound },
	{ 0x1F67CC8, PacketDirection::ClientBound },
	{ 0x1F67D48, PacketDirection::ClientBound },
	{ 0x1F67DC8, PacketDirection::ClientBound },
	{ 0x1F67E48, PacketDirection::ServerBound },
	{ 0x1F67EC8, PacketDirection::ClientBound },
	{ 0x1F68178, PacketDirection::ClientBound },
	{ 0x1F681F8, PacketDirection::ServerBound },
	{ 0x1F62F08, PacketDirection::ClientBound },
	{ 0x1F62F88, PacketDirection::ClientBound },
	{ 0x1F62FF0, PacketDirection::ClientBound },
	{ 0x1F63058, PacketDirection::ServerBound },
	{ 0x1F630D8, PacketDirection::ServerBound },
	{ 0x1F63140, PacketDirection::ServerBound },
	{ 0x1F631C0, PacketDirection::ServerBound },
	{ 0x1F68278, PacketDirection::ClientBound },
	{ 0x1F682F8, PacketDirection::ClientBound },
	{ 0x1F64330, PacketDirection::ClientBound },
	{ 0x1F643B0, PacketDirection::ServerBound },
	{ 0x1F64430, PacketDirection::ServerBound },
	{ 0x1F65CF0, PacketDirection::ServerBound },
	{ 0x1F65D70, PacketDirection::ClientBound },
	{ 0x1F65DF0, PacketDirection::Bidirectional },
	{ 0x1F69060, PacketDirection::ClientBound },
	{ 0x1F690C8, PacketDirection::ClientBound },
	{ 0x1F69130, PacketDirection::ServerBound },
	{ 0x1F69198, PacketDirection::ClientBound },
	{ 0x1F68478, PacketDirection::Bidirectional },
	{ 0x1F68570, PacketDirection::ClientBound },
	{ 0x1F685F0, PacketDirection::ClientBound },
	{ 0x1F68658, PacketDirection::ClientBound },
	{ 0x1F686C0, PacketDirection::Bidirectional },
	{ 0x1F6CB80, PacketDirection::Bidirectional },
	{ 0x1F6CC08, PacketDirection::Bidirectional },
	{ 0x1F6CC78, PacketDirection::ServerBound },
	{ 0x1F6CCE0, PacketDirection::ClientBound },
	{ 0x1F6CD48, PacketDirection::ClientBound },
	{ 0x1F6CDC8, PacketDirection::ClientBound },
	{ 0x1F6CE48, PacketDirection::ClientBound },
	{ 0x1F6CEC8, PacketDirection::ServerBound },
	{ 0x1F6CF48, PacketDirection::ServerBound },
	{ 0x1F6CFC8, PacketDirection::ServerBound },
	{ 0x1F6D048, PacketDirection::ServerBound },
	{ 0x1F6D0C8, PacketDirection::ServerBound },
	{ 0x1F6D148, PacketDirection::ClientBound },
	{ 0x1F6D1C8, PacketDirection::ClientBound },
	{ 0x1F6D248, PacketDirection::ClientBound },
	{ 0x1F6D2C8, PacketDirection::Bidirectional },
	{ 0x1F6D350, PacketDirection::ServerBound },
	{ 0x1F6D3D0, PacketDirection::Bidirectional },
	{ 0x1F6D458, PacketDirection::ClientBound },
	{ 0x1F6D4D8, PacketDirection::ClientBound },
	{ 0x1F6D558, PacketDirection::ServerBound },
	{ 0x1F6D5D8, PacketDirection::ClientBound },
	{ 0x1F6D658, PacketDirection::ServerBound },
	{ 0x1F6D6D8, PacketDirection::ServerBound },
	{ 0x1F6D758, PacketDirection::ClientBound },
	{ 0x1F6D7D8, PacketDirection::ClientBound },
	{ 0x1F6D858, PacketDirection::ServerBound },
	{ 0x1F6D8D8, PacketDirection::ServerBound },
	{ 0x1F6D958, PacketDirection::ServerBound },
	{ 0x1F6D9D8, PacketDirection::ServerBound },
	{ 0x1F6DA58, PacketDirection::ServerBound },
	{ 0x1F6DAD8, PacketDirection::ClientBound },
	{ 0x1F6DB58, PacketDirection::ServerBound },
	{ 0x1F6DBD8, PacketDirection::ClientBound },
	{ 0x1F6DC58, PacketDirection::ServerBound },
	{ 0x1F6DCD8, PacketDirection::ServerBound },
	{ 0x1F6DD58, PacketDirection::ClientBound },
	{ 0x1F6DDC0, PacketDirection::ServerBound },
	{ 0x1F6DE40, PacketDirection::ClientBound },
	{ 0x1F6DEA8, PacketDirection::ClientBound },
	{ 0x1F6DF10, PacketDirection::ServerBound },
	{ 0x1F6DF90, PacketDirection::ServerBound },
	{ 0x1F6E010, PacketDirection::ClientBound },
	{ 0x1F6E090, PacketDirection::ClientBound },
	{ 0x1F6E0F8, PacketDirection::ClientBound },
	{ 0x1F6E178, PacketDirection::ClientBound },
	{ 0x1F6E1F8, PacketDirection::ClientBound },
	{ 0x1F6E2D8, PacketDirection::ServerBound },
	{ 0x1F6E358, PacketDirection::ServerBound },
	{ 0x1F6E3D8, PacketDirection::ClientBound },
	{ 0x1F6E458, PacketDirection::ServerBound },
	{ 0x1F6E4C0, PacketDirection::ClientBound },
	{ 0x1F6E528, PacketDirection::ServerBound },
	{ 0x1F6E5A8, PacketDirection::ClientBound },
	{ 0x1F6E628, PacketDirection::ClientBound },
	{ 0x1F629F8, PacketDirection::ClientBound },
	{ 0x1F62A60, PacketDirection::ClientBound },
	{ 0x1F62990, PacketDirection::ClientBound },
	{ 0x1F68F78, PacketDirection::ServerBound },
	{ 0x1F68FE0, PacketDirection::ClientBound },
	{ 0x1F69A80, PacketDirection::ServerBound },
	{ 0x1F69AE8, PacketDirection::ServerBound },
	{ 0x1F69B68, PacketDirection::ServerBound },
	{ 0x1F69BD0, PacketDirection::ServerBound },
	{ 0x1F69C50, PacketDirection::ServerBound },
	{ 0x1F69CD0, PacketDirection::ServerBound },
	{ 0x1F69D38, PacketDirection::ServerBound },
	{ 0x1F69DA0, PacketDirection::ServerBound },
	{ 0x1F69E08, PacketDirection::ServerBound },
	{ 0x1F69E70, PacketDirection::ServerBound },
	{ 0x1F69ED8, PacketDirection::ServerBound },
	{ 0x1F69FC8, PacketDirection::ClientBound },
	{ 0x1F6A030, PacketDirection::ServerBound },
	{ 0x1F6A098, PacketDirection::ServerBound },
	{ 0x1F6A100, PacketDirection::ServerBound },
	{ 0x1F6A168, PacketDirection::ClientBound },
	{ 0x1F6A1D0, PacketDirection::ClientBound },
	{ 0x1F6A238, PacketDirection::ServerBound },
	{ 0x1F6A2A0, PacketDirection::ClientBound },
	{ 0x1F6A308, PacketDirection::ClientBound },
	{ 0x1F6A370, PacketDirection::ServerBound },
	{ 0x1F6A3D8, PacketDirection::ServerBound },
	{ 0x1F6A458, PacketDirection::ServerBound },
	{ 0x1F6A4C0, PacketDirection::ServerBound },
	{ 0x1F6A540, PacketDirection::ServerBound },
	{ 0x1F6A5A8, PacketDirection::ServerBound },
	{ 0x1F6A610, PacketDirection::ServerBound },
	{ 0x1F6A678, PacketDirection::ServerBound },
	{ 0x1F6A6F8, PacketDirection::ClientBound },
	{ 0x1F6A778, PacketDirection::ClientBound },
	{ 0x1F6A7F8, PacketDirection::ServerBound },
	{ 0x1F63978, PacketDirection::ClientBound },
	{ 0x1F639E0, PacketDirection::ClientBound },
	{ 0x1F63AA8, PacketDirection::ServerBound },
	{ 0x1F63B28, PacketDirection::ClientBound },
	{ 0x1F64F58, PacketDirection::ClientBound },
	{ 0x1F64FD8, PacketDirection::ClientBound },
	{ 0x1F65058, PacketDirection::ClientBound },
	{ 0x1F650D8, PacketDirection::ServerBound },
	{ 0x1F65158, PacketDirection::ServerBound },
	{ 0x1F651D8, PacketDirection::ServerBound },
	{ 0x1F65258, PacketDirection::ServerBound },
	{ 0x1F652C0, PacketDirection::ServerBound },
	{ 0x1F65340, PacketDirection::ClientBound },
	{ 0x1F653C0, PacketDirection::ServerBound },
	{ 0x1F65440, PacketDirection::ClientBound },
	{ 0x1F654C0, PacketDirection::ServerBound },
	{ 0x1F65540, PacketDirection::ServerBound },
	{ 0x1F655C0, PacketDirection::ServerBound },
	{ 0x1F65640, PacketDirection::ServerBound },
	{ 0x1F656C0, PacketDirection::ServerBound },
	{ 0x1F65740, PacketDirection::ServerBound },
	{ 0x1F657C0, PacketDirection::ServerBound },
	{ 0x1F65840, PacketDirection::ServerBound },
	{ 0x1F658C0, PacketDirection::ServerBound },
	{ 0x1F65928, PacketDirection::ServerBound },
	{ 0x1F659A8, PacketDirection::ServerBound },
	{ 0x1F65A28, PacketDirection::ServerBound },
	{ 0x1F65AA8, PacketDirection::ServerBound },
	{ 0x1F65B28, PacketDirection::ClientBound },
	{ 0x1F65BA8, PacketDirection::ClientBound },
	{ 0x1F65C10, PacketDirection::ClientBound },
	{ 0x1F68378, PacketDirection::ClientBound },
	{ 0x1F683F8, PacketDirection::ServerBound }
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
	return UnknownPacket;
}