#include "chokoeditor.hpp"
#include "ui/windows/inspector/status/comp/meshrenderer.hpp"
#include "ui/ew_macros.hpp"
#include "ext/ui_ext.hpp"

CE_BEGIN_ED_NAMESPACE
/*
CE_E_BEGIN_DRAWCOMP(MeshRenderer)
	static float dy = 0;
	const float y0 = r.y() + 17;
	UI::Rect(Rect(r.x() + 4, r.y(), r.w() - 8, 17), Color(0.1f, 0.7f));
	UI::Rect(Rect(r.x() + 4, y0, r.w() - 8, dy), Color(0.1f, 0.3f));
	UI::Label(Rect(r.x() + 10, r.y(), r.w() * 0.33f, 16), "Modifiers", Color::white());
	r.y(r.y() + 17);
	for (auto& m : c->modifiers()) {
		UI::Label(Rect(r.x() + 22, r.y(), r.w() - 28, 16), m->name(), Color::white());
		r.y(r.y() + 17);
		switch (m->type) {
			case MeshModifierType::Shape: {
				auto md = static_cast<MeshShapeModifier>(m);
				const auto& ss = c->mesh()->shapeKeys();
				for (size_t a = 0, n = ss.size(); a < n; a++) {
					auto& w = md->weights()[a];
					CE_E_EDIT_ST(F, , ss[a].name, w, CE_E_GETVAL,
						md->SetWeight(a, Math::Clamp<float>(res_w, 0, 1)));
				}
				break;
			}
			default:
				break;
		}
	}
	dy = r.y() - y0;
	r.y(r.y() + 1);
	static float dy2 = 0;
	const float y1 = r.y() + 17;
	UI::Rect(Rect(r.x() + 4, r.y(), r.w() - 8, 17), Color(0.1f, 0.7f));
	UI::Rect(Rect(r.x() + 4, y1, r.w() - 8, dy2), Color(0.1f, 0.3f));
	UI::Label(Rect(r.x() + 10, r.y(), r.w() * 0.33f, 16), "Materials", Color::white());
	r.y(r.y() + 17);
	for (auto& m : c->materials()) {
		UI::Label(Rect(r.x() + 22, r.y(), r.w() - 28, 16), m->shader()->name(), Color::white());
		r.y(r.y() + 17);
	}
	dy2 = r.y() - y1;
	r.y(r.y() + 1);
CE_E_END_DRAWCOMP
*/

CE_E_BEGIN_DRAWCOMP(MeshRenderer)
	UI_Ext::Layout::Push("Modifiers", lt);
	for (auto& m : c->modifiers()) {
		UI_Ext::Layout::Push(m->name(), lt);
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
		UI_Ext::Layout::Pop(lt);
	}
	UI_Ext::Layout::Pop(lt);
CE_E_END_DRAWCOMP

CE_END_ED_NAMESPACE