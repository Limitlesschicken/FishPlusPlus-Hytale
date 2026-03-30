/*
 * Copyright (c) FishPlusPlus.
 */
#include "TrampolineUnwind.h"
#include <vector>
#include <mutex>

namespace {

struct UnwindRegistration {
    void* allocBlock;
    bool  registered;
};

std::mutex g_mutex;
std::vector<UnwindRegistration> g_registrations;

}

namespace TrampolineUnwind {

void Register(void* pTrampoline, void* pOriginalFunction, uint32_t trampolineSize) {
    if (!pTrampoline || !pOriginalFunction || trampolineSize == 0)
        return;

    DWORD64 imageBase = 0;
    auto* pOrigRF = RtlLookupFunctionEntry(
        reinterpret_cast<DWORD64>(pOriginalFunction), &imageBase, nullptr);
    if (!pOrigRF)
        return;

    auto* pOrigUI = reinterpret_cast<uint8_t*>(imageBase + pOrigRF->UnwindInfoAddress);
    uint8_t countOfCodes = pOrigUI[2];
    uint32_t codesSize = countOfCodes * 2;
    if (codesSize & 7) codesSize = (codesSize + 7) & ~7u;
    uint8_t flags = (pOrigUI[0] >> 3) & 0x1F;

    uint32_t uiSize = 4 + codesSize;
    if (flags & 0x04)       uiSize += 12;
    else if (flags & 0x03)  uiSize += 4;

    auto blockSize = static_cast<uint32_t>(sizeof(RUNTIME_FUNCTION) + uiSize);
    auto* pBlock = VirtualAlloc(nullptr, blockSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!pBlock) return;

    auto base = reinterpret_cast<uintptr_t>(pBlock);
    auto* pRF = reinterpret_cast<RUNTIME_FUNCTION*>(pBlock);
    auto* pUI = reinterpret_cast<uint8_t*>(base + sizeof(RUNTIME_FUNCTION));

    memcpy(pUI, pOrigUI, uiSize);

    pRF->BeginAddress = static_cast<DWORD>(reinterpret_cast<uintptr_t>(pTrampoline) - base);
    pRF->EndAddress   = pRF->BeginAddress + trampolineSize;
    pRF->UnwindInfoAddress = static_cast<DWORD>(sizeof(RUNTIME_FUNCTION));

    BOOLEAN ok = RtlAddFunctionTable(pRF, 1, base);

    std::lock_guard lock(g_mutex);
    g_registrations.push_back({ pBlock, ok != 0 });
}

void CleanupAll() {
    std::lock_guard lock(g_mutex);
    for (auto& reg : g_registrations) {
        if (reg.registered)
            RtlDeleteFunctionTable(reinterpret_cast<PRUNTIME_FUNCTION>(reg.allocBlock));
        if (reg.allocBlock)
            VirtualFree(reg.allocBlock, 0, MEM_RELEASE);
    }
    g_registrations.clear();
}

}
