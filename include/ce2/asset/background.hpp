#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _Background : public _Texture { CE_OBJECT_COMMON
    static bool initd;

    static Shader ggxBlurShad;
    static Material ggxBlurMat;
    static TextureBuffer noiseTex;

    int _layers;

	float _brightness;

    static void Init();
public:
    /* Constructs a background image from a hdr file
     * \p div specifies the number of layers of the precomputed gloss map
     */
    _Background(const std::string& path, int div);

    CE_GET_MEMBER(layers);

	CE_GET_SET_MEMBER(brightness);
};

CE_END_NAMESPACE