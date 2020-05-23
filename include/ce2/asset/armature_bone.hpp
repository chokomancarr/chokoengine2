#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

/* A bone in an armature
 * All coordinates are in armature-space
 */
class ArmatureBone { CE_CLASS_COMMON
public:
	ArmatureBone(const std::string& name);

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

    std::vector<ArmatureBone> children;

	struct TRS {
		Vec3 pos;
		Quat rot;
		Vec3 scl;

		Mat4x4 toMat() const;
	} getTRS() const;
};

/* A bone in an armature (generated)
 * Used as glue with the animation system
 * All matrices are in armature-space
 */
class ArmatureBoneG {
	CE_CLASS_COMMON
public:
	ArmatureBoneG(const ArmatureBone&, const std::string&, int, const Mat4x4&);

	const std::string sig;

	float length;

	int parentId;

	ArmatureBone::TRS restTRS;

	Mat4x4 restMat, restMatI;

	Mat4x4 currMat;
};

CE_END_NAMESPACE