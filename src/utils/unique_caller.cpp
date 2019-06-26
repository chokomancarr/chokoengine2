#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

bool UniqueCaller::operator== (const UniqueCaller& rhs) const {
	for (int a = 0; a < CE_NUM_TRACE_FRAMES; a++) {
		if (frames[a] != rhs.frames[a]) return false;
	}
	return id == rhs.id;
}

void UniqueCallerList::Preloop() {
	frames.clear();
}

bool UniqueCallerList::Add() {
	current = UniqueCaller();
	Debug::StackTrace(CE_NUM_TRACE_FRAMES, current.frames.data());
	current.id = ++frames[current.frames];
	return current == last;
}

void UniqueCallerList::Set() {
	last = current;
}

void UniqueCallerList::Clear() {
	last = UniqueCaller();
}

CE_END_NAMESPACE