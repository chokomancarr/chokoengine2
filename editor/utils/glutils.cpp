#include "chokoeditor.hpp"
#include "ext/glmext.hpp"
#include "utils/glutils.hpp"
#include "glsl/wcol.h"

CE_BEGIN_ED_NAMESPACE

GLuint GLUtils::_vao = 0;

Shader GLUtils::simpleShad = 0;

void GLUtils::BindEmptyVao() {
	if (!_vao) glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
}

void GLUtils::DrawArrays(GLenum tp, int n, int o) {
	BindEmptyVao();
	glDrawArrays(tp, o, n);
	glBindVertexArray(0);
}

void GLUtils::DrawElemsW(GLenum tp, int n, int o, const Color& col, const Mat4x4& mat) {
	if (!simpleShad) {
		(simpleShad = Shader::New(glsl::wColVert, glsl::wColFrag))->AddUniforms({
			"MVP", "col"
		});
	}
	simpleShad->Bind();
	glUniformMatrix4fv(simpleShad->Loc(0), 1, false, fptr(mat));
	glUniform4f(simpleShad->Loc(1), col.r, col.g, col.b, col.a);
	glDrawElements(tp, n, GL_UNSIGNED_INT, (byte*)o);
	simpleShad->Unbind();
}

CE_END_ED_NAMESPACE
