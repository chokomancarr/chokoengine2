#include "chokolait.hpp"
#include "ce2/parsers/mesh.hpp"

using namespace ChokoEngine;

int main() {
	if (!ChokoLait::Init("Rotating cube", 800, 600)) {
		return -1;
	}

	MeshLoader::LoadObj("../assets/cube.obj");

	while (ChokoLait::alive()) {
		ChokoLait::Update();
		ChokoLait::Paint();
	}
}