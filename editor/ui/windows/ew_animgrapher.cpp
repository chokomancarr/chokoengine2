#include "chokoeditor.hpp"
#include "ui/ew_macros.hpp"
#include "ce2/modules/ae/asset_loader.hpp"

CE_BEGIN_ED_NAMESPACE

namespace {
	EDropdownMenu menu_node;
	EDropdownMenu menu_empty;

	constexpr int NW = 120;
	constexpr int NH = 30;
	const Vec2 NWH(NW, NH);
}

void EW_AnimGrapher::RegAsset(AnimGraph g) {
	if (_graph == g) return;

	_graph = g;

	inspNode = -1;
	linkSrcNode = -1;

	const auto meta = ModuleAE::AssetLoader::LoadMeta(g->assetSignature());

	const auto& obj = meta.Get("node_info");
	const auto& nodes = g->nodes();

	const auto sz = nodes.size();
	_nodes.clear();
	_nodes.resize(sz);
	_nodes[g->defaultState].isDefault = true;
	for (size_t a = 0; a < sz; a++) {
		_nodes[a].position = obj.group[a].value.ToVec2();
	}
}

void EW_AnimGrapher::AddState(const Vec2& pos) {
	Node n = {};
	n.position = pos;
	_nodes.push_back(n);

	_graph->AddNode();
}

void EW_AnimGrapher::DrawInspectMenu(const Rect& position) {
	auto& nd = _graph->nodes()[inspNode];
	const auto pnd = &nd;
	
	static auto lt = UI_Ext::Layout::InfoSt();
	UI_Ext::Layout::BeginLayout(position.sub(2, 20, 2, 0), lt);

	CE_E_LBL("name");
	nd.name = UI::I::TextField(CE_E_VL_RECT, nd.name, Color());
	CE_E_INC_Y();

	CE_E_LBL("type: single");
	CE_E_INC_Y();

	CE_E_ASSET_REF("clip", pnd->clip, pnd);

	CE_E_EDIT_F(nd., "speed", speed);

	CE_E_LBL("transitions");
	CE_E_INC_Y();
	for (auto& tr : nd.links) {
		UI_Ext::Layout::Block("-> " + _graph->nodes()[tr.target].name, lt, [&]() {
			CE_E_EDIT_TG(tr., "trigger on exit", useExitLength);
			CE_E_EDIT_F(tr., "duration", length);

			CE_E_LBL("conditions");
			CE_E_INC_Y();
			for (auto& cds : tr.conditions) {
				UI_Ext::Layout::Block("", lt, [&]() {
					for (auto& cd : cds) {
						const auto& vr = _graph->vars()[cd.paramId];
						CE_E_LBL(vr.name);
						const std::string condstrs[] = {
							"==", "!=", ">", "<", ">=", "<="
						};
						UI::I::Button(Rect(lt.x + 80, lt.y, 50, 16), Color(0.2f), condstrs[(int)cd.comp]);
						typedef decltype(vr.type) _CTp;
						switch (vr.type) {
						case _CTp::Bool:
							cd.value.b = UI::I::Toggle(Rect(lt.x + lt.w - 18, lt.y, 16, 16), cd.value.b, Color(0.2f));
							break;
						}
						CE_E_INC_Y();
					}
				});
			}
		});
	}

	UI_Ext::Layout::EndLayout(lt);
}

void EW_AnimGrapher::DrawMenu() {
	if (ESceneInfo::selectedType == ESceneInfo::SelType::Asset
			&& ESceneInfo::selectedAsset->assetType == AssetType::AnimGraph) {
		RegAsset((AnimGraph)ESceneInfo::selectedAsset);
	}
	else {
		_graph = pAnimGraph();
	}
	
	if (!_graph) {
		inspNode = -1;
		return;
	}

	static EUILayout::PanState st = {};
	const auto rect2 = position.sub(1, 20, 1, 1);
	Vec2 off = EUILayout::BeginPan(rect2, Rect(0, 0, 300, 200), st);
	UI::defaultFont()->alignment(FontAlign::TopCenter);

	for (size_t a = 0; a < _nodes.size(); a++) {
		const auto& nd = _graph->nodes()[a];
		const auto& _nd = _nodes[a];
		const auto& lks = nd.links;
		for (auto& lk : lks) {
			UI::Line(off + _nd.position + NWH * 0.5f, off + _nodes[lk.target].position + NWH * 0.5f, Color::red());
		}
	}
	
	bool hovered = false;
	for (size_t a = 0; a < _nodes.size(); a++) {
		const auto& nd = _graph->nodes()[a];
		const auto& _nd = _nodes[a];
		bool isi = (a == inspNode);
		if (isi) {
			UI::Rect(Rect(off.x + _nd.position.x - 1, off.y + _nd.position.y - 1, NW + 2, NH + 2), Color(0.8f));
		}
		const auto mst = UI::I::Button(Rect(off.x + _nd.position.x, off.y + _nd.position.y, NW, NH), Color(isi ? 0.3f : 0.2f));
		if (mst == InputMouseStatus::HoverUp) {
			hovered = true;
			if (linkSrcNode > -1) {
				if (linkSrcNode != a) {
					auto& ns = _graph->nodes()[linkSrcNode];
					_AnimGraph::Link lnk = {};
					lnk.target = a;
					ns.links.push_back(lnk);

					inspNode = linkSrcNode;
					EW_Inspector::customDrawer = std::bind(&EW_AnimGrapher::DrawInspectMenu, this, std::placeholders::_1);
				}
				linkSrcNode = -1;
			}
			else {
				inspNode = a;
				EW_Inspector::customDrawer = std::bind(&EW_AnimGrapher::DrawInspectMenu, this, std::placeholders::_1);
			}
		}
		else if (mst == InputMouseStatus::Hover) {
			hovered = true;
			if (Input::mouseStatus(InputMouseButton::Right) == InputMouseStatus::Up) {
				menu_node.SetAll(ECallbackArg("id", (int)a));
				EO_Dropdown::Reg(Input::mousePosition() + Vec2(1, 1), menu_node, false);
			}
		}
		UI::Label(Rect(off.x + _nd.position.x, off.y + _nd.position.y + 2, NW, 16), nd.name, Color::white());
	}

	if (!hovered && rect2.Contains(Input::mousePosition())) {
		if (Input::mouseStatus(InputMouseButton::Left) == InputMouseStatus::Up) {
			inspNode = -1;
			EW_Inspector::customDrawer = nullptr;
		}
		if (Input::mouseStatus(InputMouseButton::Right) == InputMouseStatus::Up) {
			EO_Dropdown::Reg(Input::mousePosition() + Vec2(1, 1), menu_empty, false);
		}
	}

	UI::defaultFont()->alignment(FontAlign::TopLeft);
	EUILayout::EndPan(st);
}

EW_AnimGrapher::EW_AnimGrapher() : EWindow("Animation Graph Editor") {}

bool EW_AnimGrapher::Init() {
#define addi(nm, sig) {auto op = EDropdownMenu(#nm);\
		op.callback = ECallbackCaller(sig, CallbackSigArgs.at(sig), this);\
		menu->push_back(op);}

	auto menu = &menu_node.items;
	
	addi(Add Transition, CallbackSig::ANIMGRAPH_SET_LINK_SOURCE);

	menu = &menu_empty.items;

	addi(New State, CallbackSig::ANIMGRAPH_STATE_NEW);

	Ops::Reg();

	return true;
}

CE_END_ED_NAMESPACE