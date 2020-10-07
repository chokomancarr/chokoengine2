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

	static void Init();

	static void Draw(const Mat4x4& mv, const Mat4x4& p);
	static void DrawT(const Mat4x4& mv, const Mat4x4& p);
};

CE_END_ED_NAMESPACE