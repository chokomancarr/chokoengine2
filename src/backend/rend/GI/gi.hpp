#pragma once
#include "backend/chokoengine_backend.hpp"

CE_BEGIN_BK_NAMESPACE

class GI {
public:
	class Voxelizer {
		static Shader voxShad;
		static Shader voxDebugShad;

		static RenderTarget3 res;

		static Mat4x4 lastV, lastVP;

	public:
		static int resolution();
		static void resolution(int);

		static bool InitShaders();

		static void Bake(const Mat4x4& v, float sz);

		static void DrawDebug(const Mat4x4& p);
	};

	static bool Init();
};

CE_END_BK_NAMESPACE