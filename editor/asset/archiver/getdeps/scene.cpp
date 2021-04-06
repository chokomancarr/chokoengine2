#include "../getdeps.hpp"
#include "ce2/scene/comp/detail/compdefsall.inl"

CE_BEGIN_ED_NAMESPACE

std::array<std::unordered_set<
	std::string>, (size_t)AssetType::_COUNT> AssetDepends::resultA;
std::array<std::unordered_set<
	std::string>, (size_t)EExtType::_COUNT> AssetDepends::resultE;

void AssetDepends::Clear() {
	for (auto& a : resultA) {
		a.clear();
	}
	for (auto& e : resultE) {
		e.clear();
	}
}

void AssetDepends::Reg(const SceneObject& sc) {
	std::function<void(const SceneObject&)> doreg = [&](const SceneObject& o) {
		std::cout << " - object " << o->name() << std::endl;
		for (auto& p : o->prefabInfo().uids) {
			if (!p.id && !!p.prefab) {
				RegPrefab(p.prefab.lock());
			}
		}

		for (auto& c : o->components()) {
			switch (c->componentType) {
#define CS(tp)\
	case ComponentType::tp:\
		Reg ## tp((tp)c);\
		break;
				COMP_DEFS_ALL(CS)
			}
		}

		for (auto& c : o->children()) {
			doreg(c);
		}
	};

	doreg(sc);
}

CE_END_ED_NAMESPACE