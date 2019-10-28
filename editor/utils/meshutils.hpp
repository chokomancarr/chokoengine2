#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class MeshSurfaceData;

class MeshUtils {
public:
	static MeshSurfaceData GenSurfaceData(const Mesh& m);
};

class MeshSurfaceData {
public:
	TextureBuffer positions;
	TextureBuffer uvcoords;
	TextureBuffer indices;
	TextureBuffer edgeData;
	TextureBuffer iconData;
	TextureBuffer conData;

	RenderTarget edgeInfoTex;

	void GenInfoTex(const Int2& res);
};

CE_END_ED_NAMESPACE