#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

EDragDrop::Type EDragDrop::type;

EAssetList::TypeOfSt EDragDrop::assetType;

std::vector<std::string> EDragDrop::target = {};

void EDragDrop::Set(Type t, const std::vector<std::string>& ss) {
    type = t;
    target = ss;
}

void EDragDrop::Set(EAssetList::TypeOfSt t, const std::vector<std::string>& ss) {
    type = Type::Asset;
    assetType = t;
    target = ss;
}

bool EDragDrop::IsEmpty() {
    return target.empty();
}

bool EDragDrop::IsSingle() {
    return target.size() == 1;
}

void EDragDrop::Clear() {
    target.clear();
}

void EDragDrop::PostLoop() {
    switch (Input::mouseStatus(InputMouseButton::Left)) {
    case InputMouseStatus::Down:
    case InputMouseStatus::Up:
        Clear();
        break;
    default:
        break;
    }
}

CE_END_ED_NAMESPACE
