#include "chokoengine.hpp"

BEGIN_CE_NAMESPACE

bool Engine::Init() {
	Debug::Message("Engine", "engine says hello!");

#define TRYINIT(nm)\
	if (!nm::Init()) return false;

	TRYINIT(Debug);
	TRYINIT(IO);
	TRYINIT(MVP);
	TRYINIT(UI);
	
	TRYINIT(_Camera);

	return true;
}

void Engine::Cleanup() {
	Debug::Message("Engine", "engine says byebye!");
}

END_CE_NAMESPACE
