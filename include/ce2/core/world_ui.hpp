#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

/* World-space UI elements
 * This seems strange to be called "UI",
 * but this will do until I find out a better place for it.
 */
class UI::W { CE_CLASS_COMMON

	static CE_EXPORT Mat4x4 _matrix;

public:
	/* The matrix used to transform subsequent drawings
	 */
	CE_GET_SET_ST_MEMBER(matrix);

    /* Draws a straight line connecting two points
     */
	static void Line(const Vec3& p1, const Vec3& p2, const Color& col);
};

CE_END_NAMESPACE