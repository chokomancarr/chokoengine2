#include "chokolait.hpp"

CHOKOLAIT_INIT_VARS(800, 600);

int main() {

	while (ChokoLait::alive()) {
		ChokoLait::Update();
		ChokoLait::Paint();
	}
}