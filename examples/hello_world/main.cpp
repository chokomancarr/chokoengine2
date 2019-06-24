#include "chokolait.hpp"

CHOKOLAIT_INIT_VARS

int main() {
	ChokoLait::Init(800, 600);

	while (ChokoLait::alive()) {
		ChokoLait::Update();
		ChokoLait::Paint();
	}
}