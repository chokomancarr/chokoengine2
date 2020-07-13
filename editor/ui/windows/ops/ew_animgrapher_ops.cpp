#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_AnimGrapher::Ops::Reg() {
	ECallbackManager::Register(CallbackSig::ANIMGRAPH_STATE_NEW, StateAddNew);
	ECallbackManager::Register(CallbackSig::ANIMGRAPH_SET_LINK_SOURCE, SetLinkSrc);
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

CE_END_ED_NAMESPACE