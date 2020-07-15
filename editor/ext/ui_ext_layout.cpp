#include "chokoeditor.hpp"
#include "ext/ui_ext.hpp"
#include "ui/colors.hpp"
#include "res/icons.hpp"

CE_BEGIN_ED_NAMESPACE

void UI_Ext::Layout::InfoSt::BlockGrp::Clear() {
	blks.clear();
	n = 0;
}

UI_Ext::Layout::InfoSt::Block& UI_Ext::Layout::InfoSt::BlockGrp::Get(int i) {
	if (i >= n) {
		blks.resize(i + 1);
		n = i + 1;
	}
	return blks[i];
}

void UI_Ext::Layout::BeginLayout(const Rect& r, InfoSt& st) {
	st.x = r.x();
	st.y = r.y();
	st.w = r.w();
	st.base.i = 0;
	st.current = &st.base;
}

CE_UI_BUTTON_MASK UI_Ext::Layout::Block(const std::string& title, InfoSt& st, std::function<void()> f, CE_UI_BUTTON_MASK btns) {
	const auto pblk = st.current;
	auto& blk = pblk->children.Get(pblk->i++);
	blk.i = 0;
	st.x += 2;
	st.y += 1;
	st.w -= 4;
	
	UI::Rect(Rect(st.x, st.y, st.w, 17), Color(0.1f, 0.7f));
	if (UI::I::ButtonTr(Rect(st.x + 1, st.y, 16, 16)) == InputMouseStatus::HoverUp) {
		blk.expanded = !blk.expanded;
	}
	UI::Texture(Rect(st.x + 1, st.y, 16, 16), EIcons::icons[blk.expanded ? "minus" : "plus"], Color(0.8f));
	UI::Label(Rect(st.x + 18, st.y, st.w, 17), title, Color::white());

	CE_UI_BUTTON_MASK retmask = 0;
	int xm = st.x + st.w - 17;
	if (!!(btns & CE_UI_BUTTON_CLOSE)) {
		static const auto xbtn = EIcons::icons["close"];
		if (UI::I::Button(Rect(xm, st.y, 16, 16), Color(), xbtn, Color::red()) == InputMouseStatus::HoverUp) {
			retmask = CE_UI_BUTTON_CLOSE;
		}
	}

	st.y += 17;
	UI::Rect(Rect(st.x, st.y, st.w, blk.h), Color(0.1f, 0.3f));
	blk.y0 = st.y;
	st.current = &blk;

	if (blk.expanded) {
		f();
	}
	blk.h = st.y - blk.y0;
	st.x -= 2;
	st.y += 1;
	st.w += 4;
	st.current = pblk;

	return retmask;
}

float UI_Ext::Layout::EndLayout(InfoSt& st) {
	return st.y;
}

CE_END_ED_NAMESPACE