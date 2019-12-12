#include "chokoplayer.hpp"

CE_BEGIN_PL_NAMESPACE

std::string ChokoPlayer::projectPath;

inline void paint() {

}

void ChokoPlayer::Init() {
	projectPath = IO::path() + "data/";

	auto font = Font::New(projectPath + "resources/font.ttf");
	UI::defaultFont(font);
	font->size(12);
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
