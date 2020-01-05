#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

namespace {
	std::array<std::vector<ECallbackFn>, (int)CallbackSig::_COUNT> _handlers;
}

void ECallbackManager::Init() {
	JsonObject json = JsonParser::Parse(IO::ReadFile(IO::path() + "config/input/default.json"));
	for (auto& g : json.group) {

	}
}

void ECallbackManager::Update() {

}

void ECallbackManager::Register(CallbackSig sig, ECallbackFn fn) {
	_handlers[(int)sig].push_back(fn);
}

void ECallbackManager::Invoke(CallbackSig sig, const ECallbackArgs& args) {
	if (sig == CallbackSig::Undefined) return;
	const auto& fns = _handlers[(int)sig];
	if (!fns.size()) {
		Debug::Warning("CallbackManager", "No handlers registered for callback " + CallbackSigStr.at(sig));
	}
	for (auto& fn : fns) {
		fn(args);
	}
}

CE_END_ED_NAMESPACE