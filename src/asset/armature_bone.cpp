#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

Bone::Bone(const std::string& name) : name(name), base(0), length(0, 1, 0), 
        front(0, 0, 1), connected(false), children({}) {}

_Bone::_Bone(const Bone& b, const std::string& p, const Mat4x4& m)
	: sig(p + b.name), length(b.length.length()), restMat(m) {}

CE_END_NAMESPACE