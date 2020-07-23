#include "chokoeditor.hpp"
#include "ext/ui_ext.hpp"
#include "ui/windows/inspector/status/comp/particle_system.hpp"
#include "ui/ew_macros.hpp"

CE_BEGIN_ED_NAMESPACE

CE_E_BEGIN_DRAWCOMP(ParticleSystem)

UI_Ext::Layout::Block("Generation", lt, [&]() {
	CE_E_EDIT_F_FV(c->, "rate", emissionRate);

	CE_E_EDIT_CRV_FV(c->, "velocity", initialSpeed);
	CE_E_EDIT_CRV_FV(c->, "size", initialSize);
	CE_E_EDIT_CRV_FV(c->, "angle", initialRotation);
	CE_E_EDIT_CRV_FV(c->, "rvelocity", initialRotSpeed);
});

UI_Ext::Layout::Block("Behavior", lt, [&]() {
	CE_E_EDIT_CRV_FV(c->, "lifetime", lifetime);

	
});

UI_Ext::Layout::Block("Forces", lt, [&]() {
	CE_E_EDIT_V3_FV(c->, "force (c)", force);
	CE_E_EDIT_V3_FV(c->, "force (r)", randomForce);
});

UI_Ext::Layout::Block("Rendering", lt, [&]() {
	CE_E_ASSET_REF("material", c->material, c);
});
const auto& m = c->material();
if (!!m) { //move to common function
	UI_Ext::Layout::Block(m->name() + " (Material)", lt, [&]() {
		CE_E_ASSET_REF("shader", m->shader, m);
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

CE_E_END_DRAWCOMP

CE_END_ED_NAMESPACE