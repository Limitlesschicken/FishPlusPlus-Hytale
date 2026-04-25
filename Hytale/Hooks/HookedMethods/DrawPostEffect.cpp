/*
 * Copyright (c) FishPlusPlus.
 */
#include "../Hooks.h"
#include "Events/EventRegister.h"

#include "Renderer/FrameBufferRenderer/FrameBuffers.h"

#include "Features/FeatureHandler.h"
#include "Features/ActualFeatures/Outline.h"
#include "Features/ActualFeatures/BlockESP.h"

#pragma optimize("", off)
#pragma runtime_checks("", off)

__declspec(safebuffers) __declspec(noinline)
void __fastcall Hooks::hkDrawPostEffect(GameInstance* instance) {
	Hooks::oDrawPostEffect(instance);
	if (!initialized)
		return;

	Renderer3D renderer3D;
	EventRegister::Render3DEvent.Invoke(renderer3D);

    bool ShowBlockAtFeet = false;
    if (ShowBlockAtFeet && Util::app->Stage == AppStage::InGame) {
        Vector3 renderPos = Util::getLocalPlayer()->Position.toFloor();
		renderPos.y -= 1; // Adjust to get the block the player is standing on
        ClientBlockType* block = Util::getGameInstance()->MapModule->GetBlockType(renderPos);
        renderer3D.BoxLines(Vector3(renderPos.x, renderPos.y, renderPos.z), Vector3(1.0f, 1.0f, 1.0f), Color::Normalize(255, 0, 0, 50), Color::Normalize(Color::Red()));
        Vector2 screenPos;
        if (Util::WorldToScreen(Vector3(renderPos.x + 0.5f, renderPos.y + 0.5f, renderPos.z + 0.5f), screenPos))
		    Fonts::Figtree->RenderText(Util::string_format("%s (%i)", block->Name->getString().c_str(), block->Id), screenPos.x, screenPos.y, 1, Color::White());

        
/*		DBGBlockData dbgData = Util::getGameInstance()->MapModule->GetBlockData((int) floor(playerPos.x), (int) floor(playerPos.y) - 1, (int) floor(playerPos.z));
        if (Util::WorldToScreen(Vector3(renderPos.x + 0.5f, renderPos.y + 0.5f, renderPos.z + 0.5f), screenPos)) {
			std::string debugText = Util::string_format("%s (ID: %i)", dbgData.blockName.c_str(), dbgData.BlockID);
            Fonts::Figtree->RenderText(debugText, screenPos.x, screenPos.y, 1, Color::White());
			std::string debugText2 = Util::string_format("Packed Pos: %i, Unpacked Pos: (%.1f, %.1f, %.1f)", dbgData.packedPos, dbgData.unpackedPos.x, dbgData.unpackedPos.y, dbgData.unpackedPos.z);
			Fonts::Figtree->RenderText(debugText2, screenPos.x, screenPos.y + 20, 1, Color::White());
			std::string debugText3 = Util::string_format("Packing Type: %s", dbgData.packingType == BitPackingType::Packed4Bit ? "Packed4Bit" : dbgData.packingType == BitPackingType::Packed8Bit ? "Packed8Bit" : "Packed16Bit");
			Fonts::Figtree->RenderText(debugText3, screenPos.x, screenPos.y + 40, 1, Color::White());
			std::string debugText4 = Util::string_format("Palette Address: 0x%llX", dbgData.paletteAddress);
			Fonts::Figtree->RenderText(debugText4, screenPos.x, screenPos.y + 60, 1, Color::White());
        }*/

    }

    renderer3D.Render();
    

    Outline* outline = static_cast<Outline*>(FeatureHandler::GetFeatureFromName("Outline"));
    

    if (outline->IsActive() && outline->CanExecute()) {
        RecursiveSetting* entities = static_cast<RecursiveSetting*>(outline->GetSettingFromName("Entities"));
        if (entities->GetValue()) {
            bool glow = static_cast<ToggleSetting*>(entities->GetSettingFromName("Glow"))->GetValue();
            float glowSize = static_cast<SliderSetting*>(entities->GetSettingFromName("Glow Size"))->GetValue();
            Color color = static_cast<ColorSetting*>(entities->GetSettingFromName("Color"))->GetValue();
            bool wave = static_cast<ToggleSetting*>(entities->GetSettingFromName("Wave"))->GetValue();

            OutlineFboRenderer::OutlineUniforms uniforms(Color::Normalize(color), glow, (int)glowSize, wave);
            FrameBuffers::entityOutlineFBO->setupPass(uniforms);
            FrameBuffers::entityOutlineFBO->draw();
        }
        RecursiveSetting* items = static_cast<RecursiveSetting*>(outline->GetSettingFromName("Items"));
        if (items->GetValue()) {
            bool glow = static_cast<ToggleSetting*>(items->GetSettingFromName("Glow"))->GetValue();
            float glowSize = static_cast<SliderSetting*>(items->GetSettingFromName("Glow Size"))->GetValue();
            Color color = static_cast<ColorSetting*>(items->GetSettingFromName("Color"))->GetValue();
			bool wave = static_cast<ToggleSetting*>(items->GetSettingFromName("Wave"))->GetValue();

            OutlineFboRenderer::OutlineUniforms uniforms(Color::Normalize(color), glow, (int)glowSize, wave);
            FrameBuffers::itemOutlineFBO->setupPass(uniforms);
            FrameBuffers::itemOutlineFBO->draw();
        }
    }
}
#pragma runtime_checks("", restore)
#pragma optimize("", on)