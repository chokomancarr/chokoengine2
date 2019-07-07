#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_S_Rig::Init() {
	EW_S_DrawCompList::funcs[typeid(_Rig).hash_code()] = &Draw;
}

void EW_S_Rig::Draw(const Component& rig) {
	if (rig->object() != ESceneInfo::selectedObject)
		return;
	UI::Label(Rect(20, 20, 200, 20), "asdfasdfasdfasdfsadf", Color::green());
}

CE_END_ED_NAMESPACE