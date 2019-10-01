#include "chokoeditor.hpp"
#include "ext/ui_ext.hpp"

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

void UI_Ext::Layout::BeginLayout(Rect& r, InfoSt& st) {
	st.r = &r;
	st.x = r.x();
	st.y = r.y();
	st.w = r.w();

	std::stack<InfoSt::Block*>().swap(st.current);
	st.base.i = 0;
	st.current.push(&st.base);
}

void UI_Ext::Layout::Push(const std::string& title, InfoSt& st) {
	auto& pblk = st.current.top();
	auto& blk = pblk->children.Get(pblk->i++);
	blk.i = 0;
	st.x += 2;
	st.w -= 4;
	UI::Rect(Rect(st.x, st.y, st.w, 17), Color(0.1f, 0.7f));
	UI::Label(Rect(st.x + 18, st.y, st.w, 17), title, Color::white());
	st.y += 18;
	UI::Rect(Rect(st.x, st.y, st.w, blk.h), Color(0.1f, 0.3f));
	blk.y0 = st.y;
	st.current.push(&blk);
}

void UI_Ext::Layout::Pop(InfoSt& st) {
	auto& blk = st.current.top();
	blk->h = st.y - blk->y0 + 1;
	st.y += 2;
	st.x -= 2;
	st.w += 4;
	st.current.pop();
}

void UI_Ext::Layout::EndLayout(InfoSt& st) {
	st.r->y(st.y);
}

CE_END_ED_NAMESPACE