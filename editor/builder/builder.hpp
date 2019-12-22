#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

/* Asynchronous project building
 */
class EProjectBuilder {
    static std::mutex _mtx;
    static bool _busy;
public:
    static std::string dbgProgPath;
    static std::string relProgPath;

    static bool busy();

	static void Init();
    
    /* Build the project to run in editor
     */
    static bool BuildDebug();

    /* Build the project for standalone
     */
    static bool BuildRelease();
};

CE_END_ED_NAMESPACE