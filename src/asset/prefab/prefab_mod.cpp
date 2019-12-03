#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

JsonObject _PrefabMod::ToJson() const {
	static const std::string TypeStr[] {
		"Modify",
		"Object"
	};

	JsonObject res(JsonObject::Type::Group);
	res.group.push_back(JsonPair(JsonObject("type"), TypeStr[(int)type]));
	res.group.push_back(JsonPair(JsonObject("target"), target.ToJson()));
	res.group.push_back(JsonPair(JsonObject("object"), object->ToJson()));
	return res;
}

void _PrefabMod::Instantiate(const SceneObject& par) const {
	const auto fnd = [&](const std::vector<SceneObject>& cc, const std::pair<std::string, int>& nm) -> SceneObject {
		int n = 0;
		for (auto& c : cc) {
			if (c->name() == nm.first) {
				if (n++ == nm.second) {
					return c;
				}
			}
		}
		return nullptr;
	};

	SceneObject p = par;
	for (auto& t : target.path) {
		p = fnd(p->children(), t);
		if (!p) return;
	}

	object->Instantiate(p);
}

CE_END_NAMESPACE