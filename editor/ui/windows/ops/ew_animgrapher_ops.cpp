#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_AnimGrapher::Ops::Reg() {
	ECallbackManager::Register(CallbackSig::ANIMGRAPH_STATE_NEW, StateAddNew);
}

CE_IMPL_CALLBACK(EW_AnimGrapher::Ops::StateAddNew) {
	const auto wnd = (EW_AnimGrapher*)window;
	wnd->_graph->AddNode();
	Node n = {};
	n.isDefault = false;

	wnd->_nodes.push_back(n);
}

CE_END_ED_NAMESPACE