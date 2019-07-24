#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

bool ChokoEngine::Init() {
	Debug::Message("ChokoEngine", "engine says hello!");

#define TRYINIT(nm)\
	if (!nm::Init()) return false;

	TRYINIT(Debug);
	TRYINIT(IO);
	TRYINIT(Input);
	TRYINIT(MVP);
	TRYINIT(UI);

	TRYINIT(_Font);

	TRYINIT(_Camera);

	return true;
}

void ChokoEngine::Cleanup() {
	Debug::Message("ChokoEngine", "engine says byebye!");
}

CE_END_NAMESPACE
