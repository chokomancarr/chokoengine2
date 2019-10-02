#include "chokoeditor.hpp"
#include "ui/windows/inspector/status/comp/meshrenderer.hpp"
#include "ui/ew_macros.hpp"
#include "ext/ui_ext.hpp"

CE_BEGIN_ED_NAMESPACE

CE_E_BEGIN_DRAWCOMP(MeshRenderer)
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
				default:
					break;
				}
			});
		}

		UI::I::Button(Rect(lt.x + 2, lt.y, lt.w - 4, 16), UIButtonStyle(0.2f), "Add");
		lt.y += 17;
	});
	UI_Ext::Layout::Block("Materials", lt, [&]() { //tmp
		std::vector<Material> mats(0);
		for (auto& m : c->materials()) {
			CE_E_LBL("slot");
			UI::Label(CE_E_VL_RECT, m->assetSignature(), Color(1));
			CE_E_INC_Y();
			if (std::find_if(mats.begin(), mats.end(), [&](const Material& m2) {
				return m2 == m;
			}) == mats.end()) {
				mats.push_back(m);
			}
		}
		for (auto& m : mats) {
			UI_Ext::Layout::Block(m->name() + " (Material)", lt, [&]() {
				CE_E_LBL("shader");
				UI::Label(CE_E_VL_RECT, m->shader()->name(), Color(0.7f));
				CE_E_INC_Y();
				for (auto& v : m->variables()) {
					switch (v.type()) {
					case ShaderVariableType::Float: {
						CE_E_EDIT_F_FV(v., v.name(), val_f);
						break;
					}
					case ShaderVariableType::Color: {
						CE_E_LBL(v.name());
						UI::Rect(CE_E_VL_RECT, v.val_v4());
						CE_E_INC_Y();
						break;
					}
					case ShaderVariableType::Texture: {
						CE_E_LBL(v.name());
						UI::Texture(Rect(lt.x + lt.w - 17, lt.y, 16, 16), v.val_t());
						CE_E_INC_Y();
						break;
					}
					default:
						break;
					}
				}
			});
		}
	});
CE_E_END_DRAWCOMP

CE_END_ED_NAMESPACE