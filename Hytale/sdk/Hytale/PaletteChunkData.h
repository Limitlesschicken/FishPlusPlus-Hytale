#pragma once
#include <vector>
#include <unordered_set>

/*
* Chunk size: 32x32x32 = 32,768 blocks
* 
* blockData->count == 16384: 4-bit packing (2 blocks per byte, palette indices 0-15)
* blockData->count == 32768: 8-bit packing (1 block per byte, palette indices 0-255)
* blockData->count == 65536: 16-bit packing (1 block per 2 bytes, palette indices 0-65535)
* 
* Selection logic:
*   - If blockData->count == 16384: Use 4-bit extraction
*   - Else if internalToExternal->count > 256: Use 16-bit (short) packing
*   - Else: Use 8-bit (byte) packing
*/

struct FishBlockData {
	int x, y, z;
	int blockId;

	FishBlockData(int _x, int _y, int _z, int _blockId) : x(_x), y(_y), z(_z), blockId(_blockId) {}
};

struct IChunkData {
	void* m_table;										// 0x00
	void* field_0x8;									// 0x08
	Array<int>* internalToExternal;						// 0x10 System.Int32[]
	char pad_0x10[0x10];                                // 0x18-0x28

	bool isEmpty() {
		return internalToExternal == nullptr || internalToExternal->count == 0;
	}

	int GetBlockDataCount() {
		uint64_t arrayBase = *(uint64_t*)((uint64_t)this + 0x28);
		if (arrayBase == 0) return 0;
		return *(int*)(arrayBase + 0x8);
	}

	bool Is4BitPacked() {
		return GetBlockDataCount() == 16384;
	}
};

struct Abstract4BitPaletteChunkData : IChunkData {
	Array<uint8_t>* blockData;								// 0x28 System.Byte[] (4-bit packed, 2 per byte)
};

struct Abstract8BitPaletteChunkData : IChunkData {
	Array<uint8_t>* blockData;								// 0x28 System.Byte[] (1 per byte)
};

struct Abstract16BitPaletteChunkData : IChunkData {
	Array<uint16_t>* blockData;								// 0x28 System.UInt16[] (1 per short)
};

struct PaletteChunkData {
	void* m_table;										// 0x00
	IChunkData* chunkSection;							// 0x08

	int GetPackingType() {
		if (chunkSection == nullptr || chunkSection->isEmpty())
			return -1; // Undefined
		int blockDataCount = chunkSection->GetBlockDataCount();
		if (blockDataCount == 16384)
			return 0; // 4-bit
		else if (chunkSection->internalToExternal->count > 256)
			return 2; // 16-bit
		else
			return 1; // 8-bit
	}

	int GetBlockID(int32_t idx, int undefinedBlockId) {
		if (chunkSection == nullptr || chunkSection->isEmpty())
			return undefinedBlockId;

		int blockDataCount = chunkSection->GetBlockDataCount();
		int paletteCount = chunkSection->internalToExternal->count;
		//printf("Getting block ID at index %d (blockData count: %d, palette count: %d)", idx, blockDataCount, paletteCount);
		int paletteIdx = 0;
		if (blockDataCount == 16384) {
			Abstract4BitPaletteChunkData* data4bit = (Abstract4BitPaletteChunkData*)chunkSection;
			uint8_t byteVal = data4bit->blockData->get(idx / 2);
			paletteIdx = (idx % 2 == 0) ? ((byteVal >> 4) & 0x0F) : (byteVal & 0x0F);
			//paletteIdx = (idx % 2 == 0) ? (byteVal & 0x0F) : ((byteVal >> 4) & 0x0F);
		} else {
			if (paletteCount > 256) {
				Abstract16BitPaletteChunkData* data16bit = (Abstract16BitPaletteChunkData*)chunkSection;
				paletteIdx = data16bit->blockData->get(idx);
			} else {
				Abstract8BitPaletteChunkData* data8bit = (Abstract8BitPaletteChunkData*)chunkSection;
				paletteIdx = data8bit->blockData->get(idx);
			}
		}

		if (paletteIdx >= paletteCount) {
			return undefinedBlockId;
		}

		return chunkSection->internalToExternal->get(paletteIdx);
	}

	std::vector<FishBlockData> FindBlocks(const std::vector<int>& targetBlockIds) {
		std::vector<FishBlockData> results;

		if (chunkSection == nullptr || chunkSection->isEmpty())
			return results;

		std::unordered_set<int> targetSet(targetBlockIds.begin(), targetBlockIds.end());

		const int CHUNK_SIZE = 32;
		const int TOTAL_BLOCKS = 32768;

		int blockDataCount = chunkSection->GetBlockDataCount();
		int paletteCount = chunkSection->internalToExternal->count;
		Array<int>* palette = chunkSection->internalToExternal;

		if (blockDataCount == 16384) {
			Abstract4BitPaletteChunkData* data4bit = (Abstract4BitPaletteChunkData*)chunkSection;
			Array<uint8_t>* blockData = data4bit->blockData;

			// IDX == ((y & 0x1F) << 10) | ((z & 0x1F) << 5) | (x & 0x1F);

			for (int idx = 0; idx < TOTAL_BLOCKS; idx++) {
				uint8_t byteVal = blockData->getUnsafe(idx / 2);
				int paletteIdx = (idx % 2 == 0) ? ((byteVal >> 4) & 0x0F) : (byteVal & 0x0F);

				if (paletteIdx >= paletteCount)
					continue;

				int blockId = palette->getUnsafe(paletteIdx);

				if (targetSet.find(blockId) != targetSet.end()) {
					int x = idx & 0x1F;                      // bits 0-4
					int z = (idx >> 5) & 0x1F;               // bits 5-9
					int y = (idx >> 10) & 0x1F;              // bits 10-14
					results.push_back(FishBlockData(x, y, z, blockId));
				}
			}
		} else if (paletteCount > 256) {
			Abstract16BitPaletteChunkData* data16bit = (Abstract16BitPaletteChunkData*)chunkSection;
			Array<uint16_t>* blockData = data16bit->blockData;

			for (int idx = 0; idx < TOTAL_BLOCKS; idx++) {
				int paletteIdx = blockData->getUnsafe(idx);

				if (paletteIdx >= paletteCount)
					continue;

				int blockId = palette->getUnsafe(paletteIdx);

				if (targetSet.find(blockId) != targetSet.end()) {
					int x = idx & 0x1F;                      // bits 0-4
					int z = (idx >> 5) & 0x1F;               // bits 5-9
					int y = (idx >> 10) & 0x1F;              // bits 10-14
					results.push_back(FishBlockData(x, y, z, blockId));
				}
			}
		} else {
			Abstract8BitPaletteChunkData* data8bit = (Abstract8BitPaletteChunkData*)chunkSection;
			Array<uint8_t>* blockData = data8bit->blockData;

			for (int idx = 0; idx < TOTAL_BLOCKS; idx++) {
				int paletteIdx = blockData->getUnsafe(idx);

				if (paletteIdx >= paletteCount)
					continue;

				int blockId = palette->getUnsafe(paletteIdx);

				if (targetSet.find(blockId) != targetSet.end()) {
					int x = idx & 0x1F;                      // bits 0-4
					int z = (idx >> 5) & 0x1F;               // bits 5-9
					int y = (idx >> 10) & 0x1F;              // bits 10-14
					results.push_back(FishBlockData(x, y, z, blockId));
				}
			}
		}

		return results;
	}

	// Scan only a specific section of the chunk (for incremental scanning)
	// All coordinates are in local chunk space (0-31)
	std::vector<FishBlockData> FindBlocksInSection(const std::vector<int>& targetBlockIds,
													 int minX, int maxX,
													 int minY, int maxY,
													 int minZ, int maxZ) {
		std::vector<FishBlockData> results;

		if (chunkSection == nullptr || chunkSection->isEmpty())
			return results;

		std::unordered_set<int> targetSet(targetBlockIds.begin(), targetBlockIds.end());

		const int CHUNK_SIZE = 32;

		int blockDataCount = chunkSection->GetBlockDataCount();
		int paletteCount = chunkSection->internalToExternal->count;
		Array<int>* palette = chunkSection->internalToExternal;

		// Clamp bounds to valid range
		if (minX < 0) minX = 0;
		if (minX > 31) minX = 31;
		if (maxX < 0) maxX = 0;
		if (maxX > 31) maxX = 31;
		if (minY < 0) minY = 0;
		if (minY > 31) minY = 31;
		if (maxY < 0) maxY = 0;
		if (maxY > 31) maxY = 31;
		if (minZ < 0) minZ = 0;
		if (minZ > 31) minZ = 31;
		if (maxZ < 0) maxZ = 0;
		if (maxZ > 31) maxZ = 31;

		// IDX == ((y & 0x1F) << 10) | ((z & 0x1F) << 5) | (x & 0x1F)
		// Iterate only through the specified section
		for (int y = minY; y <= maxY; y++) {
			for (int z = minZ; z <= maxZ; z++) {
				for (int x = minX; x <= maxX; x++) {
					int idx = (y << 10) | (z << 5) | x;

					int paletteIdx = 0;
					if (blockDataCount == 16384) {
						Abstract4BitPaletteChunkData* data4bit = (Abstract4BitPaletteChunkData*)chunkSection;
						uint8_t byteVal = data4bit->blockData->getUnsafe(idx / 2);
						paletteIdx = (idx % 2 == 0) ? ((byteVal >> 4) & 0x0F) : (byteVal & 0x0F);
					} else if (paletteCount > 256) {
						Abstract16BitPaletteChunkData* data16bit = (Abstract16BitPaletteChunkData*)chunkSection;
						paletteIdx = data16bit->blockData->getUnsafe(idx);
					} else {
						Abstract8BitPaletteChunkData* data8bit = (Abstract8BitPaletteChunkData*)chunkSection;
						paletteIdx = data8bit->blockData->getUnsafe(idx);
					}

					if (paletteIdx >= paletteCount)
						continue;

					int blockId = palette->getUnsafe(paletteIdx);

					if (targetSet.find(blockId) != targetSet.end()) {
						results.push_back(FishBlockData(x, y, z, blockId));
					}
				}
			}
		}

		return results;
	}

};