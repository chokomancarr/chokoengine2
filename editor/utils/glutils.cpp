#include "chokoeditor.hpp"
#include "utils/glutils.hpp"

CE_BEGIN_ED_NAMESPACE

GLuint GLUtils::_vao = 0;

void GLUtils::DrawArrays(GLenum tp, int n, int o) {
    if (!_vao) glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glDrawArrays(tp, o, n);
    glBindVertexArray(0);
}

CE_END_ED_NAMESPACE
