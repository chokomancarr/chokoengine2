#pragma once
#include "backend/chokoengine_backend.hpp"

CE_BEGIN_BK_NAMESPACE

class GI {
public:
	class Voxelizer {
		static Shader voxShad;
		static Shader voxDebugShad;

		static RenderTarget3 res;

		static Mat4x4 lastVP;

	public:
		struct regionSt {
			float x0 = -10, x1 = 10;
			float y0 = -10, y1 = 10;
			float z0 = -10, z1 = 10;
		};

	private:
		static regionSt regionOld;

	public:
		static regionSt region;

		static int resolution();
		static void resolution(int);

		static bool InitShaders();

		static void Bake();

		static void DrawVoxelAO(const Mat4x4& p);

		static void DrawDebug(const Mat4x4& vp);
	};

	static bool Init();
};

CE_END_BK_NAMESPACE