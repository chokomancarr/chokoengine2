#pragma once
#include "chokoengine.hpp"

CE_SCRIPT_DEF(Turner) {
public:

	CE_SERIALIZE float rate1, rate2;
	CE_SERIALIZE CE_NS SceneObject obj;

	void OnUpdate() override;
};