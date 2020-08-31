#pragma once
#include "chokoeditor.hpp"
#include "script_val_class.hpp"

CE_BEGIN_ED_NAMESPACE

class ScriptVal {
public:
	ScriptVal(const ScriptVar& vr = {}, const ScriptInfo& info = nullptr);

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
	ScriptValClass val_class;

	static ScriptVal invalid;
};

CE_END_ED_NAMESPACE