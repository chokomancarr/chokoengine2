#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EO_AddComp : public EOverlay {
	Vec2 pos;
	SceneObject obj;
public:
	static std::shared_ptr<EO_AddComp> instance;

	void Draw() override;
	static void Reg(const Rect& pos, const SceneObject& o);
};

CE_END_ED_NAMESPACE