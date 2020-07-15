#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_AnimGrapher::Ops::Reg() {
	ECallbackManager::Register(CallbackSig::ANIMGRAPH_STATE_NEW, StateAddNew);
	ECallbackManager::Register(CallbackSig::ANIMGRAPH_SET_LINK_SOURCE, SetLinkSrc);
	ECallbackManager::Register(CallbackSig::ANIMGRAPH_SAVE, Save);
}

CE_IMPL_CALLBACK(EW_AnimGrapher::Ops::StateAddNew) {
	const auto wnd = (EW_AnimGrapher*)window;
	auto _n = wnd->_graph->AddNode();
	Node n = {};
	n.isDefault = false;
	

	wnd->_nodes.push_back(n);
}

CE_IMPL_CALLBACK(EW_AnimGrapher::Ops::SetLinkSrc) {
	const auto wnd = (EW_AnimGrapher*)window;
	wnd->linkSrcNode = args["id"].i;
}

CE_IMPL_CALLBACK(EW_AnimGrapher::Ops::Save) {
	const auto wnd = (EW_AnimGrapher*)window;

	EAssetWriter::Write(wnd->_graph.lock());

	JsonObject info(JsonObject::Type::Group);
	int i = 0;
	for (auto& n : wnd->_nodes) {
		info.group.push_back(JsonPair(std::to_string(i), JsonObject::FromVec2(n.position)));
		i++;
	}

	JsonObject meta({
		//we should move this to a function
		JsonPair(JsonObject("signature"), JsonObject("ChokoEngine Asset MetaData")),
		JsonPair(JsonObject("version"), JsonObject("20")),

		JsonPair(JsonObject("node_info"), info)
		});

	std::ofstream(CE_DIR_ASSET + wnd->_graph->assetSignature() + ".meta") << JsonParser::Export(meta);
}

CE_END_ED_NAMESPACE