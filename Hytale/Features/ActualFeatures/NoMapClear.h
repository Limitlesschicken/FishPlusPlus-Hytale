/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "Features/Feature.h"

#include "sdk/Packets/UpdateWorldMap.h"

class NoMapClear : public Feature {
public:
	NoMapClear() : Feature("No Map Clear") {};

	void OnPacketRecieved(Object* packet, PacketIndex& index, bool& cancel);

	void Initialize() override;

private:

	std::vector<MapChunk*> mapChunks;
};