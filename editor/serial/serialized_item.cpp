#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

ESerializedItem::ESerializedItem(float f) : value({}), type(Type::Float) {
	value.f = f;
}
ESerializedItem::ESerializedItem(int i) : value({}), type(Type::Int) {
	value.i = i;
}
ESerializedItem::ESerializedItem(const Vec2& v) : value({}), type(Type::Vec2) {
	value.v2 = v;
}
ESerializedItem::ESerializedItem(const Vec3& v) : value({}), type(Type::Vec3) {
	value.v3 = v;
}
ESerializedItem::ESerializedItem(const Vec4& v) : value({}), type(Type::Vec4) {
	value.v4 = v;
}
ESerializedItem::ESerializedItem(const Color& v) : value({}), type(Type::Vec4) {
	value.v4 = v;
}
ESerializedItem::ESerializedItem(const Asset& a) : value({}), type(Type::Asset) {
	auto& av = value.assetref;
	av.sig = a->assetSignature();
	av.assetType = EAssetList::TypeOf(av.sig).assetType;
}
ESerializedItem::ESerializedItem(const SceneObject& s) : value({}), type(Type::SceneObject) {
	value.scobjref = CE_S_ObjectRef(s, Scene::objects()[1]);
}
ESerializedItem::ESerializedItem(const Component& c) : value({}), type(Type::Component) {
	auto& cv = value.compref;
	cv.obj = CE_S_ObjectRef(c->object(), Scene::objects()[1]);
	cv.type = c->componentType;
}

const std::string CE_ES_TypeS[] = {
	"Float",
	"Int",
	"Vec2",
	"Vec3",
	"Vec4",
	"Asset",
	"SceneObject",
	"Component"
};

JsonObject ESerializedItem::ToJson() const {
	JsonObject res;
	switch (type) {
	case Type::Float:
		res = JsonObject(std::to_string(value.f));
		break;
	case Type::Int:
		res = JsonObject(std::to_string(value.i));
		break;
	case Type::Vec2:
		res = JsonObject::FromVec2(value.v2);
		break;
	case Type::Vec3:
		res = JsonObject::FromVec3(value.v3);
		break;
	case Type::Vec4:
		res = JsonObject::FromVec4(value.v4);
		break;
	case Type::Asset:
		res = JsonObject({
			JsonPair(JsonObject("type"), EAssetTypeStr[(int)value.assetref.assetType]),
			JsonPair(JsonObject("sig"), JsonObject(value.assetref.sig))
		});
		break;
	default:
		res = JsonObject("not_implemented!");
		break;
	}

	return JsonObject({
		JsonPair(JsonObject("type"), CE_ES_TypeS[(int)type]),
		JsonPair(JsonObject("value"), res)
	});
}

CE_END_ED_NAMESPACE