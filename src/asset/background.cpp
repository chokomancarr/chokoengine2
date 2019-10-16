#include "chokoengine.hpp"
#include "texture_internal.hpp"
#include "glsl/minVert.h"
#include "glsl/presum_ggx.h"

CE_BEGIN_NAMESPACE

bool _Background::initd = false;

Shader _Background::ggxBlurShad;
Material _Background::ggxBlurMat;
TextureBuffer _Background::noiseTex;

#define SAMPLES 1024

void  _Background::Init() {
    //auto noise = Random::Hammersley(SAMPLES);
    std::vector<Vec2> noise(65535);
    for (auto& n : noise) {
        n = Vec2(rand() * 1.f / RAND_MAX, rand() * 1.f / RAND_MAX);
    }
    auto noiseBuf = VertexBuffer_New(true, 2, 65535, noise.data());
    noiseTex = TextureBuffer::New(noiseBuf, GL_RG32F);

    ggxBlurShad = Shader::New(glsl::minVert, glsl::presumGGX);
    ggxBlurShad->AddUniform("mainTex", ShaderVariableType::Texture);
    ggxBlurShad->AddUniform("rough", ShaderVariableType::Float);
    ggxBlurShad->AddUniform("screenSize", ShaderVariableType::Vec2);
    ggxBlurShad->AddUniform("noise", ShaderVariableType::Texture);
    ggxBlurShad->AddUniform("samples", ShaderVariableType::Int);

    ggxBlurMat = Material::New();
    ggxBlurMat->shader(ggxBlurShad);
    ggxBlurMat->SetUniform("noise", static_cast<Texture>(noiseTex));
    ggxBlurMat->SetUniform("samples", (int)SAMPLES);

    initd = true;
}

_Background::_Background(const std::string& path, int div) : _Texture(nullptr), _layers(div), _brightness(1) {
    if (!initd)
        Init();
    
    std::string ss = path.substr(path.find_last_of('.') + 1, std::string::npos);

    if (ss != "hdr") {
        Debug::Error("Background", "Cannot open non-hdr file \"" + path + "\"!");
        return;
    }

    std::vector<byte> data;

    if (!Texture_I::FromHDR(path, _width, _height, _channels, data))
        return;
    _hdr = true;

    std::vector<RenderTarget> mips(div - 1, nullptr);

    Texture tmp0 = Texture::New(path, TextureOptions(TextureWrap::Repeat, TextureWrap::Mirror, 0, true));
    uint w = _width / 2, h = _height / 2;
    ggxBlurMat->SetUniform("mainTex", tmp0);
    for (int a = 1; a < div; a++) {
        RenderTarget target2 = RenderTarget::New(w, h, true, false);
        ggxBlurMat->SetUniform("rough", a * 1.f / div);
        ggxBlurMat->SetUniform("screenSize", Vec2(w, h));
        tmp0->Blit(target2, ggxBlurMat);
        mips[a - 1] = target2;
        tmp0 = static_cast<Texture>(target2); //this is wrong, but it reduces noise for now
        w /= 2;
        h /= 2;
    }

    glGenTextures(1, &_pointer);
	glBindTexture(GL_TEXTURE_2D, _pointer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, _width, _height, 0, GL_RGB, GL_FLOAT, data.data());

    for (int a = 1; a < div; a++) {
		mips[a - 1]->BindTarget(true);
        glCopyTexImage2D(GL_TEXTURE_2D, a, GL_RGB32F, 0, 0, mips[a - 1]->width(), mips[a - 1]->height(), 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    }
    
	SetTexParams<>(div - 1, GL_REPEAT, GL_MIRRORED_REPEAT,
		GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

CE_END_NAMESPACE