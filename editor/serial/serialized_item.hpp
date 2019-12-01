#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class ESerializedItem {
public:
	enum class Type {
		Float,
		Int,
		Vec2,
		Vec3,
		Vec4,
		Asset,
		SceneObject,
		Component
	};

	struct {
		float f;
		int i;
		Vec2 v2;
		Vec3 v3;
		Vec4 v4;
		struct _assetrefst {
			EAssetType assetType;
			std::string sig;
		} assetref;
		CE_S_ObjectRef scobjref;
		struct _comprefst {
			CE_S_ObjectRef obj;
			ComponentType type;
		} compref;
	} value;

	Type type;

	ESerializedItem(float);
	ESerializedItem(int);
	ESerializedItem(const Vec2&);
	ESerializedItem(const Vec3&);
	ESerializedItem(const Vec4&);
	ESerializedItem(const Color&);
	ESerializedItem(const Asset&);
	ESerializedItem(const SceneObject&);
	ESerializedItem(const Component&);
	template <typename T, typename std::enable_if<std::is_enum<T>::value, T>::type* = nullptr>
	ESerializedItem(const T&);

	ESerializedItem(const JsonObject&);

	template <typename T>
	typename std::enable_if<std::is_enum<T>::value, T>::type
	Get() const;

	template <typename T>
	typename std::enable_if<!std::is_enum<T>::value, T>::type
	Get() const;

	template <typename T>
	T _Get() const;

	JsonObject ToJson() const;
};

CE_END_ED_NAMESPACE

#include "serialized_item.inl"