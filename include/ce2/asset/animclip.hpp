#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _AnimClip : public _Asset { CE_OBJECT_COMMON
public:
    struct Value;
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

};

CE_END_NAMESPACE

#include "animclip/animclip_value.hpp"
#include "animclip/animclip_entry.hpp"

/*
self.write(file, "ANIM")
file.write(struct.pack("<H", len(bones)))
file.write(struct.pack("<HH", fr0, fr1))

for i, bfn in enumerate(fullnames):
	self.write(file, bfn + "\x00")
	for m in mats:
		res = m[i][0]
		file.write(struct.pack("<fff", res[0], res[2], res[1]))
		res = m[i][1]
		file.write(struct.pack("<ffff", res[0], -res[1], -res[3], -res[2]))
		res = m[i][2]
		file.write(struct.pack("<fff", res[0], res[2], res[1]))

file.close()
*/