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
			Debug::Error("Prefab::ObjectRef", "target object is not in base tree! (" + _tnm0 + " -> " + base->name() + ")");
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

const SceneObject& Prefab_ObjRef::Seek(const std::vector<SceneObject>& objs) const {
	typedef decltype(path)::const_iterator _it;
	static SceneObject null = nullptr;
	if (!path.size()) {
		return null;
	}

	const std::function<const SceneObject&(_it, const std::vector<SceneObject>&)> seek = 
			[&](_it it, const std::vector<SceneObject>& oo) -> const SceneObject& {
		const auto& s = it->first;
		int i = it->second;
		for (auto& o : oo) {
			if (o->name() == s) {
				if (!(i--)) {
					it++;
					if (it == path.end()) return o;
					else {
						return seek(it, o->children());
					}
				}
			}
		}
		return null;
	};

	return seek(path.begin(), objs);
}

JsonObject Prefab_ObjRef::ToJson() const {
	JsonObject res(JsonObject::Type::Group);
	for (auto& i : path) {
		res.group.push_back(JsonPair(JsonObject(i.first), JsonObject(std::to_string(i.second))));
	}
	return res;
}

CE_END_NAMESPACE