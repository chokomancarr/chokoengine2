#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

enum class ShaderType {
	Vertex,
	Geometry,
	TesselCtrl,
	TesselEval,
	Fragment
};

CE_END_NAMESPACE