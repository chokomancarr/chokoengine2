#include "chokoeditor.hpp"
#include "ce2/modules/ae/asset_loader.hpp"

CE_BEGIN_ED_NAMESPACE

namespace {
	EDropdownMenu menu_empty;

	constexpr int NW = 120;
	constexpr int NH = 40;
	const Vec2 NWH(NW, NH);
}

void EW_AnimGrapher::RegAsset(AnimGraph g) {
	if (_graph == g) return;

	_graph = g;

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

void EW_AnimGrapher::DrawMenu() {
	if (ESceneInfo::selectedType == ESceneInfo::SelType::Asset
			&& ESceneInfo::selectedAsset->assetType == AssetType::AnimGraph) {
		RegAsset((AnimGraph)ESceneInfo::selectedAsset);
	}
	
	if (!_graph) return;

	static EUILayout::PanState st = {};
	const auto rect2 = position.sub(1, 20, 1, 1);
	Vec2 off = EUILayout::BeginPan(rect2, Rect(0, 0, 300, 200), st);
	UI::defaultFont()->alignment(FontAlign::TopCenter);

	for (size_t a = 0; a < _nodes.size(); a++) {
		const auto& nd = _graph->nodes()[a];
		const auto& _nd = _nodes[a];
		const auto& lks = nd->links();
		for (auto& lk : lks) {
			UI::Line(off + _nd.position + NWH * 0.5f, off + _nodes[lk.target].position + NWH * 0.5f, Color::red());
		}
	}
	
	for (size_t a = 0; a < _nodes.size(); a++) {
		const auto& nd = _graph->nodes()[a];
		const auto& _nd = _nodes[a];
		UI::Rect(Rect(off.x + _nd.position.x, off.y + _nd.position.y, NW, NH), Color(0.5f, 0.7f));
		UI::Label(Rect(off.x + _nd.position.x + NW / 2, off.y + _nd.position.y + 2, NW, 16), nd->name(), Color::white());
	}

	if (rect2.Contains(Input::mousePosition())) {
		if (Input::mouseStatus(InputMouseButton::Right) == InputMouseStatus::Up) {
			EO_Dropdown::Reg(Input::mousePosition() + Vec2(1, 1), menu_empty, false);
		}
	}

	UI::defaultFont()->alignment(FontAlign::TopLeft);
	EUILayout::EndPan(st);
}

EW_AnimGrapher::EW_AnimGrapher() : EWindow("Animation Graph Editor") {}

bool EW_AnimGrapher::Init() {
#define addi(nm, ...) {auto op = EDropdownMenu(#nm);\
		op.callback = ECallbackCaller(__VA_ARGS__);\
		menu->push_back(op);}

	auto menu = &menu_empty.items;

	addi(New State, CallbackSig::ANIMGRAPH_STATE_NEW);

	Ops::Reg();

	return true;
}

CE_END_ED_NAMESPACE