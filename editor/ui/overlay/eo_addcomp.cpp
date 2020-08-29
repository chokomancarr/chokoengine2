#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

namespace {
	typedef std::array<std::pair<std::string, std::vector<ComponentType>>, 3> compArr;
	typedef void(*addcompfn)(SceneObject&);
	typedef std::array<addcompfn, (int)ComponentType::_COUNT> addCompArr;

	compArr GenArrs() {
		compArr res;
		int i = 0;
#define add(nm) res[i].first = #nm; res[i++].second =
		add(Modifiers) { ComponentType::Animator, ComponentType::Rig, ComponentType::MeshRenderer };
		add(Rendering) { ComponentType::Camera, ComponentType::Light, ComponentType::LightProbe };
#undef add
		res.back().first = "Other";
		for (int a = 0; a < (int)ComponentType::_COUNT; a++) {
			const auto c = (ComponentType)a;
			for (size_t b = 0; b < res.size() - 1; b++) {
				const auto& s = res[b].second;
				if (std::find(s.begin(), s.end(), c) != s.end()) {
					goto found;
				}
			}
			res.back().second.push_back(c);
		found:;
		}
		return res;
	}

	addCompArr GenArrGens() {
		addCompArr res;
#define add(nm, ...) res[(int)ComponentType::nm] = [](SceneObject& o) { o->AddComponent<nm>(__VA_ARGS__); }
#define addv(nm, tp, ...) res[(int)ComponentType::nm] = [](SceneObject& o) { o->AddComponent<tp>(__VA_ARGS__); }
		add(Animator, );
		add(Camera, );
		add(Collider, );
		add(Light, LightType::Point);
		add(MeshRenderer, );
		add(ParticleSystem, );
		add(Rig, );
		add(Rigidbody, );
		addv(Script, DummyScript, );
#undef add
		return res;
	}

	Vec2 GetSz(const compArr& arr) {
		Vec2 res = Vec2(arr.size() * 150 + 4, 0);
		for (auto& a : arr) {
			res.y = std::max(res.y, 24 + a.second.size() * 18 + 2.f);
		}
		return res;
	}
}

std::shared_ptr<EO_AddComp> EO_AddComp::instance;

void EO_AddComp::Draw() {
	static const auto arr = GenArrs();
	static const auto arrg = GenArrGens();
	static const auto sz = GetSz(arr);

	auto x0 = pos.x - sz.x;
	auto y0 = pos.y;
	UI::Rect(Rect(x0, y0, sz.x, sz.y), Color(0.1f, 0.7f));

	x0 += 4;
	y0 += 2;

	for (auto& a : arr) {
		UI::Label(Rect(x0, y0, 150, 16), a.first, Color(0.7f));
		int y1 = y0 + 22;
		for (auto& c : a.second) {
			if (UI::I::Button(Rect(x0, y1, 146, 16), Color(0.2f), ComponentTypeStr.at(c)) == InputMouseStatus::HoverUp) {
				arrg[(int)c](obj);
				active = false;
			}
			y1 += 18;
		}
		x0 += 150;
	}
}

void EO_AddComp::Reg(const Rect& pos, const SceneObject& o) {
	auto i = GetInstance<EO_AddComp>();
	i->pos = Vec2(pos.x2(), pos.y2());
	i->obj = o;
	i->active = true;
}

CE_END_ED_NAMESPACE