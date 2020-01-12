#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

EDropdownMenu::EDropdownMenu(const std::string& t)  
        : title(t), items({}), enabled(true), callback(CallbackSig::Undefined) {}

void EDropdownMenu::Set(const ECallbackArg& arg) {
	for (auto& i : callback.args._args) {
		if (i.name == arg.name) {
			i = arg;
			return;
		}
	}
}

void EDropdownMenu::SetAll(const ECallbackArg& arg, bool r) {
	for (auto& i : items) {
		i.Set(arg);
		if (r) {
			i.SetAll(arg);
		}
	}
}

CE_END_ED_NAMESPACE