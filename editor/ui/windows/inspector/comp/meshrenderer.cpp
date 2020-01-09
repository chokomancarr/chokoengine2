#include "chokoeditor.hpp"
#include "ext/ui_ext.hpp"
#include "ui/windows/inspector/status/comp/meshrenderer.hpp"
#include "ui/ew_macros.hpp"

CE_BEGIN_ED_NAMESPACE

CE_E_BEGIN_DRAWCOMP(MeshRenderer)
	CE_E_ASSET_REF("mesh", c->mesh);

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
				CE_E_ASSET_REF("shader", m->shader);
				for (auto& v : m->variables()) {
					switch (v.type()) {
					case ShaderVariableType::Float: {
						CE_E_EDIT_F_FV(v., v.name(), val_f);
						break;
					}
					case ShaderVariableType::Color: {
						CE_E_EDIT_CL_FV(v., v.name(), val_v4);
						break;
					}
					case ShaderVariableType::Texture: {
						CE_E_LBL(v.name());
						if (UI::I::Button(CE_E_VL_RECT.sub(0, 0, 34, 0), UIButtonStyle(Color(0.2f)), CE_E_ASSET_SIG(v.val_t())) == InputMouseStatus::HoverUp) {
							EO_SelectRef::RegAsset(v.val_t(), std::function<void(const Texture&)>([&](const Texture& tx) {
								v.val_t(tx);
							}));
						}
						else CE_E_ASSET_DROP(CE_E_VL_RECT.sub(0, 0, 34, 0), AssetType::Texture, v.val_t((Texture)res))
						UI::Texture(Rect(lt.x + lt.w - 35, lt.y, 16, 16), v.val_t());
						CE_E_ASSET_SEEK_BTN();
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