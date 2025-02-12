#pragma once
#include "inc.hpp"

CE_BEGIN_MOD_AC_NAMESPACE

namespace {
	void LoadBones(const JsonObject& data, std::vector<ArmatureBone>& bones) {
		for (const auto& d : data.group) {
			ArmatureBone bn(d.key.string);
			for (const auto& c : d.value.group) {
				if (c.key.string == "head") {
					bn.base = c.value.ToVec3();
				}
				else if (c.key.string == "tail") {
					bn.length = c.value.ToVec3();
				}
				else if (c.key.string == "front") {
					bn.front = c.value.ToVec3();
				}
				else if (c.key.string == "connected") {
					bn.connected = c.value.ToBool();
				}
				else if (c.key.string == "children") {
					LoadBones(c.value, bn.children);
				}
			}
			bn.length -= bn.base;
			bones.push_back(bn);
		}
	}
}

Armature LoadArmature(const JsonObject& data) {
	if (data.group[0].key.string != "armature") {
		Debug::Error("LoadArmature", "Cannot create from json, no armature entry!");
		return nullptr;
	}
	std::vector<ArmatureBone> bones = {};
	LoadBones(data.group[0].value, bones);

	Armature arm = Armature::New();
	arm->bones(bones);
	return arm;
}

CE_END_MOD_AC_NAMESPACE
