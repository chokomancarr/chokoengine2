#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_S_DrawCompList {
	typedef void(*_DrawFn)(const Component& c);

public:
	static std::array<_DrawFn, (int)ComponentType::_COUNT> funcs;
};

CE_END_ED_NAMESPACE

#include "light.hpp"
#include "rig.hpp"