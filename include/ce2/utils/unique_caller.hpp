#pragma once
#include "chokoengine.hpp"

/* hash function specialization for std::array
 */
namespace std {
    template<typename T, size_t N>
    struct hash<array<T, N>> {
        size_t operator()(const array<T, N>& a) const;
    };
}

#ifndef CE_NUM_TRACE_FRAMES
#define CE_NUM_TRACE_FRAMES 8
#endif

CE_BEGIN_NAMESPACE

class UniqueCaller {
public:
    typedef std::array<uintptr_t, CE_NUM_TRACE_FRAMES> traceframes;

	traceframes frames;
	ushort id;

	bool operator== (const UniqueCaller& rhs) const;
};

class UniqueCallerList {
public:
	std::unordered_map<UniqueCaller::traceframes, ushort> frames;

	UniqueCaller current, last;

	void Preloop();
	bool Add();
	void Set(), Clear();
};

CE_END_NAMESPACE

#include "detail/unique_caller.inl"