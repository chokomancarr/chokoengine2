#include "asset_loader.hpp"

CE_BEGIN_MOD_AE_NAMESPACE

CE_MOD_AE_IMPL(AnimGraph) {
	const auto meta = LoadMeta(path);
	const auto data = JsonParser::Parse(IO::ReadFile(_basePath + path));
	auto res = AnimGraph::New();

	const auto& prms = data.Get("params");
	std::vector<_AnimGraph::Var> vars = {};
	for (auto& prm : prms.group) {
		_AnimGraph::Var var = {};
		var.name = prm.key.string;
		const auto& vl = prm.value.group[0];
		if (vl.key.string == "bool") {
			var.type = _AnimGraph::Var::Type::Bool;
			var.defaultValue.b = vl.value.ToBool();
		}
		else if (vl.key.string == "int") {
			var.type = _AnimGraph::Var::Type::Int;
			var.defaultValue.i = vl.value.ToInt();
		}
		else if (vl.key.string == "float") {
			var.type = _AnimGraph::Var::Type::Float;
			var.defaultValue.f = vl.value.ToFloat();
		}
		vars.push_back(var);
	}
	res->vars(vars);

	res->defaultState = data.Get("default_state").ToInt();
	const auto& stts = data.Get("states");
	for (auto& stt : stts.group) {
		auto& nd = res->AddNode();
		nd->name(stt.key.string);
		nd->clip(AssetLoader::Get<AnimClip>(AssetType::AnimClip, stt.value.Get("clip").string, async));
		nd->speed(stt.value.Get("speed").ToFloat());
	}

	return res;
}

CE_END_MOD_AE_NAMESPACE
