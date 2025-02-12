#pragma once
#include "chokoplayer.hpp"

#define CE_MOD_AC_NS ModuleAC

#define CE_BEGIN_MOD_AC_NAMESPACE\
	CE_BEGIN_NAMESPACE\
	namespace CE_MOD_AC_NS {

#define CE_END_MOD_AC_NAMESPACE }\
	CE_END_NAMESPACE

CE_BEGIN_MOD_AC_NAMESPACE

AnimClip LoadAnimClip(DataStream strm);
AnimGraph LoadAnimGraph(const JsonObject& data);
Armature LoadArmature(const JsonObject& data);
Shader LoadShader(const JsonObject& data);

CE_END_MOD_AC_NAMESPACE
