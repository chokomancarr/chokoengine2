#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

CE_E_BEGIN_DRAWASSET(Texture)

UI::Texture(Rect(lt.x + 4, lt.y + 4, lt.w - 8, lt.w - 8), obj);

lt.y += lt.w - 4;

CE_E_END_DRAWASSET

CE_END_ED_NAMESPACE
