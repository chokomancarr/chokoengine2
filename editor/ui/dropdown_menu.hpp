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
};

CE_END_ED_NAMESPACE