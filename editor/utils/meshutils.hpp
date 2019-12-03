#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class MeshSurfaceData;

class MeshUtils {
	static bool initd;

	static Shader padShad;
	static Shader blurShad;

	static void Init();

public:
	static MeshSurfaceData GenSurfaceData(const Mesh& m);

	static void PadTexture(MeshSurfaceData& data, const Texture& src, const RenderTarget& tar);

	static void SurfaceBlur(MeshSurfaceData& data, const Texture& src,
			const RenderTarget& tar, const RenderTarget& tmp, float size);
};

class MeshSurfaceData {
	static bool initd;

	static Shader uvInfoShad;
	static Shader uvInfoShad2;
	static Shader jmpInfoShad;

	static void InitShaders();

public:
	size_t vertCount;
	size_t indCount;

	TextureBuffer positions; //model space XYZ coords
	TextureBuffer uvcoords; //texture space UV coords
	TextureBuffer indices; //triangle indices
	TextureBuffer edgeData; //texture space (u1-u0, u2-u0), model space (p1-p0, 0), model space (p2-p0, 0)
	TextureBuffer iconData; //connected triangle indices
	TextureBuffer uvMats; //transform matrices

	struct infoTexSt {
		FrameBuffer uvInfoTex;
		FrameBuffer jmpInfoTex;
	};

	std::unordered_map<uint64_t, infoTexSt> texs;

	const infoTexSt& GetInfoTex(const Int2& res);
	const infoTexSt& GenInfoTex(const Int2& res);
};

CE_END_ED_NAMESPACE