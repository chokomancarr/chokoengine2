#include "chokoeditor.hpp"
#include "obj/operatorT.h"

CE_BEGIN_ED_NAMESPACE

namespace {
	Mesh operatorT;

	Mat4x4 removescaling(Mat4x4 m) {
		const auto v1 = (*(Vec3*)&m[0]).normalized();
		const auto v2 = (*(Vec3*)&m[4]).normalized();
		const auto v3 = (*(Vec3*)&m[8]).normalized();
		return Mat4x4(
			v1.x, v1.y, v1.z, m[3], v2.x, v2.y, v2.z, m[7],
			v3.x, v3.y, v3.z, m[11], m[12], m[13], m[14], m[15]
		);
	}
}

void EW_S_Operator::Init() {
	operatorT = MeshLoader::LoadObj(obj::operatorT);
}

void EW_S_Operator::Draw(const Mat4x4& mv, const Mat4x4& p) {
	DrawT(mv, p);
}

void EW_S_Operator::DrawT(const Mat4x4& mv, const Mat4x4& p) {
	auto& mat = EW_S_Resources::unlitMat;
	auto mvp = p * mv;

	struct _axis {
		Vec4 pos;
		Color col;
		Mat4x4 mat;
	} axis[3];

	axis[0] = _axis{ Vec4(1, 0, 0, 1), Color::red(), Mat4x4() };
	axis[1] = _axis{ Vec4(0, 1, 0, 1), Color::green(), Mat4x4::Rotation(Vec3(0, 0, -90.f)) };
	axis[2] = _axis{ Vec4(0, 0, -1, 1), Color::blue(),  Mat4x4::Rotation(Vec3(0, -90.f, 0)) };

	for (int a = 0; a < 3; a++) {
		auto& ps = axis[a].pos;
		ps = (mvp * ps);
		ps /= ps.w;
	}
	std::cout << std::endl;

	std::sort(axis, axis + 3, [](const _axis& a, const _axis& b) {
		return a.pos.z > b.pos.z;
	});
	
	operatorT->BindVao();
	operatorT->BindElo(0);
	for (int a = 0; a < 3; a++) {
		mat->SetUniform("MVP", mvp * axis[a].mat);
		mat->SetUniform("col", axis[a].col);
		mat->Bind();
		glDrawElements(GL_TRIANGLES, operatorT->triangleCount() * 3, GL_UNSIGNED_INT, 0);
	}
	mat->Unbind();
	operatorT->Unbind();
}

CE_END_ED_NAMESPACE