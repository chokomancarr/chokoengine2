#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

Mat4x4 UI::W::_matrix = Mat4x4::Identity();

void UI::W::Line(const Vec3& p1, const Vec3& p2, const Color& col) {
	auto cv = [](const Vec4& v) {
		return *(Vec3*)glm::value_ptr((glm::vec4)v) / v.w;
	};

	const Vec3 vs[] = {
		cv(_matrix * Vec4(p1, 1)),
		cv(_matrix * Vec4(p2, 1))
	};

	UI::SetVao(2, vs, nullptr);

	colShad->Bind();
	glUniform4f(colShad->Loc(0), col.r, col.g, col.b, col.a * _alpha);
	_vao->Bind();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_LINES, 0, 2);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	_vao->Unbind();
	colShad->Unbind();
}

CE_END_NAMESPACE