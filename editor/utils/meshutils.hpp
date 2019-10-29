#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class MeshSurfaceData;

class MeshUtils {
public:
	static MeshSurfaceData GenSurfaceData(const Mesh& m);

	static void SurfaceBlur(const MeshSurfaceData& data, const RenderTarget& tar, float size);
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

	FrameBuffer uvInfoTex;
	FrameBuffer jmpInfoTex;

	void GenInfoTex(const Int2& res);
};

CE_END_ED_NAMESPACE