/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include "Object.h"

template<typename T>
struct Array : Object {
	int count;          // 0x08
	char pad_C[0x4];    // 0x0C
	T list[1];          // 0x10

    T get(int index) {
        if (index < 0 || index >= count)
			return T();
        return list[index];
    }

    T getUnsafe(int index) {
        return list[index];
    }

    template<typename T>
    static Array<T>* createArray(int size) {
        size_t totalSize = sizeof(Array<T>) + sizeof(T) * (size - 1);
        void* memory = malloc(totalSize);

        Array<T>* arr = new(memory) Array<T>();
        arr->count = size;

        for (int i = 0; i < size; i++)
            new(&arr->list[i]) T();

        return arr;
    }
};