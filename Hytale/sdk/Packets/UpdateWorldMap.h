/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include "sdk/BaseDataTypes/Object.h"
#include "Structs/IndependentStructs.h"
#include "../BaseDataTypes/Array.h"
#include "../BaseDataTypes/HytaleString.h"

struct UpdateWorldMap : Object {
	Array<MapChunk*>* chunks;
	Array<void*>* addedMarkers;
	Array<HytaleString>* removedMarkers;
};