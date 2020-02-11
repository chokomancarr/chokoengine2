#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

template <typename T>
typename std::enable_if<std::is_base_of<_Object, T>::value, void>::type
ESceneInfo::Select(const Ref<T>& o) {
    Clear();
    if (std::is_same<T, _SceneObject>::value) {
        selectedType = SelType::SceneObject;
        selectedObject = (SceneObject)o;
    }
    else if (std::is_base_of<_Asset, T>::value) {
        selectedType = SelType::Asset;
        selectedAsset = (Asset)o;
    }
    else {
        selectedType = SelType::String;
        selectedString = o->name();
    }
}

CE_END_ED_NAMESPACE