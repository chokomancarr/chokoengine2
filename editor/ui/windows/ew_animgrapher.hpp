#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_AnimGrapher : public EWindow {
	
	pAnimGraph _graph;
	class Node {
	public:
		bool isDefault = false;
		Vec2 position;
	};
	std::vector<Node> _nodes;

	Vec2 viewCenter;
	float viewScale;

	void RegAsset(AnimGraph g);

	void AddState(const Vec2& pos);

protected:

	void DrawMenu() override;

public:

	EW_AnimGrapher();

	bool Init() override;

	class Ops;
};

CE_END_ED_NAMESPACE

#include "ops/ew_animgrapher_ops.hpp"