#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_S_Rig {
	static VertexArray _vao;
	static VertexBuffer _elo;
	static Shader _boneProg;

public:
	static void Init();

	static void Draw(const Component& rig);
};

CE_END_ED_NAMESPACE