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
	if (fns.empty()) {
		Debug::Warning("CallbackManager", "No handlers registered for callback " + CallbackSigStr.at(sig));
		return;
	}
	std::string argstr = " [";
	auto& aa = args.args();
	if (!aa.size()) {
		argstr = " [no args]";
	}
	else {
		for (auto& a : aa) {
			const std::string ats[] = { "Int", "Float", "String", "Object" };
			argstr += a.name + "(" + ats[(int)a.type] + ")=";
			switch (a.type) {
			case ECallbackArg::Type::Int:
				argstr += std::to_string(a.i);
				break;
			case ECallbackArg::Type::Float:
				argstr += std::to_string(a.f);
				break;
			case ECallbackArg::Type::String:
				argstr += "\"" + a.s + "\"";
				break;
			case ECallbackArg::Type::Object:
				argstr += (!a.o) ? "none" : ("\"" + a.o->name() + "\"");
				break;
			}
			argstr += (&a == &aa.back()) ? "]" : ", ";
		}
	}
	Debug::Message("CallbackManager", "Invoke " + CallbackSigStr.at(sig)
			+ argstr + " (" + std::to_string(fns.size()) + " handlers)");
	for (auto& fn : fns) {
		fn(args);
	}
}

CE_END_ED_NAMESPACE