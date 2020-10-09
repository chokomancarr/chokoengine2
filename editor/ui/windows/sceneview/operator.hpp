#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_S_Operator {
public:
	static enum class Mode {
		Translate,
		Rotate,
		Scale
	} mode;

	static enum class Space {
		Local,
		Global,
		View
	} space;

	static bool editing;
	static EW_SceneView* editingParent;

	static void Init();
	static void Update();

	static void Draw(const pTransform& tr, EW_SceneView* parent, bool active);

private:
	static bool DoDraw(const Mat4x4& mv, const Mat4x4& mvp, const Mesh& op, bool act);
};

CE_END_ED_NAMESPACE