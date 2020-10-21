#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _Background : public _Asset { CE_OBJECT_COMMON CE_OBJECT_ALLOW_ASYNC
    static Shader ggxBlurShad;
    static Material ggxBlurMat;
    static TextureBuffer noiseTex;

    static bool initd;

    GLuint _pointer;

    uint _width, _height;
    byte _channels;

    int _layers;

	float _brightness;

    std::vector<byte> _pixels;

    static void Init();
public:
    /* Constructs a background image from a hdr file
     * \p div specifies the number of layers of the precomputed gloss map
     */
	_Background(DataStream strm, int div, bool async = false);

    ~_Background();

    CE_GET_MEMBER(layers);

	CE_GET_SET_MEMBER(brightness);
};

CE_END_NAMESPACE