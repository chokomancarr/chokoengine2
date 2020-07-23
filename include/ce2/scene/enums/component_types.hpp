#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

enum class ComponentType {
	Animator,
	Camera,
	Light,
	LightProbe,
	MeshRenderer,
	ParticleSystem,
	Rig,
	Script,
	_COUNT,
	Unknown
};

CE_END_NAMESPACE

#include "component_types_str.hpp"
