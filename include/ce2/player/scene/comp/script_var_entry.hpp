#pragma once
#include "chokoplayer.hpp"

CE_BEGIN_PL_NAMESPACE

/* struct describing a script variable,
 * used for serialization
 */

struct ScriptVarEntry {
    std::string name;
    
    enum class Type {
        Int,
        Float,
        Vec2,
        Vec3,
        Vec4,
        Quat,
        Asset,
        Class
    } type;

    /* if type is class, the signature of the class
     * used for subsequent queries
     */
    std::string sub_class;
};

CE_END_PL_NAMESPACE
