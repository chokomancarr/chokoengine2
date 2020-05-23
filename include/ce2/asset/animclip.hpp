#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

/* A clip containing multiple channels (entries)
 */
class _AnimClip : public _Asset { CE_OBJECT_COMMON
public:
	class Entry;

private:
	std::vector<Entry> _entries;
	Int2 _range;
	bool _repeat;

public:
	_AnimClip();

	CE_GET_SET_MEMBER(entries);
	CE_GET_SET_MEMBER(range);
	CE_GET_SET_MEMBER(repeat);

	struct VQ {
		bool valid = false;
		bool isQuat;
		Vec4 v;
		Quat q;
	};
	VQ Get(const std::string& sig, float t) const;
};

CE_END_NAMESPACE

#include "animclip/animclip_entry.hpp"