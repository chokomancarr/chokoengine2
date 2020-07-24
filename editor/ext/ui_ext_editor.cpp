#pragma once
#include "chokoeditor.hpp"
#include "ui_ext.hpp"
#include "ui/ew_macros.hpp"

CE_BEGIN_ED_NAMESPACE

void UI_Ext::Editor::Gradient(Layout::InfoSt& lt, CE_NS Gradient& gd) {
	static CE_NS Gradient* _egd = nullptr;
	static Gradient::Entry* _egdi, *_egdil;
	static float _eoff;

	bool changed = false;
	bool exists = false;

	const float h = lt.w / 6;
	UI::Texture(Rect(lt.x + 2, lt.y + 2, lt.w - 4, h), gd.GetLookupTexture());
	for (auto& p : gd.entries) {
		const float x = lt.x + 2 + (lt.w - 4) * p.position;
		bool ia = _egdi == &p;
		bool ial = ia || (!ia && (_egdil == &p));
		exists |= ial;

		UI::Line(Vec2(x, lt.y + 2), Vec2(x, lt.y + 2 + h), Color(1, ia ? 1 : 0.7f));
		if (UI::I::Button(Rect(x - 3, lt.y + 2 + h, 6, 10), Color(ial ? 0.8f : 0.2f)) == InputMouseStatus::HoverDown) {
			_egd = &gd;
			_egdi = _egdil = &p;
			ia = true;
			_eoff = Math::ILerp(lt.x + 2, lt.x + lt.w - 2, Input::mousePosition().x, true) - p.position;
		}
		UI::Rect(Rect(x - 2, lt.y + 3 + h, 4, 8), p.value);
		if (ia) {
			if (Input::mouseStatus(InputMouseButton::Left) == InputMouseStatus::Up) {
				ia = false;
				_egdi = nullptr;
			}
			else {
				p.position = Math::ILerp(lt.x + 2, lt.x + lt.w - 2, Input::mousePosition().x, true) - _eoff;
				changed = true;
			}
		}
	}
	lt.y += h + 16;

	if (exists) {
		auto pn = UI_Ext::TextFieldF(Rect(lt.x + 3, lt.y, CE_E_LBL_W - 3, 16), _egdil->position);
		if (pn != _egdil->position) {
			_egdil->position = pn;
			changed = true;
		}
		if (UI::I::Button(CE_E_VL_RECT, _egdil->value) == InputMouseStatus::HoverUp) {
			const auto pgd = &gd;
			const auto pp = _egdil;
			EO_ColorPicker::Reg(CE_E_VL_RECT, _egdil->value, std::function<void(const Color&)>([pgd, pp](const Color& vl) {
				pp->value = vl;
				pgd->Apply();
			}));
		}
		CE_E_INC_Y();
	}
	else {
		UI::Label(Rect(lt.x + 2, lt.y, lt.w - 4, 16), "Select a point to edit", Color(0.6f));
	}

	if (exists && gd.entries.size() > 1) {
		if (UI::I::Button(Rect(Rect(lt.x + lt.w - CE_E_LBL_W * 2 - 4, lt.y, CE_E_LBL_W, 16)),
				Color(0.4f, 0.2f, 0.2f), "delete") == InputMouseStatus::HoverUp) {
			gd.entries.erase(gd.entries.begin() + (_egdil - &gd.entries[0]));
			_egdil = nullptr;
			gd.Apply();
		}

	}
	if (UI::I::Button(CE_E_VL2_RECT, Color(0.2f), "add") == InputMouseStatus::HoverUp) {
		gd.entries.push_back(Gradient::Entry(0.5f, Color(1.f)));
		_egdil = &gd.entries.back();
		changed = true;
	}
	CE_E_INC_Y();

	if (changed) {
		auto& it = std::find_if(gd.entries.begin(), gd.entries.end(), [&](const Gradient::Entry& e2) {
			return e2.position > _egdil->position;
		});
		if (it != gd.entries.end() && (&*it - _egdil) != 1) {
			const auto tmp = *_egdil;
			gd.entries.erase(gd.entries.begin() + (_egdil - &gd.entries[0]));
			it = gd.entries.insert(it, tmp);
			_egdil = &*it;
			if (_egdi) _egdi = _egdil;
		}
		gd.Apply();
	}

	/*
	Layout::Block("points", lt, [&]() {
		for (auto& p : gd.entries) {
			auto pn = UI_Ext::TextFieldF(Rect(lt.x + 3, lt.y, CE_E_LBL_W - 3, 16), p.position);
			if (pn != p.position) {
				p.position = pn;
				gd.Apply();
			}
			if (UI::I::Button(CE_E_VL_RECT, p.value) == InputMouseStatus::HoverUp) {
				const auto pgd = &gd;
				const auto pp = &p;
				EO_ColorPicker::Reg(CE_E_VL_RECT, p.value, std::function<void(const Color&)>([pgd, pp](const Color& vl) {
					pp->value = vl;
					pgd->Apply();
				}));
			}
			CE_E_INC_Y();
		}
	}, 0, CE_UI_BLOCK_CLOSED);
	*/
}

CE_END_ED_NAMESPACE