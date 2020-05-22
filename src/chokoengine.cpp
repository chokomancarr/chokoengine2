#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

bool ChokoEngine::Init() {

#define TRYINIT(nm)\
	if (!nm::Init()) return false;

	TRYINIT(Debug);
	TRYINIT(IO);
	TRYINIT(Input);
	TRYINIT(MVP);
	TRYINIT(UI);

	TRYINIT(_Font);

	TRYINIT(_Camera);

	Debug::Message("Engine", "ChokoEngine 2 build "
#include "../githash.h"
	);

	return true;
}

void ChokoEngine::Cleanup() {
	Debug::Message("Engine", "engine says byebye!");
}

CE_END_NAMESPACE
