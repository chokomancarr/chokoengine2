#include "chokoplayer.hpp"

CE_BEGIN_PL_NAMESPACE

std::string ChokoPlayer::dataPath;

inline void paint() {

}

void ChokoPlayer::Init() {
	auto font = Font::New(IO::path() + "res/font.ttf");
	UI::defaultFont(font);
	font->size(12);

	dataPath = IO::path() + "data/";
}

void ChokoPlayer::Main() {
	ChokoLait::InitOptionsSt opts;
	//opts.visible = false;
	ChokoLait::Init(1000, 600);

	Init();

	while (ChokoLait::alive()) {
		ChokoLait::Update([]() {
			
		});
		ChokoLait::Paint(0, paint);
	}
}

CE_END_PL_NAMESPACE
