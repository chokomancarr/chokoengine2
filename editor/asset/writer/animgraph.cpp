#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

#define ASSIG(nm) (!(nm) ? "none" : (nm)->assetSignature())

CE_EAW_IMPL(AnimGraph) {
	JsonObject params(JsonObject::Type::Group);
	for (auto& p : obj->vars()) {
		JsonPair pr(JsonObject(p.name), JsonObject(JsonObject::Type::Group));
		switch (p.type) {
		case _AnimGraph::Var::Type::Bool:
			pr.value.group.push_back(JsonPair(
				JsonObject("bool"), std::to_string(p.defaultValue.b)
			));
			break;
		case _AnimGraph::Var::Type::Int:
			pr.value.group.push_back(JsonPair(
				JsonObject("int"), std::to_string(p.defaultValue.i)
			));
			break;
		case _AnimGraph::Var::Type::Float:
			pr.value.group.push_back(JsonPair(
				JsonObject("float"), std::to_string(p.defaultValue.f)
			));
			break;
		}
		params.group.push_back(pr);
	}
	
	JsonObject states(JsonObject::Type::Group);
	for (auto& st : obj->nodes()) {
		const std::string tpnms[] = {
			"Single", "Linear1D"
		};
		JsonObject clips1D(JsonObject::Type::Group);
		for (auto& g : st.clips1D()) {
			clips1D.group.push_back(JsonPair(std::to_string(g.first), ASSIG(g.second)));
		}
		JsonPair pr(JsonObject(st.name), JsonObject({
			JsonPair(JsonObject("type"), tpnms[(int)st.type]),
			JsonPair(JsonObject("clip"), ASSIG(st.clip())),
			JsonPair(JsonObject("clips1D"), clips1D),
			JsonPair(JsonObject("var1D"), std::to_string(st.var1D)),
			JsonPair(JsonObject("speed"), std::to_string(st.speed)),
			JsonPair(JsonObject("repeat"), JsonObject(st.repeat ? "1" : "0"))
		}));
		states.group.push_back(pr);
	}
	
	JsonObject transitions(JsonObject::Type::Group);
	int i = 0;
	for (auto& st : obj->nodes()) {
		if (st.links.size() > 0) {
			JsonObject data(JsonObject::Type::Group);
			for (auto& lnk : st.links) {
				JsonObject cdata2(JsonObject::Type::List);
				for (auto& cnds : lnk.conditions) {
					JsonObject cdata(JsonObject::Type::Group);
					for (auto& cnd : cnds) {
						static const char* compstrs[] = {
							"Equals",
							"NEquals",
							"Greater",
							"Less",
							"GEquals",
							"LEquals"
						};

						std::string valstr;
						switch (obj->vars()[cnd.paramId].type) {
						case _AnimGraph::Var::Type::Bool:
							valstr = std::to_string(cnd.value.b);
							break;
						case _AnimGraph::Var::Type::Int:
							valstr = std::to_string(cnd.value.i);
							break;
						case _AnimGraph::Var::Type::Float:
							valstr = std::to_string(cnd.value.f);
							break;
						}

						cdata.group.push_back(JsonPair(std::to_string(cnd.paramId), 
							JsonObject({
								JsonObject(compstrs[(int)cnd.comp]),
								JsonObject(valstr)
								})));
					}

					cdata2.list.push_back(cdata);
				}

				data.group.push_back(JsonPair(std::to_string(lnk.target), JsonObject({
					JsonPair(JsonObject("length"), std::to_string(lnk.length)),
					JsonPair(JsonObject("offset"), std::to_string(lnk.offset)),
					JsonPair(JsonObject("on_exit"), JsonObject(lnk.useExitLength ? "1" : "0")),
					JsonPair(JsonObject("exit_length"), std::to_string(lnk.exitLength)),
					JsonPair(JsonObject("conditions"), cdata2)
					})));
			}

			transitions.group.push_back(JsonPair(std::to_string(i), data));
		}
		i++;
	}

	JsonObject res({
		JsonPair(JsonObject("params"), params),
		JsonPair(JsonObject("default_state"), std::to_string(obj->defaultState)),
		JsonPair(JsonObject("states"), states),
		JsonPair(JsonObject("transitions"), transitions)
	});

	std::ofstream(CE_DIR_ASSET + obj->assetSignature()) << JsonParser::Export(res);
}

CE_END_ED_NAMESPACE
