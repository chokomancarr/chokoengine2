#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

EDropdownMenu::EDropdownMenu(const std::string& t)  
        : title(t), items({}), callback(nullptr) {}

CE_END_ED_NAMESPACE