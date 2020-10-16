#include "chokoeditor.hpp"
#include "ext/ui_ext.hpp"
#include "ui/windows/inspector/status/comp/particle_system.hpp"
#include "ui/ew_macros.hpp"

CE_BEGIN_ED_NAMESPACE

CE_E_BEGIN_DRAWCOMP(ParticleSystem)

UI_Ext::Layout::Block("Simulation (Preview)", lt, [&]() {
	bool running = !!(c->callbackMask() & CompCallbackFlag::EXEC_IN_EDIT_MODE);
	if (UI::I::Button(Rect(lt.x, lt.y, lt.w / 2 - 1, 16), Color(0.3f),
			running ? "stop" : "start") == InputMouseStatus::HoverUp) {
		running = !running;
		c->callbackMask(c->callbackMask() ^ CompCallbackFlag::EXEC_IN_EDIT_MODE);
	}
	CE_E_INC_Y();
});

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

UI_Ext::Layout::Block("Attributes", lt, [&]() {
	CE_E_LBL("size");
	{
		bool tg = UI::I::Button(CE_E_VL_RECT, Color(0.2f)) == InputMouseStatus::HoverUp;
		if (UI_Ext::Layout::GetNextBlock(lt).expanded) {
			UI::Texture(Rect(lt.x + lt.w - 18, lt.y, 16, 16), EIcons::icons["close"]);
		}
		CE_E_INC_Y();
		UI_Ext::Layout::IBlock(tg, lt, [&]() {
			UI_Ext::Editor::FCurve(lt, c->lifetimeSize());
		}, CE_UI_BLOCK_CLOSED);
	}

	CE_E_LBL("color");
	{
		bool tg = UI::I::Button(CE_E_VL_RECT, Color(0.2f)) == InputMouseStatus::HoverUp;
		UI::Texture(CE_E_VL_RECT.sub(1, 1, 1, 1), c->lifetimeColor().GetLookupTexture());
		if (UI_Ext::Layout::GetNextBlock(lt).expanded) {
			UI::Texture(Rect(lt.x + lt.w - 18, lt.y, 16, 16), EIcons::icons["close"]);
		}
		CE_E_INC_Y();
		UI_Ext::Layout::IBlock(tg, lt, [&]() {
			UI_Ext::Editor::Gradient(lt, c->lifetimeColor());
		}, CE_UI_BLOCK_CLOSED);
	}
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
		EW_I_Asset::DrawMaterial2(m, lt);
	});
}

CE_E_END_DRAWCOMP

CE_END_ED_NAMESPACE