#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EDropdownMenu {
public:
    EDropdownMenu(const std::string& t = "");

    std::string title;
    std::vector<EDropdownMenu> items;
	bool enabled;

    ECallbackCaller callback;

	void Set(const ECallbackArg&);
	void SetAll(const ECallbackArg&, bool recurse = true);
};

CE_END_ED_NAMESPACE