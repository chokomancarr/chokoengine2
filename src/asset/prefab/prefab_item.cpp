#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

#define CE_CHK(cond, err) if (cond) {\
	Debug::Warning("PrefabItem", err);\
	return;\
}

/* Construct from values
 */

#define PR_CTOR(tp, tpe, vr) PrefabItem::PrefabItem(tp t, const std::string& nm)\
		: PrefabItem(Type::tpe, nm) {\
	value.vr = t;\
}

PrefabItem::PrefabItem(Type t, const std::string& nm)
	: name(nm), value({}), type(t), is_array(false), is_default(false) {}

PR_CTOR(float, Float, f)
PR_CTOR(int, Int, i)
PR_CTOR(const Vec2&, Vec2, v2)
PR_CTOR(const Vec3&, Vec3, v3)
PR_CTOR(const Vec4&, Vec4, v4)
PR_CTOR(const Quat&, Quat, q)
PR_CTOR(const std::string&, String, s)
PR_CTOR(const Color&, Vec4, v4);

PrefabItem::PrefabItem(const Asset& a, const std::string& nm) : PrefabItem(Type::Asset, nm) {
	auto& av = value.assetref;
	if (!a) {
		av.sig = "none";
		av.assetType = AssetType::Unknown;
	}
	else {
		av.sig = a->assetSignature();
		av.assetType = a->assetType;
	}
}
PrefabItem::PrefabItem(const Prefab_ObjRef& s, const std::string& nm) : PrefabItem(Type::SceneObject, nm) {
	value.scobjref = s;
}
PrefabItem::PrefabItem(const Component& c, const std::string& nm) : PrefabItem(Type::Component, nm) {
	//auto& cv = value.compref;
	//cv.obj = Prefab_ObjRef(c->object(), ChokoEditor::scene->objects()[1]);
	//cv.type = c->componentType;

}
PrefabItem::PrefabItem(PrefabItemGroup g, const std::string& nm) : PrefabItem(Type::ItemGroup, nm) {
	value.group = std::move(g);
}
PrefabItem::PrefabItem(PrefabObjGroup g, const std::string& nm) : PrefabItem(Type::ObjGroup, nm) {
	value.objgroup = std::move(g);
}

PrefabItem::PrefabItem(const CRValue& cr, const std::string& nm) : PrefabItem(Type::ItemGroup, nm) {
	value.group.push_back(PrefabItem(cr.constant, "C"));
	value.group.push_back(PrefabItem(cr.random, "R"));
}

PrefabItem::PrefabItem(const Gradient& g, const std::string& nm) : PrefabItem(Type::ItemGroup, nm) {
	for (auto& i : g.entries) {
		value.group.push_back(PrefabItem(i.value, std::to_string(i.position)));
	}
}

/* Construct from json object
 */

const std::string CE_ES_TypeS[] = {
	"Float",
	"Int",
	"Vec2",
	"Vec3",
	"Vec4",
	"Quat",
	"String",
	"Asset",
	"SceneObject",
	"Component",
	"ItemGroup",
	"ObjGroup"
};

PrefabItem::PrefabItem(std::string tp, const JsonObject& vl, const Type force) : PrefabItem(Type::Int) {
	if (force == (Type)-1) {
		if (tp.back() == '!') {
			is_default = true;
			tp.pop_back();
		}
		if (tp.back() == '*') {
			is_array = true;
			tp.pop_back();
		}
		for (size_t i = 0; i < sizeof(CE_ES_TypeS) / sizeof(std::string); i++) {
			if (tp == CE_ES_TypeS[i]) {
				type = (Type)i;
				break;
			}
		}
		if (is_default) return;
		if (is_array) {
			for (auto& l : vl.list) {
				value.group.emplace_back(PrefabItem("", l, type));
			}
			return;
		}
	}
	else {
		if (vl.type == JsonObject::Type::String && vl.string == "__DEFAULT__") {
			is_default = true;
			return;
		}
		type = force;
	}
	switch (type) {
	case Type::Float:
		value.f = vl.ToFloat();
		break;
	case Type::Int:
		value.i = vl.ToInt();
		break;
	case Type::Vec2:
		value.v2 = vl.ToVec2();
		break;
	case Type::Vec3:
		value.v3 = vl.ToVec3();
		break;
	case Type::Vec4:
		value.v4 = vl.ToVec4();
		break;
	case Type::Quat:
		value.q = vl.ToQuat();
		break;
	case Type::String:
		value.s = vl.string;
		break;
	case Type::Asset: {
		CE_CHK(!vl.group.size(), "json asset entry is incomplete!");
		auto& g = vl.group[0];
		auto it = std::find_if(AssetTypeStr.begin(), AssetTypeStr.end(),
			[&g](decltype(AssetTypeStr)::const_reference i) {
			return i.second == g.key.string;
		});
		if (it != AssetTypeStr.end()) {
			value.assetref.assetType = it->first;
			value.assetref.sig = g.value.string;
		}
		else {
			Debug::Warning("PrefabItem", "Unknown asset type \"" + g.key.string + "\"!");
		}
		break;
	}
	case Type::SceneObject: {
		value.scobjref = Prefab_ObjRef(vl);
		break;
	}
	case Type::Component: {
		value.compref = Prefab_CompRef(vl);
		break;
	}
	case Type::ItemGroup: {
		for (auto& g2 : vl.group) {
			const auto ky = g2.key.string;
			const auto lc = ky.find_last_of('.');
			auto ress = PrefabItem(ky.substr(lc + 1), g2.value);
			ress.name = ky.substr(0, lc);
			value.group.emplace_back(std::move(ress));
		}
		break;
	}
	case Type::ObjGroup: {
		for (auto& g2 : vl.group) {
			const auto& k = g2.key.string;
			if (k == "object") {
				value.objgroup.push_back(PrefabObj_New(g2.value));
			}
			else if (k == "prefab") {
				value.objgroup.push_back(PrefabLink_New(g2.value));
			}
			else {
				value.objgroup.push_back(PrefabComp_New(g2));
			}
		}
		break;
	}
	default:
		Debug::Warning("PrefabItem", "Unknown type: \"" + tp + "\"!");
		break;
	}
}

/* Export to json object
 * "name.type": (contents)
 */

JsonPair PrefabItem::ToJson(const std::string& nm) const {
	JsonObject res;
	if (is_default) {
		return JsonPair(JsonObject(nm + "." + CE_ES_TypeS[(int)type] + 
			(is_array? "*!" : "!")), JsonObject("__DEFAULT__"));
	}
	if (is_array) {
		res = JsonObject(JsonObject::Type::List);
		for (auto& i : value.group) {
			res.list.push_back(i.ToJson("").value);
		}
		return JsonPair(JsonObject(nm + "." + CE_ES_TypeS[(int)type] + "*"), res);
	}
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
	case Type::Quat:
		res = JsonObject::FromQuat(value.q);
		break;
	case Type::String:
		res = JsonObject(value.s);
		break;
	case Type::Asset:
		res = JsonObject({JsonPair(
			JsonObject(AssetTypeStr.at(value.assetref.assetType)),
			JsonObject(value.assetref.sig)
		)});
		break;
	case Type::SceneObject:
		res = value.scobjref.ToJson();
		break;
	case Type::Component:
		res = value.compref.ToJson();
		break;
	case Type::ItemGroup: {
		res = JsonObject(JsonObject::Type::Group);
		for (auto& g : value.group) {
			res.group.push_back(g.ToJson(g.name));
		}
		break;
	}
	case Type::ObjGroup: {
		res = JsonObject(JsonObject::Type::Group);
		for (auto& g : value.objgroup) {
			res.group.push_back(g->ToJson());
		}
		break;
	}
	default:
		res = JsonObject("not_implemented!");
		break;
	}

	return JsonPair(JsonObject(nm + "." + CE_ES_TypeS[(int)type]), res);
}

CE_END_NAMESPACE