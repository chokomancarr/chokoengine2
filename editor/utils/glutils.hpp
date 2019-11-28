#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class GLUtils {
    static GLuint _vao;

public:
	static Shader simpleShad;

    static void DrawArrays(GLenum tp, int n, int o = 0);

	static void DrawElemsW(GLenum tp, int n, int o = 0, const Color& col = Color(1, 1), const Mat4x4& mat = Mat4x4::Identity());
};

CE_END_ED_NAMESPACE