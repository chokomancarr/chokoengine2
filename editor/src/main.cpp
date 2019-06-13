#include "chokolait.hpp"

namespace CE = ChokoEngine;

CHOKOLAIT_INIT_VARS(800, 600);

CE::Texture tex(CE::IO::path() + "../../res/logo.png");

void paint();

int main() {

	while (ChokoLait::alive()) {
		ChokoLait::Update();
		ChokoLait::Paint(0, paint);
	}
}

void paint() {
	CE::UI::Texture(CE::Rect(10, 10, 128, 128), tex);
}