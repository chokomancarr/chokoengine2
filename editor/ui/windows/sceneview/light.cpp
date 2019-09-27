#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_S_Light::Init() {
	EW_S_DrawCompList::funcs[(int)ComponentType::Light] = &Draw;
}

void EW_S_Light::Draw(const Component& c) {
	
}

CE_END_ED_NAMESPACE