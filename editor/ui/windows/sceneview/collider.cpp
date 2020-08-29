#include "chokoeditor.hpp"
#include "utils/glutils.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_S_Collider::Init() {
	EW_S_DrawCompList::funcs[(int)ComponentType::Collider] = &Draw;
	EW_S_DrawCompList::activeFuncs[(int)ComponentType::Collider] = &DrawActive;
}

void EW_S_Collider::Draw(const Component& c, const Mat4x4& p) {
	const auto& col = static_cast<EW_S_Collider>(c);
	const auto& tr = col->object()->transform();

	const auto mvp = p * tr->worldMatrix();

	
}

void EW_S_Collider::DrawActive(const Component& c, const Mat4x4& p) {
	Draw(c, p);
}

CE_END_ED_NAMESPACE