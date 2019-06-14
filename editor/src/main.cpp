#include "chokolait.hpp"

namespace CE = ChokoEngine;

CE::Texture tex;

void paint();

int main() {
	ChokoLait::Init("ChokoEditor", 800, 600);

	tex = CE::Texture::New(CE::IO::path() + "../../res/logo.png");

	while (ChokoLait::alive()) {
		ChokoLait::Update();
		ChokoLait::Paint(0, paint);
	}
}

void paint() {
	CE::UI::Texture(CE::Rect(10, 10, 128, 128), tex);
}