#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class ScriptVal {
public:
	ScriptVal(const ScriptVar& vr);

	ScriptVar var;
	union {
		bool val_b;
		int val_i;
		float val_f;
		double val_d;
		Vec2 val_v2;
		Vec3 val_v3;
		Vec4 val_v4;
		float _data[4];
	};
	Asset val_asset;
	SceneObject val_obj;
	Component val_comp;
	std::vector<ScriptVal> val_vec;

	static ScriptVal invalid;
};

CE_END_ED_NAMESPACE