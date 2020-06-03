#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

EDragDrop::Type EDragDrop::type;

EAssetList::TypeOfSt EDragDrop::assetType;

std::vector<std::string> EDragDrop::target = {};
std::vector<Object> EDragDrop::targetObj = {};

void EDragDrop::Set(Type t, const std::vector<std::string>& ss) {
	Clear();
    type = t;
    target = ss;
}

void EDragDrop::Set(EAssetList::TypeOfSt t, const std::vector<std::string>& ss) {
	Clear();
    type = Type::Asset;
    assetType = t;
    target = ss;
}

void EDragDrop::Set(const std::vector<SceneObject>& oo) {
	Clear();
	type = Type::SceneObject;
	for (auto& o : oo) {
		targetObj.push_back(o);
	}
}

bool EDragDrop::IsEmpty() {
    return target.empty() && targetObj.empty();
}

bool EDragDrop::IsSingle() {
    return target.size() == 1 || targetObj.size() == 1;
}

void EDragDrop::Clear() {
    target.clear();
	targetObj.clear();
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
