#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

Prefab_ObjRef::Prefab_ObjRef(SceneObject tar, const SceneObject& base) : path(0) {
	if (tar == base || !tar || !base) {
		return;
	}
	const auto _tnm0 = tar->name();

	const auto indexof = [](const std::vector<SceneObject>& oo, const SceneObject& o) {
		const auto& nm = o->name();
		int i = 0;
		for (auto& o2 : oo) {
			if (o2 == o) return i;
			else if (o2->name() == nm) {
				i++;
			}
		}
		return -1;
	};

	do {
		const auto tpr = tar->parent().lock();
		if (!tpr) {
			Debug::Warning("Prefab::ObjectRef", "target object is not in base tree! (" + _tnm0 + " -> " + base->name() + ")");
			path = {};
			return;
		}
		path.push_back(std::make_pair(tar->name(), indexof(tpr->children(), tar)));
		tar = tpr;
	} while (tar != base);
	std::reverse(path.begin(), path.end());
}

Prefab_ObjRef::Prefab_ObjRef(const JsonObject& json) {
	for (auto& p : json.group) {
		path.push_back(std::make_pair(p.key.string, p.value.ToInt()));
	}
}

const SceneObject& Prefab_ObjRef::Seek(const SceneObject& root) const {
	return Seek<SceneObject>(root,
		[](SceneObject o) {
			return o->name();
		},
		[](SceneObject o) {
			return o->children().size();
		},
		[](SceneObject o, size_t i) {
			return o->children()[i];
		}
	);
}

JsonObject Prefab_ObjRef::ToJson() const {
	JsonObject res(JsonObject::Type::Group);
	for (auto& i : path) {
		res.group.push_back(JsonPair(JsonObject(i.first), JsonObject(std::to_string(i.second))));
	}
	return res;
}

CE_END_NAMESPACE