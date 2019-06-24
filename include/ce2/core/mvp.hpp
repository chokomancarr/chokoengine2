#pragma once
#include "chokoengine.hpp"
#include <stack>

CE_BEGIN_NAMESPACE

class MVP {CE_CLASS_COMMON
	class stack : public std::stack<Mat4x4> {
	public:
		using std::stack<Mat4x4>::c;
	};

	static CE_EXPORT stack MV, P;
	static CE_EXPORT Mat4x4 _mv, _p;
	static CE_EXPORT bool changedMv, changedP;
	static CE_EXPORT const Mat4x4 identity;
	static CE_EXPORT bool isProj;

	static bool Init();
public:
	/* Changes which stack subsequent operations affects
	 * false: ModelView, true: Projection
	 */
	static void Switch(bool isProj);

	/* Adds a frame to the top of the current stack
	 * Matrices are applied top to bottom
	 */
	static void Push();

	/* Removes the top frame from the current stack
	 */
	static void Pop();

	/* Removes all frames from the current stack
	 */
	static void Clear();

	/* Removes all frames from all stacks
	 */
	static void ClearAll();

	/* Multiplies the top frame with \p m
	 * where M' = m * M
	 */
	static void Mul(const Mat4x4& m);

	/* Returns the ModelView matrix
	 */
	static Mat4x4 modelview();
	
	/* Returns the Projection matrix
	 */
	static Mat4x4 projection();
};

CE_END_NAMESPACE