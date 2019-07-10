#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Bone { CE_CLASS_COMMON
public:
    Bone(const std::string& name);

    std::string name;

    /* The base position
     */
    Vec3 base;
    /* tip - base
     */
    Vec3 length;
    /* The z-axis direction
     */
    Vec3 front;

    /* Is the base connected to the tail of the parent
     */
    bool connected;

    std::vector<Bone> children;
};

class _Bone { CE_CLASS_COMMON
public:
    _Bone(const Bone&, const std::string&, const Mat4x4&);

    const std::string sig;

    float length;

	Mat4x4 restMat;
};

CE_END_NAMESPACE