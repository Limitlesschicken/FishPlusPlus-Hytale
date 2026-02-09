#pragma once

struct EntityStoreModule {
    char pad_0000[0x58];
    uintptr_t entityArray;
    char pad_0060[0x80];
    int entityCount;
};