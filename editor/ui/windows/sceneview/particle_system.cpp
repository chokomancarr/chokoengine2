#include "chokoeditor.hpp"
#include "utils/glutils.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_S_ParSys::Init() {
	EW_S_DrawCompList::activeFuncs[(int)ComponentType::ParticleSystem] = &DrawActive;
}

void EW_S_ParSys::DrawActive(const Component& c, const Mat4x4& p) {
	const auto& sys = static_cast<ParticleSystem>(c);
	const auto& tr = sys->object()->transform();

	const auto mvp = p * tr->worldMatrix();

	switch (sys->shape()) {
	case ParticleEmissionShape::Cone: {
		const auto r = sys->radius();
		const auto cth = std::cos(sys->angle() * Math::deg2rad);
		const auto sth = std::sin(sys->angle() * Math::deg2rad);

		const Color col(0.5f, 0.5f);

		UI::W::matrix(mvp);
		UI::W::Line(Vec3( r, 0, 0), Vec3( r + sth, 0, cth), col);
		UI::W::Line(Vec3(-r, 0, 0), Vec3(-r - sth, 0, cth), col);
		UI::W::Line(Vec3(0,  r, 0), Vec3(0,  r + sth, cth), col);
		UI::W::Line(Vec3(0, -r, 0), Vec3(0, -r - sth, cth), col);
		break;
	}
	case ParticleEmissionShape::Sphere: {

	}
	}
}

CE_END_ED_NAMESPACE