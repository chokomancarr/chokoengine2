#pragma once
#include "chokoengine.hpp"
#include "texture/texture_options.hpp"
#include "enums/cubemap_face.hpp"

CE_BEGIN_NAMESPACE

class _CubeMap : public _Texture { CE_OBJECT_COMMON
	static bool initd;

	static Shader ggxBlurShad;
	static TextureBuffer noiseTex;

	int _layers;

	static void Init();
public:
	_CubeMap(uint res, GLenum type, const TextureOptions& opts, int div);
	/* Constructs a cubemap from six textures
	 * \p div specifies the number of layers of the precomputed gloss map
	 */
	_CubeMap(
		const std::string& path_px, const std::string& path_nx,
		const std::string& path_py, const std::string& path_ny,
		const std::string& path_pz, const std::string& path_nz, int div);

	CE_GET_MEMBER(layers);

	void ComputeGlossMipmaps();

	void Bind() override;
	void Unbind() const override;

	friend class _FrameBufferCube;
};

CE_END_NAMESPACE