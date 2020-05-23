#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

ArmatureBone::TRS ArmatureBone::getTRS() const {
	TRS res;
	res.pos = base;
	res.rot = Quat::LookAt(length, front);
	if (std::isnan(res.rot.x)
		|| std::isnan(res.rot.y)
		|| std::isnan(res.rot.z)
		|| std::isnan(res.rot.w)) {
		res.rot = Quat::identity();
	}
	res.scl = Vec3(1);
	return res;
}

Mat4x4 ArmatureBone::TRS::toMat() const {
	return Mat4x4::FromTRS(pos, rot, scl);
}


ArmatureBone::ArmatureBone(const std::string& name) : name(name), base(0), length(0, 1, 0), 
        front(0, 0, 1), connected(false), children({}) {}


ArmatureBoneG::ArmatureBoneG(const ArmatureBone& b, const std::string& n, int p, const Mat4x4& m)
	: sig(n + b.name), length(b.length.length()), parentId(p), restTRS(b.getTRS()),
	restMat(m), restMatI(m.inverse()), currMat(Mat4x4::Identity()) {}

CE_END_NAMESPACE