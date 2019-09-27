#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

enum class ComponentType {
	Animator,
	Camera,
	Light,
	LightProbe,
	MeshRenderer,
	Rig,
	Script,
	_COUNT
};

CE_END_NAMESPACE