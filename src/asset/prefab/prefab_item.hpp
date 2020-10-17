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
	bool is_default;

	PrefabItem(Type, const std::string& nm = "");
	PrefabItem(float, const std::string& nm = "");
	PrefabItem(int, const std::string& nm = "");
	PrefabItem(const Vec2&, const std::string& nm = "");
	PrefabItem(const Vec3&, const std::string& nm = "");
	PrefabItem(const Vec4&, const std::string& nm = "");
	PrefabItem(const Quat&, const std::string& nm = "");
	PrefabItem(const std::string&, const std::string& nm = "");
	PrefabItem(const Color&, const std::string& nm = "");
	PrefabItem(const Asset&, const std::string& nm = "");
	PrefabItem(const Prefab_ObjRef&, const std::string& nm = "");
	PrefabItem(const Prefab_CompRef&, const std::string& nm = "");
	template <typename T, typename std::enable_if<
		std::is_enum<T>::value && !std::is_same<T, Type>::value, T>::type* = nullptr>
	PrefabItem(const T&, const std::string& nm = "");
	PrefabItem(PrefabItemGroup, const std::string& nm = "");
	PrefabItem(PrefabObjGroup, const std::string& nm = "");

	//extension types
	PrefabItem(const CRValue&, const std::string& nm = "");
	PrefabItem(const FCurve&, const std::string& nm = "");
	PrefabItem(const Gradient&, const std::string& nm = "");

	PrefabItem(std::string tp, const JsonObject&, const Type force = (Type)-1);

#define ISENUM std::is_enum<T>::value
#define ISASSET is_base_ref_of<Asset, T>::value
#define ISCOMP is_base_ref_of<Component, T>::value

	template <typename T>
	typename std::enable_if<ISENUM, T>::type
		Get() const;

	template <typename T>
	typename std::enable_if<ISASSET, T>::type
		Get() const;

	template <typename T>
	typename std::enable_if<ISCOMP, T>::type
		Get() const;

	template <typename T>
	typename std::enable_if<!ISENUM && !ISASSET && !ISCOMP, T>::type
		Get() const;

#undef ISENUM
#undef ISASSET
#undef ISCOMP

	template <typename T>
	T _Get() const;

	JsonPair ToJson(const std::string&) const;
};

CE_END_NAMESPACE

#include "prefab_item.inl"