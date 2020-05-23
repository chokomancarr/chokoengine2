#include "asset_loader.hpp"

CE_BEGIN_MOD_AE_NAMESPACE

#define READ(vr) strm.read((char*)&vr, sizeof(vr))

#pragma optimize("", off)
CE_MOD_AE_IMPL(AnimClip) {
	std::ifstream strm(_basePath + path, std::ios::binary);
	if (!strm) return nullptr;
	{ //the first 4 bytes is ANIM
		char sig[5] = {};
		strm.read(sig, 4);
		if (!!strcmp(sig, "ANIM")) {
			return nullptr;
		}
	}
	//channel and frame sizes
	uint16_t numchannels;
	READ(numchannels);
	int32_t framestart, frameend;
	READ(framestart);
	READ(frameend);

	union _dt {
		Vec4 v;
		Quat q;
		char buf[16];
		_dt() {}
	} dt;
	
	std::vector<_AnimClip::Entry> entries = {};

	//for each channel:
	// name(str) \0 numframes(uint) nc(byte) isquat(byte) [value(floatxnc) time(ushort)]xnumframes
	//or
	// name(str) \0 -1(uint) nc(byte) isquat(byte) [value(floatxnc)]x(frameend-framestart+1)
	//note: time is relative to framestart
	for (uint16_t c = 0; c < numchannels; c++) {
		_AnimClip::Entry entry;
		std::getline(strm, entry.signature, char(0));
		auto pos = strm.tellg();
		uint32_t numframes;
		READ(numframes);
		byte numchn;
		READ(numchn);
		byte isquat;
		READ(isquat);
		entry.isQuat = !!isquat;

		memset(dt.buf, 0, sizeof(dt));

		std::vector<Curve<Quat>::_KeyTp> qvals = {};
		std::vector<Curve<Vec4>::_KeyTp> vvals = {};

		if (!(~numframes)) {
			for (int32_t a = framestart; a <= frameend; a++) {
				strm.read(dt.buf, numchn * 4);
				if (isquat) {
					qvals.push_back(std::make_pair(a, dt.q));
				}
				else {
					vvals.push_back(std::make_pair(a, dt.v));
				}
			}
		}
		else {
			uint16_t time;
			for (int32_t a = 0; a < numframes; a++) {
				strm.read(dt.buf, numchn * 4);
				READ(time);
				if (isquat) {
					entry.values_q.AddKey(time + framestart, dt.q);
				}
				else {
					entry.values_v.AddKey(time + framestart, dt.v);
				}
			}
		}
		entry.values_q.keys(qvals);
		entry.values_v.keys(vvals);

		entries.push_back(entry);
	}

	auto res = AnimClip::New();
	res->entries(entries);
	res->range(Int2(framestart, frameend));
	res->repeat(false);

	return res;
}

CE_END_MOD_AE_NAMESPACE
