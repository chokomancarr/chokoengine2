#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class GLUtils {
    static GLuint _vao;

public:
    static void DrawArrays(GLenum tp, int n, int o = 0);
};

CE_END_ED_NAMESPACE