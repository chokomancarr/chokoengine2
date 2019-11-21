#include "serializer.hpp"

CE_BEGIN_ED_NAMESPACE

std::unordered_map<ChokoEngine::objectid, int> ESerializer::objectToSceneId;

void ESerializer::UpdateSceneIds() {
	objectToSceneId.clear();
	int id = 0;
	std::function<void(const SceneObject&)> loop = [&](const SceneObject& o) {
		objectToSceneId[o->id()] = id++;
		for (const auto& c : o->children()) {
			loop(c);
		}
	};

	for (const auto& c : Scene::objects()) {
		loop(c);
	}
}

int ESerializer::SceneIdOf(const SceneObject& o) {
	return SceneIdOf(o->id());
}
int ESerializer::SceneIdOf(ChokoEngine::objectid id) {
	return objectToSceneId.at(id);
}

CE_END_ED_NAMESPACE