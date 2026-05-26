#include "Core.h"

#include "Hooks/Hooks.h"
#include "Features/FeatureHandler.h"
#include "Features/ConfigHandler.h"
#include "Renderer/FrameBufferRenderer/FrameBuffers.h"

void ClientInitialize() {

    Shaders::initShaders();
    Fonts::initFonts();

    Renderer2D::InitRenderer();

    FrameBuffers::initFBOS();

    HookData::menu = std::make_unique<Menu>();

    FeatureHandler::Init();

    if (!ConfigHandler::FishDirectoryExists()) {
        ConfigHandler::CreateFishDirectory();
        ConfigHandler::SaveConfig("immediateConfig", false);
        ConfigHandler::CreateFriendsFile();
    }
    else {
        ConfigHandler::LoadConfig("immediateConfig", false);
    }
}
