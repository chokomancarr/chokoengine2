#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

EDropdownMenu::EDropdownMenu(const std::string& t)  
        : title(t), items({}), enabled(true), callback(nullptr) {}

CE_END_ED_NAMESPACE