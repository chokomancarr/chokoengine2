#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EDebug {
public:
    typedef std::function<std::string(const SceneObject&)> obj2strFn;

    static std::string ObjTree(const std::vector<SceneObject>&, obj2strFn);
};

CE_END_ED_NAMESPACE