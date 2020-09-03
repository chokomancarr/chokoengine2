#pragma once
#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

typedef std::vector<PrefabItem> PrefabItemGroup;
typedef std::vector<PrefabObjBase> PrefabObjGroup;

class PrefabItem {
public:
	enum class Type {
		Float,
		Int,
		Vec2,
		Vec3,
		Vec4,
		Quat,
		String,
		Asset,
		SceneObject,
		Component,
		ItemGroup,
		ObjGroup
	};

	std::string name;

	struct {
		float f;
		int i;
		Vec2 v2;
		Vec3 v3;
		Vec4 v4;
		Quat q;
		std::string s;
		struct _assetrefst {
			AssetType assetType;
			std::string sig;
			template <typename T>
			T Load(AssetType t) const {
				return (T)PrefabState::sig2AssFn(t, sig);
			}
		} assetref;
		Prefab_ObjRef scobjref;
		Prefab_CompRef compref;
		PrefabItemGroup group;
		PrefabObjGroup objgroup;
	} value;

	Type type;

	/* array items have values in [itemgroup]
	 * with contents of type [type]
	 */
	bool is_array;

	PrefabItem(Type);
	PrefabItem(float);
	PrefabItem(int);
	PrefabItem(const Vec2&);
	PrefabItem(const Vec3&);
	PrefabItem(const Vec4&);
	PrefabItem(const Quat&);
	PrefabItem(const std::string&);
	PrefabItem(const Color&);
	PrefabItem(const Asset&);
	PrefabItem(const Prefab_ObjRef&);
	PrefabItem(const Component&);
	template <typename T, typename std::enable_if<
		std::is_enum<T>::value && !std::is_same<T, Type>::value, T>::type* = nullptr>
	PrefabItem(const T&);
	PrefabItem(PrefabItemGroup);
	PrefabItem(PrefabObjGroup);

	PrefabItem(std::string tp, const JsonObject&, const Type force = (Type)-1);

	template <typename T>
	typename std::enable_if<std::is_enum<T>::value, T>::type
		Get() const;

	template <typename T>
	typename std::enable_if<!std::is_enum<T>::value, T>::type
		Get() const;

	template <typename T>
	T _Get() const;

	JsonPair ToJson(const std::string&) const;
};

CE_END_NAMESPACE

#include "prefab_item.inl"