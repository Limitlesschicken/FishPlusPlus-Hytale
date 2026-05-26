#pragma once
#include <stdint.h>

namespace SM {
	inline uint64_t SetCursorHiddenAddress = 0;
	inline uint64_t UpdateInputStatesAddress = 0;
	inline uint64_t SendPacketImmediateAddress = 0;
	inline uint64_t GCToEEInterface_CreateThreadAddress = 0;
	inline uint64_t Object_ToStringAddress = 0;
	inline uint64_t ConcurrentDictionary_TryGetValueAddress = 0;

	inline uint64_t beginGLContextAddress = 0;
	inline uint64_t endGLContextAddress = 0;
	inline uint64_t renderQueueFlushAddress = 0;
	inline uint64_t submitDrawCommandsAddress = 0;
	inline uint64_t g_UniformManagerAddress = 0;
	inline uint64_t g_BufferManagerAddress = 0;
	inline uint64_t g_GlobalStateTableAddress = 0;

	inline uint64_t RhpNewFastAddress = 0;
	inline uint64_t RhpNewArray_GenericAddress = 0;
	inline uint64_t SetClientBlockAddress = 0;

	inline uint64_t Array_SyncInteractionChain_MTAddress = 0;
	inline uint64_t SyncInteractionChain_MTAddress = 0;
	inline uint64_t InteractionChainData_MTAddress = 0;
	inline uint64_t BlockPosition_MTAddress = 0;
	inline uint64_t Array_InteractionSyncData_MTAddress = 0;
	inline uint64_t InteractionSyncData_MTAddress = 0;
}