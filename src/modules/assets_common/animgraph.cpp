#include "inc.hpp"

CE_BEGIN_MOD_AC_NAMESPACE

namespace {
	inline AnimClip GetClip(const std::string& nm) {
		return (AnimClip)CE_PL_NS _AssetLoaderBase::instance
			->Load(AssetType::AnimClip, nm);
	}
}

AnimGraph LoadAnimGraph(const JsonObject& data) {
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
		nd.name = stt.key.string;
		nd.type = stt.value.Get("type").ToEnum<_AnimGraph::Node::Type>({ "Single", "Linear1D" });
		nd.clip(GetClip(stt.value.Get("clip").string));
		const auto& c1d = stt.value.Get("clips1D");
		for (auto& c1 : c1d.group) {
			nd.clips1D().push_back(std::make_pair(c1.key.ToFloat(), GetClip(c1.value.string)));
		}
		nd.var1D = stt.value.Get("var1D").ToInt();
		nd.speed = stt.value.Get("speed").ToFloat();
		nd.repeat = stt.value.Get("repeat").ToBool();
	}

	const auto& trss = data.Get("transitions");
	for (auto& trs : trss.group) {
		std::vector<_AnimGraph::Link> links = {};
		links.reserve(trss.group.size());
		const int from = trs.key.ToInt();
		for (auto& tr : trs.value.group) {
			_AnimGraph::Link lnk = {};
			lnk.target = tr.key.ToInt();
			lnk.length = tr.value.Get("length").ToFloat();
			lnk.offset = tr.value.Get("offset").ToFloat();
			lnk.useExitLength = tr.value.Get("on_exit").ToBool();
			lnk.exitLength = tr.value.Get("exit_length").ToFloat();

			const auto& cnds = tr.value.Get("conditions").list;
			lnk.conditions.reserve(cnds.size());

			for (auto& cnd : cnds) {
				lnk.conditions.push_back({});
				auto& cc = lnk.conditions.back();
				for (auto& cn : cnd.group) {
					_AnimGraph::Link::Cond c = {};
					c.paramId = cn.key.ToInt();

					typedef _AnimGraph::Link::Cond::Comp _CTp;
					const std::string _compstrs[] = {
						"Equals",
						"NEquals",
						"Greater",
						"Less",
						"GEquals",
						"LEquals"
					};
					for (int a = 0; a < (int)_CTp::_COUNT; a++) {
						if (cn.value.list[0].string == _compstrs[a]) {
							c.comp = (_CTp)a;
							break;
						}
					}

					typedef _AnimGraph::Var::Type _Tp;
					switch (res->vars()[c.paramId].type) {
					case _Tp::Bool:
						c.value.b = cn.value.list[1].ToBool();
						break;
					case _Tp::Int:
						c.value.b = cn.value.list[1].ToInt();
						break;
					case _Tp::Float:
						c.value.b = cn.value.list[1].ToFloat();
						break;
					}
					cc.push_back(c);
				}
			}

			links.push_back(lnk);
		}
		res->nodes()[from].links = links;
	}

	return res;
}

CE_END_MOD_AC_NAMESPACE