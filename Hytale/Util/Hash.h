/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include <cstdint>
#include <string>

namespace Hash {
    constexpr uint64_t FNV_OFFSET = 0xCBF29CE484222325;
    constexpr uint64_t FNV_PRIME = 0x100000001B3;

    static constexpr inline uint64_t Hash(const char* str, uint64_t hash = FNV_OFFSET) {
        return (*str == '\0') ? hash : Hash(str + 1, (hash ^ static_cast<uint64_t>(*str)) * FNV_PRIME);
    }

    static constexpr inline uint64_t Hash(const std::string& str) {
        uint64_t hash = FNV_OFFSET;
        for (char c : str) {
            hash ^= static_cast<uint64_t>(c);
            hash *= FNV_PRIME;
        }
        return hash;
    }

    constexpr char ToLower(char c) {
        return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
    }

    static constexpr inline uint64_t HashCI(const char* str, uint64_t hash = FNV_OFFSET) {
        return (*str == '\0') ? hash : HashCI(str + 1, (hash ^ static_cast<uint64_t>(ToLower(*str))) * FNV_PRIME);
    }
}

static consteval inline uint64_t operator"" _hash(const char* str, size_t) {
    return Hash::Hash(str);
}

static consteval inline uint64_t operator"" _hashci(const char* str, size_t) {
    return Hash::HashCI(str);
}