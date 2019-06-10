#include "chokoengine.hpp"

BEGIN_CE_NAMESPACE

bool Engine::Init() {
	Debug::Message("Engine", "engine says hello!");
	return true;
}

void Engine::Cleanup() {
	Debug::Message("Engine", "engine says byebye!");
}

END_CE_NAMESPACE
