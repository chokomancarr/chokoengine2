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
	_COUNT,
	Unknown
};

const std::array<std::string, (int)ComponentType::_COUNT> ComponentTypeStr = {
	"Animator",
	"Camera",
	"Light",
	"LightProbe",
	"MeshRenderer",
	"Rig",
	"Script"
};

CE_END_NAMESPACE