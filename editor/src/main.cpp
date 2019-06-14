#include "chokolait.hpp"

namespace CE = ChokoEngine;

CE::Texture tex;

void paint();

int main() {
	ChokoLait::Init("ChokoEditor", 800, 600);

	tex = CE::Texture::New(CE::IO::path() + "../../res/logo.png");

	CE::Scene s = CE::Scene::New();
	auto o = s->AddNewObject();
	o->name("object1");
	auto o2 = s->AddNewObject();
	o2->name("object2");
	s->AddNewObject(o)->name("subobject1");
	s->AddNewObject(o)->name("subobject2");
	s->AddNewObject(o2)->name("subobject3");
	o2->AddComponent<CE::Camera>();

	std::cout << s->Tree() << std::endl;

	while (ChokoLait::alive()) {
		ChokoLait::Update();
		ChokoLait::Paint(0, paint);
	}
}

void paint() {
	CE::UI::Texture(CE::Rect(10, 10, 128, 128), tex);
}