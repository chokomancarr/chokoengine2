#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

Bone::Bone(const std::string& name) : name(name), base(0), length(0, 1, 0), 
        front(0, 0, 1), connected(false), children({}) {}

_Bone::_Bone(const Bone& b, const std::string& n, int p, const Mat4x4& m)
	: sig(n + b.name), length(b.length.length()), parentId(p), restMat(m), restMatI(m.inverse()), currMat(Mat4x4::Identity()) {}

CE_END_NAMESPACE