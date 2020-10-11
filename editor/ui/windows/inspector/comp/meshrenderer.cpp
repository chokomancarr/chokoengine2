#include "chokoeditor.hpp"
#include "ext/ui_ext.hpp"
#include "ui/windows/inspector/status/comp/meshrenderer.hpp"
#include "ui/ew_macros.hpp"

CE_BEGIN_ED_NAMESPACE

CE_E_BEGIN_DRAWCOMP(MeshRenderer)
	CE_E_ASSET_REF("mesh", c->mesh, c);

	UI_Ext::Layout::Block("Modifiers", lt, [&]() {
		for (auto& m : c->modifiers()) {
			UI_Ext::Layout::Block(m->name(), lt, [&]() {
				switch (m->type) {
				case MeshModifierType::Shape: {
					auto md = static_cast<MeshShapeModifier>(m);
					const auto& ss = c->mesh()->shapeKeys();
					for (size_t a = 0, n = ss.size(); a < n; a++) {
						auto& w = md->weights()[a];
						UI::Label(Rect(lt.x + 2, lt.y, lt.w, 17), ss[a].name, Color::white());
						lt.y += 16;
					}
					break;
				}
				case MeshModifierType::Skin: {
					auto md = static_cast<MeshSkinModifier>(m);
					CE_E_LBL("rig");
					if (UI::I::Button(CE_E_VL_RECT.sub(0, 0, 17, 0), UIButtonStyle(Color(0.2f)),
							!md->rig() ? "none" :
							md->rig()->object()->name() + " (" + ComponentTypeStr.at(md->rig()->componentType) + ")"
							) == InputMouseStatus::HoverUp) {
						
					}
					CE_E_INC_Y();
					break;
				}
				default:
					break;
				}
			});
		}

		UI::I::Button(Rect(lt.x + 2, lt.y, lt.w - 4, 16), UIButtonStyle(0.2f), "Add");
		lt.y += 17;
	});

	UI_Ext::Layout::Block("Materials", lt, [&]() { //tmp
		std::vector<Material*> mats(0);
		for (auto& m : c->materials()) {
			auto& _m = (Material&)m;
			CE_E_ASSET_REF_FV("slot", _m);
			if (!m) continue;
			if (std::find_if(mats.begin(), mats.end(), [&](const Material* m2) {
				return m2 == &m;
			}) == mats.end()) {
				mats.push_back((Material*)&m);
			}
		}
		for (auto _m : mats) {
			auto& m = *_m;
			UI_Ext::Layout::Block(m->name() + " (Material)", lt, [&]() {
				EW_I_Asset::DrawMaterial2(m, lt);
			});
		}
	});
CE_E_END_DRAWCOMP

CE_END_ED_NAMESPACE