/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include "core.h"
#include "Math/Vector3.h"

struct LookupEntry {
    bool valid;
    const char* name;
    Color color;
};

namespace HookData {
    inline bool initialized = false;
    inline bool initialized3D = false;

    inline int oldWindowWidth = 0;
    inline int oldWindowHeight = 0;

    inline std::unique_ptr<Menu> menu;
    inline std::unique_ptr<FramebufferRenderer> fboRenderer;

	inline bool queueTeleport = false;
	inline Vector3 teleportTarget = Vector3(0, 0, 0);
    
	inline void* PlaceBlockInteraction = nullptr;
    inline void* Context = nullptr;

    inline std::vector<LookupEntry> lookupTable;
    inline std::vector<int> allTargetBlockIds;
    inline std::vector<RenderBlockInfo> ImportantBlocks = {
        { { }, "Chest", Color::Normalize(0, 255, 255), 0 },
        { { }, "Bench", Color::Normalize(0, 255, 255), 1 },
        { { }, "Adamantite", Color::Normalize(255, 0, 255), 2 },
        { { }, "Gold", Color::Normalize(255, 0, 255), 3 },
        { { }, "Silver", Color::Normalize(255, 0, 255), 4 },
        { { }, "Iron", Color::Normalize(255, 0, 255), 5 },
        { { }, "Copper", Color::Normalize(255, 0, 255), 6 },
        { { }, "Mithril", Color::Normalize(255, 0, 255), 7 },
        { { }, "Cobalt", Color::Normalize(255, 0, 255), 8 },
        { { }, "Thorium", Color::Normalize(255, 0, 255), 9 },
        { { }, "Treasure", Color::Normalize(255, 0, 255), 10 },
        { { }, "Crystal", Color::Normalize(255, 0, 255), 11 },
        { { }, "Gem", Color::Normalize(255, 0, 255), 12 }
    };
}

using namespace HookData;

namespace Hooks {
    typedef BOOL(WINAPI* WglSwapBuffers)(HDC hdc);
    inline WglSwapBuffers oWglSwapBuffers = nullptr;
    extern BOOL WINAPI hkWglSwapBuffers(HDC hdc);

	typedef void(__fastcall* DoMoveCycle)(DefaultMovementController* dmc, Vector3 offset);
	inline DoMoveCycle oDoMoveCycle = nullptr;
	extern void __fastcall hkDoMoveCycle(DefaultMovementController* dmc, Vector3 offset);

	typedef void(__fastcall* HandleScreenShotting)(App* app);
	inline HandleScreenShotting oHandleScreenShotting = nullptr;
	extern void __fastcall hkHandleScreenShotting(App* app);

	typedef void(__fastcall* OnUserInput)(uint64_t instance, SDL_Event event);
	inline OnUserInput oOnUserInput = nullptr;
	extern void __fastcall hkOnUserInput(uint64_t instance, SDL_Event event);

	typedef void(__fastcall* WeatherUpdate)(uint64_t instance, float deltaTime);
	inline WeatherUpdate oWeatherUpdate = nullptr;
	extern void __fastcall hkWeatherUpdate(uint64_t instance, float deltaTime);

    typedef void(__fastcall* TimeModuleNewFrame)(void* instance, float deltaTime);
    inline TimeModuleNewFrame oTimeModuleNewFrame = nullptr;
    extern void __fastcall hkTimeModuleNewFrame(void* instance, float deltaTime);

	typedef void(__fastcall* OnChat)(uint64_t instance, HytaleString* chatString);
	inline OnChat oOnChat = nullptr;
	extern void __fastcall hkOnChat(uint64_t instance, HytaleString* chatString);

	typedef void(__fastcall* DrawPostEffect)(GameInstance* instance);
	inline DrawPostEffect oDrawPostEffect = nullptr;
	extern void __fastcall hkDrawPostEffect(GameInstance* instance);

    typedef void(__fastcall* DrawEntityCharactersAndItems)(SceneRenderer* instance, bool useOcclusionCulling);
	inline DrawEntityCharactersAndItems oDrawEntityCharactersAndItems = nullptr;
	extern void __fastcall hkDrawEntityCharactersAndItems(SceneRenderer* instance, bool useOcclusionCulling);

	typedef void(__fastcall* BuildGeometry)(void* instance, ChunkColumn* a2, int chunkX, int chunkY, int chunkZ, int64_t a6, int64_t a7, int64_t a8, int64_t a9, int64_t a10, int64_t a11, int64_t a12, int a13, int a14, int64_t* a15);
	inline BuildGeometry oBuildGeometry = nullptr;
	extern void __fastcall hkBuildGeometry(void* instance, ChunkColumn* a2, int chunkX, int chunkY, int chunkZ, int64_t a6, int64_t a7, int64_t a8, int64_t a9, int64_t a10, int64_t a11, int64_t a12, int a13, int a14, int64_t* a15);

    typedef void(__fastcall* temp)(void* instance, void* object);
    inline temp otemp = nullptr;
    extern void __fastcall hktemp(void* instance, void* object);

    typedef void(__fastcall* ProcessPacket)(void* instance, Object* packet);
    inline ProcessPacket oProcessPacket = nullptr;
    extern void __fastcall hkProcessPacket(void* instance, Object* packet);

    typedef void*(__fastcall* SocketSend)(void* instance, void* error, void* byteArray, char socketFlags, void* param5);
    inline SocketSend oSocketSend = nullptr;
    extern void* __fastcall hkSocketSend(void* instance, void* error, void* byteArray, char socketFlags, void* param5);

	bool CreateHooks();
};    