/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once
#include "SigManager.h"

namespace API {
	template<typename T>
	T RHPNewFast(uint64_t objectMT) {
		using m_RhpNewFast = void* (*)(uint64_t);
		static m_RhpNewFast RhpNewFast_method = reinterpret_cast<m_RhpNewFast>(SM::RhpNewFastAddress);
		return (T) RhpNewFast_method(objectMT);
	}

	template<typename T>
	T RHPNewArray(uint64_t arrayMT, int count) {
		using m_RhpNewArray = void* (*)(uint64_t, int);
		static m_RhpNewArray RhpNewArray_method = reinterpret_cast<m_RhpNewArray>(SM::RhpNewArray_GenericAddress);
		return (T) RhpNewArray_method(arrayMT, count);
	}
}
