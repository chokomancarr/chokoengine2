#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

std::array<EW_S_DrawCompList::_DrawFn, (int)ComponentType::_COUNT> EW_S_DrawCompList::funcs;
std::array<EW_S_DrawCompList::_DrawFn, (int)ComponentType::_COUNT> EW_S_DrawCompList::activeFuncs;

CE_END_ED_NAMESPACE