#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EPreview {
	struct {
		Scene scene;
		MeshRenderer rend;
		Camera cam;
	} _mat;

public:
	static void Init();

	static Texture GetMaterial(const Material&);
};

CE_END_ED_NAMESPACE