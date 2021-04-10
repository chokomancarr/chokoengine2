#include "chokoeditor.hpp"
#include "obj/operatorT.h"
#include "obj/operatorR.h"
#include "obj/operatorS.h"
#include "prefs/preferences.hpp"

CE_BEGIN_ED_NAMESPACE

namespace {
	Mesh operatorT, operatorR, operatorS;

	EW_SceneView* parent;

	Color axisCols[3] = {
		Color(1, 0.2f, 0.2f),
		Color(0, 1, 0),
		Color(0.2f, 0.2f, 1)
	};
	Color activeAxisCol = Color(1.0f, 1.0f, 0.2f);
	Color inactiveAxisCol = Color(0.5f);
	Color axisLineCol = Color(0.7f);

	Vec3 editingAxis;
	pTransform editingTr;

	inline float uint2float(const uint i) {
		return *(float*)&i;
	}
}

EW_S_Operator::Mode EW_S_Operator::mode = Mode::Translate;

EW_S_Operator::Space EW_S_Operator::space = Space::Local;

bool EW_S_Operator::editing = false;
EW_SceneView* EW_S_Operator::editingParent = nullptr;

void EW_S_Operator::Init() {
	operatorT = MeshLoader::LoadObj(obj::operatorT);
	operatorR = MeshLoader::LoadObj(obj::operatorR);
	operatorS = MeshLoader::LoadObj(obj::operatorS);

	EPreferences::Colors::Reg("Operators", "X axis", axisCols[0]);
	EPreferences::Colors::Reg("Operators", "Y axis", axisCols[1]);
	EPreferences::Colors::Reg("Operators", "Z axis", axisCols[2]);
	EPreferences::Colors::Reg("Operators", "active axis", activeAxisCol);
	EPreferences::Colors::Reg("Operators", "axis line", axisLineCol);
}

void EW_S_Operator::Update() {
	if (Input::mouseStatus(InputMouseButton::Left) == InputMouseStatus::Up) {
		editing = false;
		Cursor::locked(false);
	}
	else {
		switch (mode) {
		case Mode::Translate:
			editingTr->worldPosition(
				editingTr->worldPosition() + editingAxis * 0.05f * Input::mouseDelta().x
			);
			break;
		case Mode::Rotate:
			editingTr->worldRotation(
				Quat::FromAxisAngle(editingAxis, 0.2f * Input::mouseDelta().x)
				* editingTr->worldRotation()
			);
			break;
		case Mode::Scale:
			editingTr->localScale(
				editingTr->localScale() + editingAxis * 0.05f * Input::mouseDelta().x
			);
			break;
		}
	}
}

void EW_S_Operator::Draw(const pTransform& tar, EW_SceneView* par, bool active) {
	parent = par;
	const auto& cam = parent->camera();
	const auto& p = cam->lastViewProjectionMatrix();

	if (editing) {
		const auto p0 = editingTr->worldPosition();
		UI::W::matrix(p);
		UI::W::Line(p0, p0 - editingAxis * 100, axisLineCol);
		UI::W::Line(p0, p0 + editingAxis * 100, axisLineCol);
		return;
	}

	const float operatorSize = 200; //relative to pixels
	const auto& ptr = parent->pivot()->transform();

	const float scl = operatorSize * ptr->localScale().x / parent->position.h();

	
	Mat4x4 mv;
	Mat4x4 mvp;

	switch (space) {
	case Space::Global:
		mvp = p * 
			(mv = Mat4x4::FromTRS(tar->worldPosition(), Quat::identity(), scl));
		break;
	case Space::Local:
		mvp = p * 
			(mv = Mat4x4::FromTRS(tar->worldPosition(), tar->worldRotation(), scl));
		break;
	case Space::View:
		if (cam->orthographic())
			mvp = p * 
				(mv = Mat4x4::FromTRS(tar->worldPosition(), ptr->worldRotation(), scl));
		else
			mvp = p * 
				(mv = Mat4x4::FromTRS(tar->worldPosition(), 
					Quat::LookAt(cam->object()->transform()->worldPosition()
					- tar->worldPosition(), Vec3::up()), scl));
		break;
	}

	glDepthMask(true);
	glDepthFunc(GL_LEQUAL);
	glClear(GL_DEPTH_BUFFER_BIT);
	glDisable(GL_BLEND);

	switch (mode) {
	case Mode::Translate:
		if (DoDraw(mv, mvp, operatorT, active)) {
			editingTr = tar;
		}
		break;
	case Mode::Rotate:
		if (DoDraw(mv, mvp, operatorR, active)) {
			editingTr = tar;
		}
		break;
	case Mode::Scale:
		if (DoDraw(mv, mvp, operatorS, active)) {
			editingTr = tar;
		}
		break;
	}

	glDepthMask(false);
	glDepthFunc(GL_ALWAYS);
	glEnable(GL_BLEND);
}

bool EW_S_Operator::DoDraw(const Mat4x4& mv, const Mat4x4& mvp, const Mesh& mesh, bool act) {
	auto& mat = EW_S_Resources::unlitMat;

	Mat4x4 mats[3] = {
		Mat4x4(),
		Mat4x4::Rotation(Vec3(0, 0, -90.f)),
		Mat4x4::Rotation(Vec3(0, 90.f, 0))
	};

	mesh->BindVao();
	mesh->BindElo(0);
	for (int a = 0; a < 3; a++) {
		Color col = act ? axisCols[a] : inactiveAxisCol;
		col.a = uint2float(32 + a);
		mat->SetUniform("MVP", mvp * mats[a]);
		mat->SetUniform("col", col);
		mat->Bind();
		glDrawElements(GL_TRIANGLES, mesh->triangleCount() * 3, GL_UNSIGNED_INT, 0);
	}
	mat->Unbind();
	mesh->Unbind();

	if (act) {
		parent->camera()->target()->BindTarget(true);
		auto pos = Input::mousePosition() - Vec2(parent->position.x(), parent->position.y());
		pos.y = parent->position.h() - pos.y;
		Color pixel;
		glReadPixels(pos.x, pos.y, 1, 1, GL_RGBA, GL_FLOAT, &pixel);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

		const uint sid = *(uint*)&pixel.a;
		if (sid >= 32 && sid <= 34) {
			mesh->BindVao();
			mesh->BindElo(0);
			mat->SetUniform("MVP", mvp * mats[sid - 32]);
			mat->SetUniform("col", activeAxisCol);
			mat->Bind();
			glDrawElements(GL_TRIANGLES, mesh->triangleCount() * 3, GL_UNSIGNED_INT, 0);
			mat->Unbind();
			mesh->Unbind();

			if (Input::mouseStatus(InputMouseButton::Left) == InputMouseStatus::Down) {
				editing = true;
				editingParent = parent;
				Vec4 v(0);
				v[sid - 32] = 1;
				v = mv * v;
				editingAxis = ((Vec3)v).normalized();
				Cursor::locked(true);
				return true;
			}
		}
	}
	return false;
}

CE_END_ED_NAMESPACE