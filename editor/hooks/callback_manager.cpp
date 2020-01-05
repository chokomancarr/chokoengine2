#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

namespace {
	std::array<std::vector<ECallbackManager::CbFn>, (int)CallbackSig::_COUNT> _handlers;
}

void ECallbackManager::Init() {
	JsonObject json = JsonParser::Parse(IO::ReadFile(IO::path() + "config/input/default.json"));
	for (auto& g : json.group) {

	}
}

void ECallbackManager::Update() {

}

void ECallbackManager::Register(CallbackSig sig, CbFn fn) {
	_handlers[(int)sig].push_back(fn);
}

void ECallbackManager::Register(CallbackSig sig, std::function<void()> fn) {
	_handlers[(int)sig].push_back([fn](const ECallbackArgs&) {
		fn();
	});
}

void ECallbackManager::Invoke(CallbackSig sig, const ECallbackArgs& args) {
	const auto& fns = _handlers[(int)sig];
	if (!fns.size()) {
		Debug::Warning("CallbackManager", "No handlers registered for callback " + CallbackSigStr.at(sig));
	}
	for (auto& fn : fns) {
		fn(args);
	}
}

CE_END_ED_NAMESPACE