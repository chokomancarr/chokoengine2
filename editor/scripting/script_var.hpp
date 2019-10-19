#pragma once
#include "chokoeditor.hpp"
#include "asset/enums/asset_type.hpp"

CE_BEGIN_ED_NAMESPACE

class ScriptVar {
public:
	enum class Type {
		Int,
		Float,
		Double,
		Asset,
		Comp,
		ExtType
	};

	Type type;
	EAssetType typeAsset;
	ComponentType typeComp;
	pScriptTypeExt typeExt;
	std::string typeName;

	std::string name;
};

CE_END_ED_NAMESPACE