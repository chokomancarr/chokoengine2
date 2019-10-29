#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class MeshSurfaceData;

class MeshUtils {
	static bool initd;
	static Shader blurShad;

	static void Init();

public:
	static MeshSurfaceData GenSurfaceData(const Mesh& m);

	static void SurfaceBlur(MeshSurfaceData& data, const Texture& src, const RenderTarget& tar, float size);
};

class MeshSurfaceData {
	static bool initd;

	static Shader uvInfoShad;
	static Shader jmpInfoShad;

	static void InitShaders();

public:
	size_t vertCount;
	size_t indCount;

	TextureBuffer positions;
	TextureBuffer uvcoords;
	TextureBuffer indices;
	TextureBuffer edgeData;
	TextureBuffer iconData;
	TextureBuffer conData;

	struct infoTexSt {
		FrameBuffer uvInfoTex;
		FrameBuffer jmpInfoTex;
	};

	std::unordered_map<uint64_t, infoTexSt> texs;

	infoTexSt GetInfoTex(const Int2& res);
	infoTexSt GenInfoTex(const Int2& res);
};

CE_END_ED_NAMESPACE