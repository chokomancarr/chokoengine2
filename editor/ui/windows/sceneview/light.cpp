#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_S_Light::Init() {
	EW_S_DrawCompList::funcs[(int)ComponentType::Light] = &Draw;
	EW_S_DrawCompList::activeFuncs[(int)ComponentType::Light] = &DrawActive;
}

void EW_S_Light::Draw(const Component& c) {

}

void EW_S_Light::DrawActive(const Component& c) {
	const auto& lht = static_cast<Light>(c);
	const auto& tr = lht->object()->transform();

	switch (lht->type()) {
	case LightType::Point: {

		break;
	}
	case LightType::Spot: {
		const auto& p1 = tr->worldPosition();
		const auto& fw = tr->forward();
		UI::W::Line(p1 + fw * lht->radius(), p1 + fw * lht->distance(), Color::white());
		break;
	}
	case LightType::Directional: {

		break;
	}
	}
}

CE_END_ED_NAMESPACE