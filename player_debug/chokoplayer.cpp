#include "chokoplayer.hpp"

CE_BEGIN_PL_NAMESPACE

inline void paint() {

}

void ChokoPlayer::Init() {
	
}

void ChokoPlayer::Main() {
	ChokoLait::InitOptionsSt opts;
	ChokoLait::Init(1000, 600, opts);

	auto font = Font::New(IO::path() + "res/font.ttf");
	UI::defaultFont(font);
	font->size(12);

	while (ChokoLait::alive()) {
		ChokoLait::Update([]() {
			
		});
		ChokoLait::Paint(0, paint);
	}
}

CE_END_PL_NAMESPACE
