/*
 * Copyright (c) FishPlusPlus.
 */
#include "../Hooks.h"

#include "Renderer/FrameBufferRenderer/FrameBuffers.h"
#include "Features/FeatureHandler.h"

#include <cstdint>
#include <windows.h>

// lazy resolver
template<typename T>
T sub_offset(uintptr_t address) {
    static T fn = nullptr;
    if (!fn)
        fn = reinterpret_cast<T>(address);
    return fn;
}

inline void beginGLContext(void* stateBuffer) {
    sub_offset<void(__fastcall*)(void*)>(SM::beginGLContextAddress)(stateBuffer);
}
inline void endGLContext(void* stateBuffer) {
    sub_offset<void(__fastcall*)(void*)>(SM::endGLContextAddress)(stateBuffer);
}

inline void renderGeometry(EntityDrawData* a1, int uniformBufferId, int animationId, int unkValue, int unkValue2, int unkValue3, uint64_t unkValue4 = 0, uint64_t unkValue5 = 0, uint64_t unkValue6 = 0, uint64_t unkValue7 = 0) {
    return sub_offset<void(__fastcall*)(EntityDrawData*, int, int, int, int, int, uint64_t, uint64_t, uint64_t, uint64_t)>(SM::drawGeometryAddress)(a1, uniformBufferId, animationId, unkValue, unkValue2, unkValue3, unkValue4, unkValue5, unkValue6, unkValue7);
}


#pragma optimize("", off)
#pragma runtime_checks("", off)
__declspec(safebuffers) __declspec(noinline)
void renderIndividualEntity(SceneRenderer* _this, EntityDrawTask entityDrawTask, int entityIndex) {
    
    GPUProgramStore* sceneContext = _this->contextContainer->GPUProgramStore;
    GraphicsDevice* renderStats = _this->graphicsDevice->graphicsDevice;
   
    
    UniformManager* uniformMgrPtr = *(UniformManager**)(SM::g_UniformManagerAddress);
    BufferManager* bufferMgrPtr = *(BufferManager**)(SM::g_BufferManagerAddress);
    

    if (entityIndex >= _this->EntityDrawTask1->count)
        return;
    
    uintptr_t v27 = *(uintptr_t*)(*(uintptr_t*)((uintptr_t)entityDrawTask.drawData + 0x20) + 0x10);

    for (int i = 0; i < entityDrawTask.drawData->AnimationData->count; i++) {
        auto bindUniformBufferRange = (void(__fastcall*)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t))(bufferMgrPtr->vtable->bindUniformBufferRange);

        if (i >= entityDrawTask.drawData->AnimationSizeData->count)
            return;

        if (i >= entityDrawTask.drawData->AnimationData->count)
            return;
        

        int AnimationOffset = entityDrawTask.drawData->animationOffset;
		int AnimationSize = 0;

        AnimationOffset = (int)((entityDrawTask.drawData->AnimationData->get(i) << 6) + AnimationOffset);
        AnimationSize = entityDrawTask.drawData->AnimationSizeData->get(i) << 6;

        
        //beginGLContext(glStateBuffer);
		//Util::log("Uniform Buffer ID: %i, Animation ID: %i, Animation Offset: %i, Animation Size: %i", sceneContext->uniformBufferId, entityDrawTask.AnimationId, AnimationOffset, AnimationSize);
        
        bindUniformBufferRange(35345, sceneContext->uniformBufferId, entityDrawTask.AnimationId, AnimationOffset, AnimationSize);
        //endGLContext(glStateBuffer);

        
		int dataCount = entityDrawTask.drawData->drawCallArray->get(i);
        int vertexId = entityDrawTask.drawData->vertexArrayArray->get(i);

        glBindVertexArray(vertexId);
        *(uint32_t*)(v27 + 0x468) += 1;
        *(uint32_t*)(v27 + 0x46C) += dataCount;
        glDrawElements(GL_TRIANGLES, dataCount, GL_UNSIGNED_SHORT, 0);
    }
}

__declspec(safebuffers) __declspec(noinline)
void originalDrawEntityCharactersAndItems(SceneRenderer* _this) {
    Array<EntityDrawTask>* drawTasks = _this->EntityDrawTask1;

    GPUProgramStore* sceneContext = _this->contextContainer->GPUProgramStore;
    GraphicsDevice* renderStats = _this->graphicsDevice->graphicsDevice;


    UniformManager* uniformMgrPtr = *(UniformManager**)(SM::g_UniformManagerAddress);
    BufferManager* bufferMgrPtr = *(BufferManager**)(SM::g_BufferManagerAddress);

    //Render entities normally but through walls

    for (int i = 0; i < _this->entityDrawCount; i++) {
        auto setShaderUniform = (void(__fastcall*)(uint64_t, uint64_t, uint64_t))(uniformMgrPtr->vtable->setShaderUniform);
        setShaderUniform(sceneContext->shaderProgramId, 0, i);

        EntityDrawTask entityDrawTask = drawTasks->get(i);

        uint64_t v14 = ((uint64_t)sceneContext->unkValue4 << 32) | 1;
        uint64_t v15 = ((uint64_t)sceneContext->unkValue5 << 32) | 1;

        uint64_t value = 0xCCCCCCCC00000001ULL;
        uint64_t value2 = 0xCCCCCCCC00000000ULL;

        Util::log("My passed data: a1: %llx, uniformBufferId: %i, animationId: %i, unkValue: %i, unkValue2: %i, unkValue3: %i, unkValue4: %llx, unkValue5: %llx, unkValue6: %llx, unkValue7: %llx", entityDrawTask.drawData, sceneContext->uniformBufferId, entityDrawTask.AnimationId, sceneContext->unkValue, sceneContext->unkValue2, sceneContext->unkValue3, value, value, value2, value2);
		renderGeometry(entityDrawTask.drawData, sceneContext->uniformBufferId, entityDrawTask.AnimationId, sceneContext->unkValue, sceneContext->unkValue2, sceneContext->unkValue3, value, value, value2, value2);
		//renderIndividualEntity(_this, entityDrawTask, i);
    }


    /*
    //Render entities for outline/glow
    //FrameBuffers::entityOutlineFBO->bind();
    for (int i = 0; i < _this->entityDrawCount; i++) {
        EntityDrawTask entityDrawTask = drawTasks->get(i);
        Entity* entityToRender = Util::app->appInGame->gameInstance->EntityStoreModule->entityArray->get(entityDrawTask.entityLocalId);
        if (entityToRender->entityType == Entity::EntityType::Character)
			renderGeometry(entityDrawTask.drawData, _this->contextContainer->GPUProgramStore->uniformBufferId, entityDrawTask.drawData->AnimationId, _this->contextContainer->GPUProgramStore->unkValue, _this->contextContainer->GPUProgramStore->unkValue2);
            //renderIndividualEntity(_this, entityDrawTask, i);
    }
    //FrameBuffers::entityOutlineFBO->unbind();

    //Render items for outline/glow
    FrameBuffers::itemOutlineFBO->bind();
    for (int i = 0; i < _this->entityDrawCount; i++) {
        EntityDrawTask entityDrawTask = drawTasks->get(i);
        Entity* entityToRender = Util::app->appInGame->gameInstance->EntityStoreModule->entityArray->get(i);
        //if (entityToRender->entityType == Entity::EntityType::Item)
            //renderIndividualEntity(_this, entityDrawTask, i);
    }
    FrameBuffers::itemOutlineFBO->unbind();
    */
}

__declspec(safebuffers) __declspec(noinline)
void __fastcall Hooks::hkDrawEntityCharactersAndItems(SceneRenderer* _this, bool useOcclusionCulling) {
    if (!Util::IsInGame())
        return Hooks::oDrawEntityCharactersAndItems(_this, useOcclusionCulling);

    Feature* outline = FeatureHandler::GetFeatureFromName("Outline");
    if (!outline) {
        Hooks::oDrawEntityCharactersAndItems(_this, useOcclusionCulling);
        return;
    }
    if (outline->IsActive()) {
        originalDrawEntityCharactersAndItems(_this);
        Hooks::oDrawEntityCharactersAndItems(_this, useOcclusionCulling);
        return;
    }
    
    Hooks::oDrawEntityCharactersAndItems(_this, useOcclusionCulling);

}

__declspec(safebuffers) __declspec(noinline)
void __fastcall Hooks::hkDrawEntity(EntityDrawData* a1, uint64_t a2,
    uint64_t a3,
    uint64_t a4,
    uint64_t a5,
    uint64_t a6,
    uint64_t a7,
    uint64_t a8,
    uint64_t a9,
    uint64_t a10) {
    //Util::log("unkValue5: %llx, unkValue6: %llx", a7, a8);
	Util::log("Game passed data: a1: %llx, uniformBufferId: %i, animationId: %i, unkValue: %i, unkValue2: %i, unkValue3: %i, unkValue4: %llx, unkValue5: %llx,  unkValue6: %llx,  unkValue7: %llx", a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
    Hooks::oDrawEntity(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}
#pragma runtime_checks("", restore)
#pragma optimize("", on)

