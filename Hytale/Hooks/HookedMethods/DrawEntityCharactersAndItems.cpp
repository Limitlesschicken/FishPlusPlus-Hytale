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
inline void renderQueueFlush(uint64_t queue, uint64_t renderContext) {
    sub_offset<void(__fastcall*)(uint64_t, uint64_t)>(SM::renderQueueFlushAddress)(queue, renderContext);
}
inline void submitDrawCommands() {
    sub_offset<void(__fastcall*)()>(SM::submitDrawCommandsAddress)();
}

#pragma optimize("", off)
#pragma runtime_checks("", off)

/*
__declspec(safebuffers) __declspec(noinline)
void renderIndividualEntity(SceneRenderer* _this, int i) {
    SceneContext* sceneContext = _this->contextContainer->sceneContext;
    RenderStats* renderStats = _this->renderDevice->renderStats;
    EntityList* entityList = _this->entityList;
    UniformManager* uniformMgrPtr = *(UniformManager**)(SM::g_UniformManagerAddress);
    BufferManager* bufferMgrPtr = *(BufferManager**)(SM::g_BufferManagerAddress);

    auto setShaderUniform = (void(__fastcall*)(uint64_t, uint64_t, uint64_t))(uniformMgrPtr->vtable->setShaderUniform);
    auto bindUniformBufferRange = (void(__fastcall*)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t))(bufferMgrPtr->vtable->bindUniformBufferRange);

    if (i >= entityList->count)
        return;
    EntityDrawTask* entityDrawTask = &entityList->entities[i];

    if (*(&SM::g_GlobalStateTableAddress - 1))
        submitDrawCommands();

    //beginGLContext(glStateBuffer);
    setShaderUniform(sceneContext->shaderProgramId, 0, (uint32_t)i);
    bindUniformBufferRange(35345, sceneContext->uniformBufferId, entityDrawTask->AnimationData.InternalId, entityDrawTask->AnimationDataOffset, entityDrawTask->AnimationDataSize);
    //endGLContext(glStateBuffer);

    glBindVertexArray(entityDrawTask->VertexArray.InternalId);
    ++renderStats->drawCallCount;
    renderStats->totalIndicesDrawn += entityDrawTask->DataCount;
    glDrawElements(GL_TRIANGLES, entityDrawTask->DataCount, GL_UNSIGNED_SHORT, 0);
}
*/


__declspec(safebuffers) __declspec(noinline)
void originalDrawEntityCharactersAndItems(SceneRenderer* _this) {
    /*
    EntityList* entityList = _this->entityList;

    //Render entities normally but through walls
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, -1500000.0f);
    for (int i = 0; i < _this->_entityDrawTaskCount; i++) {
        renderIndividualEntity(_this, i);
    }
    glPolygonOffset(1.0f, 1500000.0f);
    glDisable(GL_POLYGON_OFFSET_FILL);

    //Render entities for outline/glow
    FrameBuffers::entityOutlineFBO->bind();
    for (int i = 0; i < _this->_entityDrawTaskCount; i++) {
        EntityDrawTask* entityDrawTask = &entityList->entities[i];
        Entity* entityToRender = Util::app->appInGame->gameInstance->EntityStoreModule->entityArray->get(entityDrawTask->EntityLocalId);
        if (entityToRender->entityType == Entity::EntityType::Character)
            renderIndividualEntity(_this, i);
    }
    FrameBuffers::entityOutlineFBO->unbind();

    //Render items for outline/glow
    FrameBuffers::itemOutlineFBO->bind();
    for (int i = 0; i < _this->_entityDrawTaskCount; i++) {
        EntityDrawTask* entityDrawTask = &entityList->entities[i];
        Entity* entityToRender = Util::app->appInGame->gameInstance->EntityStoreModule->entityArray->get(entityDrawTask->EntityLocalId);
        if (entityToRender->entityType == Entity::EntityType::Item)
            renderIndividualEntity(_this, i);
    }
    FrameBuffers::itemOutlineFBO->unbind();
    */
}

__declspec(safebuffers) __declspec(noinline)
void __fastcall Hooks::hkDrawEntityCharactersAndItems(SceneRenderer* _this, bool flag) {
    if (!Util::IsInGame())
        return Hooks::oDrawEntityCharactersAndItems(_this, flag);


    Feature* outline = FeatureHandler::GetFeatureFromName("Outline");
    if (!outline)
        return;
    if(outline->IsActive())
        originalDrawEntityCharactersAndItems(_this);
    else 
		Hooks::oDrawEntityCharactersAndItems(_this, flag);
    


}
#pragma runtime_checks("", restore)
#pragma optimize("", on)

