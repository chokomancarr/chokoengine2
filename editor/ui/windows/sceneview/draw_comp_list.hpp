#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_S_DrawCompList {
	typedef void(*_DrawFn)(const Component& c, const Mat4x4& p);

public:
	static std::array<_DrawFn, (int)ComponentType::_COUNT> funcs;
	static std::array<_DrawFn, (int)ComponentType::_COUNT> activeFuncs;
};

CE_END_ED_NAMESPACE

#include "camera.hpp"
#include "light.hpp"
#include "rig.hpp"