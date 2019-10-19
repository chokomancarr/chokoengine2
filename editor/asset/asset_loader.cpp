#include "chokoeditor.hpp"
#include "ce2/parsers/mesh.hpp"
#include "exporters/blender.hpp"
#include "templates/meta/meta_common.hpp"

CE_BEGIN_ED_NAMESPACE

JsonObject EAssetLoader::LoadMeta(const std::string& path) {
	auto obj = JsonParser::Parse(IO::ReadFile(ChokoEditor::assetPath + path + ".meta"));
	if (obj.group[0].key.string != "signature") {
		Debug::Error("Asset MetaData", "Signature entry missing!");
	}
	if (obj.group[0].value.string != CE_E_META_SIGNATURE) {
		Debug::Error("Asset MetaData", "Signature is incorrect!");
	}
	if (obj.group[1].key.string != "version") {
		Debug::Error("Asset MetaData", "Version entry missing!");
	}
	//check for version compat here
	return obj;
}

SceneObject EAssetLoader::JsonToObject(const JsonObject& data, bool async) {
	auto obj = SceneObject::New();
	for (auto& g : data.group) {
		const auto& k = g.key.string;
		const auto& v = g.value;
		if (k == "name") {
			obj->name(v.string);
		}
		else if (k == "position") {
			obj->transform()->localPosition(v.ToVec3());
		}
		else if (k == "rotation") {
			obj->transform()->localRotation(v.ToQuat());
		}
		else if (k == "scale") {
			obj->transform()->localScale(v.ToVec3());
		}
		else if (k == "components") {
			for (auto& c : v.group) {
				#define CE_E_SW(nm) if (c.key.string == #nm) {\
					Load ## nm(c.value, obj, async);\
				}
				CE_E_SW(MeshRenderer)
				CE_E_SW(Rig)
			}
		}
		else if (k == "children") {
			for (auto& c : v.group) {
				if (c.key.string == "object") {
					JsonToObject(c.value, async)->parent(obj);
				}
			}
		}
	}
	return obj;
}

std::vector<Bone> EAssetLoader::LoadBones(const JsonObject& data) {
	std::vector<Bone> res;
	res.reserve(data.group.size());
	for (auto& g : data.group) {
		Bone b(g.key.string);
		for (auto& p : g.value.group) {
			if (p.key.string == "head") {
				b.base = p.value.ToVec3();
			}
			else if (p.key.string == "tail") {
				b.length = p.value.ToVec3();
			}
			else if (p.key.string == "front") {
				b.front = p.value.ToVec3();
			}
			else if (p.key.string == "connected") {
				b.connected = p.value.ToBool();
			}
			else if (p.key.string == "children") {
				b.children = LoadBones(p.value);
			}
		}
		b.length -= b.base;
		res.push_back(b);
	}
	return res;
}

#define CE_E_MKM(t, nm) case t::nm: {\
	std::ofstream strm(ChokoEditor::assetPath + path + ".meta");\
	strm << meta::nm;\
	break;\
}

void EAssetLoader::GenDefaultScriptMeta(const std::string& path) {
	std::ofstream strm(ChokoEditor::assetPath + path + ".meta");
	strm << meta::Script;
}

void EAssetLoader::GenDefaultMeta(const std::string& path, const EAssetType t) {
	switch (t) {
		CE_E_MKM(EAssetType, AnimClip)
		CE_E_MKM(EAssetType, Armature)
		CE_E_MKM(EAssetType, Material)
		CE_E_MKM(EAssetType, Mesh)
		CE_E_MKM(EAssetType, Shader)
		CE_E_MKM(EAssetType, VShader)
		CE_E_MKM(EAssetType, Texture)
		CE_E_MKM(EAssetType, SceneObject)
		default:
			break;
	}
}

void EAssetLoader::GenDefaultMeta(const std::string& path, const EExportType t) {
	switch (t) {
		CE_E_MKM(EExportType, Model)
		CE_E_MKM(EExportType, Image)
		default:
			break;
	}
}

#define CE_E_LD(nm) case EAssetType::nm: {\
	auto res = static_cast<Asset>(Load ## nm(path, async));\
	if (!res)\
		Debug::Error("EAssetLoader", "Failed to load \"" + path + "\" (nullptr returned by loader)!");\
	res->assetSignature(path);\
	res->name(StrExt::RemoveExt(StrExt::RemoveFd(path)));\
	return res; }

Asset EAssetLoader::Load(const std::string& path, const EAssetType t, bool async) {
	switch (t) {
		CE_E_LD(AnimClip)
		CE_E_LD(Armature)
		CE_E_LD(Material)
		CE_E_LD(Mesh)
		CE_E_LD(Shader)
		CE_E_LD(VShader)
		CE_E_LD(Texture)
		CE_E_LD(SceneObject)
		default:
			break;
	}
	return nullptr;
}

#undef CE_E_LD
#define CE_E_EX(nm) case EExportType::nm:\
	result = Export ## nm(path);\
	break;

bool EAssetLoader::Load(const std::string& path, const EExportType t) {
	bool result;
	switch (t) {
		CE_E_EX(Model)
		CE_E_EX(Image)
		default:
			return false;
	}
	if (!result) {
		Debug::Warning("Asset Loader", "Exporter operation failed!");
	}
	return result;
}

#undef CE_E_EX

CE_E_AL_IMPL(AnimClip) {
	const auto meta = LoadMeta(path);
	std::ifstream strm(ChokoEditor::assetPath + path, std::ios::binary);
	char sig[5] = {};
	strm.read(sig, 4);
	if (!!std::strcmp(sig, "ANIM")) {
		Debug::Error("AssetLoader", "AnimClip signature incorrect!");
		return nullptr;
	}
	uint16_t n, r1, r2;
	strm.read((char*)&n, 2);
	strm.read((char*)&r1, 2);
	strm.read((char*)&r2, 2);
	std::vector<_AnimClip::Entry> es(n);
	for (auto& e : es) {
		std::getline(strm, e.signature, '\0');
		std::vector<_AnimClip::Value> trs;
		for (uint16_t a = r1; a <= r2; a++) {
			Vec3 v31;
			strm.read((char*)&v31, sizeof(Vec3));
			Quat q;
			strm.read((char*)&q, sizeof(Quat));
			Vec3 v32;
			strm.read((char*)&v32, sizeof(Vec3));
			trs.push_back(_AnimClip::Value(v31, q, v32));
		}
		e.values = Curve<_AnimClip::Value>(trs, r1, r2);
	}
	auto clip = AnimClip::New();
	clip->entries(es);
	clip->range(Int2(r1, r2));
	return clip;
}

CE_E_AL_IMPL(Armature) {
	const auto meta = LoadMeta(path);
	const auto data = JsonParser::Parse(IO::ReadFile(ChokoEditor::assetPath + path));
	if (data.group[0].key.string != "armature") {
		Debug::Error("AssetLoader", "Armature entry missing!");
		return nullptr;
	}
	auto arm = Armature::New();
	arm->bones(LoadBones(data.group[0].value));
	return arm;
}

CE_E_AL_IMPL(Material) {
	const auto meta = LoadMeta(path);
	const auto data = JsonParser::Parse(IO::ReadFile(ChokoEditor::assetPath + path));
	if (data.group[0].key.string != "shader") {
		Debug::Error("AssetLoader::Material", "shader entry missing!");
		return nullptr;
	}
	auto shad = static_cast<Shader>(EAssetList::Get(EAssetType::Shader, data.group[0].value.string));
	auto mat = Material::New();
	mat->shader(shad);
	for (auto& d : data.group) {
		std::string vrnm = "";
		JsonObject vrvl;
		for (auto& d2 : d.value.group) {
			if (d2.key.string == "name")
				vrnm = d2.value.string;
			else if (d2.key.string == "value")
				vrvl = d2.value;
		}
		if (!vrnm.size()) continue;
		#define CE_E_ME(tp) if (d.key.string == #tp) {\
			mat->SetUniform(vrnm, vrvl.To ## tp());\
		}
		CE_E_ME(Float)
		else CE_E_ME(Color)
		else if (d.key.string == "Texture") {
			mat->SetUniform(vrnm, static_cast<Texture>(EAssetList::Get(EAssetType::Texture, vrvl.string, async)));
		}
	}
	return mat;
}

CE_E_AL_IMPL(Mesh) {
	const auto meta = LoadMeta(path);
	const auto ext = StrExt::ExtensionOf(path);
	if (ext == "obj") {
		return MeshLoader::LoadObj(ChokoEditor::assetPath + path);
	}
	else if (ext == "mesh") {
		return MeshLoader::LoadMesh(ChokoEditor::assetPath + path);
	}
	abort(); //we should never get here
}

CE_E_AL_IMPL(Shader) {
	const auto meta = LoadMeta(path);
	std::string nm, vs, fs;
	bool tr = false;
	const auto data = JsonParser::Parse(IO::ReadFile(ChokoEditor::assetPath + path));
	JsonObject vrs;
	for (auto& d : data.group) {
		if (d.key.string == "name") {
			nm = d.value.string;
		}
		if (d.key.string == "type") {
			if (d.value.string == "Transparent") tr = true;
			else if (d.value.string != "Opaque") {
				Debug::Warning("Shader Asset Loader", "unknown \"type\" value: \"" + d.value.string + "\" (accepted values: \"Opaque\", \"Transparent\")!");
			}
		}
		if (d.key.string == "variables") {
			vrs = d.value;
		}
		else if (d.key.string == "vertex") {
			vs = d.value.string;
		}
		else if (d.key.string == "fragment") {
			fs = d.value.string;
		}
	}
	auto shd = Shader::New(vs, fs);
	shd->name(nm);
	shd->queue(tr ? ShaderQueue::Transparent : ShaderQueue::Opaque);
	shd->RegisterStandardUniforms();
	for (auto v : vrs.group) {
		#define CE_E_SHV(nm) if (v.value.string == #nm) {\
			shd->AddUniform(v.key.string, ShaderVariableType::nm);\
		}
		CE_E_SHV(Float)
		else CE_E_SHV(Color)
		else CE_E_SHV(Texture)
		else CE_E_SHV(CubeMap)
	}
	return shd;
}

CE_E_AL_IMPL(VShader) {
	const auto meta = LoadMeta(path);
	const auto data = JsonParser::Parse(IO::ReadFile(ChokoEditor::assetPath + path));
	auto shad = VShader::New();
	for (auto& n : data.group) {
		shad->nodes.push_back(_VShaderNode::FromSig(n.key.string));
	}
	for (size_t a = 0, n = data.group.size(); a < n; a++) {
		shad->nodes[a]->Parse(data.group[a].value, shad);
	}
	return shad;
}

CE_E_AL_IMPL(Texture) {
	const auto meta = LoadMeta(path);
	auto opts = TextureOptions();
	for (auto& g : meta.group) {
		if (g.key.string == "xwrap")
			opts.xwrap = g.value.ToEnum<TextureWrap>({ "Clamp", "Repeat", "Mirror" });
		else if (g.key.string == "ywrap")
			opts.ywrap = g.value.ToEnum<TextureWrap>({ "Clamp", "Repeat", "Mirror" });
		else if (g.key.string == "mipmaps")
			opts.mipmaps = std::stoi(g.value.string);
		else if (g.key.string == "linear")
			opts.linear = g.value.ToBool();
	}
	return Texture::New(ChokoEditor::assetPath + path, opts, async);
}

CE_E_AL_IMPL(SceneObject) {
	const auto meta = LoadMeta(path);
	const auto data = JsonParser::Parse(IO::ReadFile(ChokoEditor::assetPath + path));
	if (data.group[0].key.string != "object") {
		Debug::Error("AssetLoader::LoadPrefab", "Object entry missing!");
	}
	return JsonToObject(data.group[0].value, async);
}

CE_E_AL_IMPL_EX(Model) {
	const auto meta = LoadMeta(path);
	const auto ext = StrExt::ExtensionOf(path);
	if (ext == "blend") {
		return BlenderExporter::ExportBlend(ChokoEditor::assetPath + path, ChokoEditor::assetPath, ".exported/" + path + "/");
	}
	else abort(); //we should never get here
}

CE_E_AL_IMPL_EX(Image) {
	const auto meta = LoadMeta(path);
	const auto ext = StrExt::ExtensionOf(path);
	if (ext == "psd") {
		return BlenderExporter::ExportImage(ChokoEditor::assetPath + path, ChokoEditor::assetPath + ".exported/" + path + "/");
	}
	else abort(); //we should never get here
}

CE_END_ED_NAMESPACE