/*
 * This file is part of the FishPlusPlus distribution (https://github.com/Limitlesschicken/FishPlusPlus).
 * Copyright (c) FishPlusPlus.
 */
#pragma once

template<typename T>
struct Array {
    char pad_0000[0x10];
    T list[1];

    T get(int index) {
        return list[index];
    }
};