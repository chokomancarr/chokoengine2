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
	auto& sv = value.scobjref;
	sv.id = ESerializer::SceneIdOf(s);
}
ESerializedItem::ESerializedItem(const Component& c) : value({}), type(Type::Component) {
	auto& cv = value.compref;
	cv.obj.id = ESerializer::SceneIdOf(c->object());
	cv.type = c->componentType;
}

CE_END_ED_NAMESPACE