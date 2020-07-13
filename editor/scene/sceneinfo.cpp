#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

ESceneInfo::SelType ESceneInfo::selectedType;

SceneObject ESceneInfo::selectedObject;
std::vector<SceneObject> ESceneInfo::subSelectedObjects;

Asset ESceneInfo::selectedAsset;

std::string ESceneInfo::selectedString;

void ESceneInfo::Clear() {
    selectedType = SelType::None;
    selectedObject = nullptr;
    selectedAsset = nullptr;
    selectedString.clear();
	EW_Inspector::customDrawer = nullptr;
}

void ESceneInfo::Select(const std::string& s) {
    Clear();
    selectedType = SelType::String;
    selectedString = s;
}

CE_END_ED_NAMESPACE