#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_S_DrawCompList {
	typedef void(*_DrawFn)(const Component& c);

public:
	static std::unordered_map<size_t, _DrawFn> funcs;
};

CE_END_ED_NAMESPACE

#include "rig.hpp"