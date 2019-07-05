#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

#define SW(nm) else if (k == #nm)
void Prefab::LoadObject(const JsonObject& data, SceneObject& parent) {
    SceneObject o = SceneObject::New();
    o->parent(parent);
    for (auto& i : data.group) {
        const auto& k = i.key.string;
        const auto& v = i.value;
        if (k == "name") {
            o->name(v.string);
        }
        SW(pos) {
            Vec3 pos = Vec3(
                std::stof(v.list[0].string),
                std::stof(v.list[1].string),
                std::stof(v.list[2].string)
            );
            o->transform()->localPosition(pos);
        }
        SW(rot) {
            Quat rot = Quat(
                std::stof(v.list[0].string),
                std::stof(v.list[1].string),
                std::stof(v.list[2].string),
                std::stof(v.list[3].string)
            );
            o->transform()->localRotation(rot);
        }
        SW(scl) {
            Vec3 scl = Vec3(
                std::stof(v.list[0].string),
                std::stof(v.list[1].string),
                std::stof(v.list[2].string)
            );
            o->transform()->localScale(scl);
        }
        SW(components) {
            for (auto& c : v.list) {
                LoadComponent(c, o);
            }
        }
        SW(children) {
            for (auto& c : v.list) {
                LoadObject(c, o);
            }
        }
    }
}

#define LD(nm) if (k == #nm) Load ## nm(v, obj->AddComponent<nm>())
#define ELD(nm) else LD(nm)
void Prefab::LoadComponent(const JsonObject& data, SceneObject& obj) {
    for (auto& c : data.group) {
        const auto& k = c.key.string;
        const auto& v = c.value;

        LD(MeshRenderer);
    }
}

void Prefab::LoadMeshRenderer(const JsonObject& data, MeshRenderer c) {
    for (auto& p : data.group) {
        const auto& k = p.key.string;
        const auto& v = p.value;
        if (k == "mesh") {

        }
        SW(modifiers) {

        }
        SW(materials) {

        }
    }
}

SceneObject Prefab::Load(const std::string& path) {
    const auto p = ChokoEditor::assetPath + path;
    const auto data = JsonParser::Parse(p);
    auto pn = p.substr(p.find_last_of('/') + 1);
    pn = pn.substr(0, pn.size() - EXT_PREFAB.size());
    auto o = SceneObject::New(pn);
    for (auto& g : data.group) {
        if (g.key.string == "object") {
            LoadObject(g.value, o);
        }
    }
}

CE_END_ED_NAMESPACE