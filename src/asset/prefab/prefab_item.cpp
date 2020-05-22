#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

#define CE_CHK(cond, err) if (cond) {\
	Debug::Warning("PrefabItem", err);\
	return;\
}

/* Construct from values
 */

#define PR_CTOR(tp, tpe, vr) PrefabItem::PrefabItem(tp t)\
		: value({}), type(Type::tpe) {\
	value.vr = t;\
}

PrefabItem::PrefabItem(Type t) : value({}), type(t) {}

PR_CTOR(float, Float, f)
PR_CTOR(int, Int, i)
PR_CTOR(const Vec2&, Vec2, v2)
PR_CTOR(const Vec3&, Vec3, v3)
PR_CTOR(const Vec4&, Vec4, v4)
PR_CTOR(const Quat&, Quat, q)
PR_CTOR(const std::string&, String, s)
PR_CTOR(const Color&, Vec4, v4);

PrefabItem::PrefabItem(const Asset& a) : value({}), type(Type::Asset) {
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
PrefabItem::PrefabItem(const Prefab_ObjRef& s) : value({}), type(Type::SceneObject) {
	value.scobjref = s;
}
PrefabItem::PrefabItem(const Component& c) : value({}), type(Type::Component) {
	//auto& cv = value.compref;
	//cv.obj = Prefab_ObjRef(c->object(), ChokoEditor::scene->objects()[1]);
	//cv.type = c->componentType;

}
PrefabItem::PrefabItem(PrefabItemGroup g) : value({}), type(Type::ItemGroup) {
	value.group = std::move(g);
}
PrefabItem::PrefabItem(PrefabObjGroup g) : value({}), type(Type::ObjGroup) {
	value.objgroup = std::move(g);
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

PrefabItem::PrefabItem(const std::string& tp, const JsonObject& vl) : value({}) {
	for (size_t i = 0; i < sizeof(CE_ES_TypeS)/sizeof(std::string); i++) {
		if (tp == CE_ES_TypeS[i]) {
			type = (Type)i;

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
				if(it != AssetTypeStr.end()) {
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
					auto ress = PrefabItem(ky.substr(lc+1), g2.value);
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
				CE_NOT_IMPLEMENTED
				break;
			}

			return;
		}
	}
	Debug::Warning("PrefabItem", "Unknown type: \"" + tp + "\"!");
}

/* Export to json object
 * "name.type": (contents)
 */

JsonPair PrefabItem::ToJson(const std::string& nm) const {
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