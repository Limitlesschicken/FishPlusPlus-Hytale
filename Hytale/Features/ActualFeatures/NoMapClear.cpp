/*
 * Copyright (c) FishPlusPlus.
 */

#include "NoMapClear.h"

#include "sdk/Packets/UpdateWorldMap.h"
#include "Hooks/Hooks.h"

void NoMapClear::OnPacketRecieved(Object* packet, PacketIndex& index, bool& cancel) {
	if (index != PacketIndex::UpdateWorldMap_S2C)
		return;

	UpdateWorldMap* worldmapPacket = (UpdateWorldMap*)packet;

	if (!worldmapPacket)
		return;

	if (!worldmapPacket->chunks)
		return;
	
	
	for (int i = 0; i < worldmapPacket->chunks->count; i++) {
		MapChunk* chunk = worldmapPacket->chunks->get(i);
		if (!chunk)
			continue;
		
		bool exists = false;
		for (MapChunk* savedChunk : this->mapChunks) {
			if (chunk->chunkX == savedChunk->chunkX && chunk->chunkY == savedChunk->chunkY) {
				exists = true;
				break;
			}
		}
		if (!exists) {
			MapChunk* newChunk = new MapChunk();

			*newChunk = *chunk;
			newChunk->image = new MapImage();
			*newChunk->image->packed_indices = *chunk->image->packed_indices;
			*newChunk->image->palette = *chunk->image->palette;

			this->mapChunks.push_back(newChunk);
		}
	}
	

	if (mapChunks.empty())
		return;

	
	Array<MapChunk*>* newChunks = API::RHPNewArray<Array<MapChunk*>*>(SM::Array_MapChunk_MTAddress, mapChunks.size());

	MapChunk* allocated = API::RHPNewFast<MapChunk*>(SM::MapChunk_MTAddress);
	for (int i = 0; i < mapChunks.size(); i++) {
		MapChunk* saved = mapChunks[i];

		allocated->chunkX = saved->chunkX;
		allocated->chunkY = saved->chunkY;
		allocated->image = saved->image;

		if (!newChunks)
			return;

		newChunks->list[i] = allocated;
	}



	
	UpdateWorldMap* newPacket = CreatePacket<UpdateWorldMap*>(PacketIndex::UpdateWorldMap_S2C);

	newPacket->chunks = newChunks;
	newPacket->addedMarkers = worldmapPacket->addedMarkers;
	newPacket->removedMarkers = worldmapPacket->removedMarkers;

	Hooks::oProcessPacket(Util::getGameInstance()->PacketHandler, newPacket);

	cancel = true;
	
	
	
	//worldmapPacket->chunks = newChunks;
	//packet = worldmapPacket;
	
	
}

void NoMapClear::Initialize() {
	Util::log("Initialized No Map Clear feature");
	RegisterEvent(this);
}