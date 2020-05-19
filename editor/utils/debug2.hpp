#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

#define EDEBUG_LVL_VERBOSE 0
#define EDEBUG_LVL_MESSAGE 1
#define EDEBUG_LVL_WARNING 2
#define EDEBUG_LVL_ERROR 3

class EDebug {
public:
    typedef std::function<std::string(const SceneObject&)> obj2strFn;

	static int printLevel;

	static void Log(const std::string& caller, const std::string& msg, const int level = EDEBUG_LVL_MESSAGE);

    static std::string ObjTree(const std::vector<SceneObject>&, obj2strFn);
};

CE_END_ED_NAMESPACE