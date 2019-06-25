#include "chokoengine.hpp"
#include "texture_internal.hpp"
#include "glsl/minVert.h"
#include "glsl/blurFrag.h"

CE_BEGIN_NAMESPACE

bool _Background::initd = false;

Shader _Background::ggxBlurShad;
Material _Background::ggxBlurMat;
TextureBuffer _Background::noiseTex;

void  _Background::Init() {
    ggxBlurShad = Shader::New(glsl::minVert, glsl::blurFrag);
    ggxBlurShad->AddUniform("mainTex", ShaderVariableType::Texture);
    ggxBlurShad->AddUniform("mul", ShaderVariableType::Float);
    ggxBlurShad->AddUniform("screenSize", ShaderVariableType::Vec2);
    ggxBlurShad->AddUniform("isY", ShaderVariableType::Int);

    ggxBlurMat = Material::New();
    ggxBlurMat->shader(ggxBlurShad);
    ggxBlurMat->SetUniform("mul", 1.f);

    initd = true;
}

_Background::_Background(const std::string& path, int div) : _Texture(nullptr), _layers(div) {
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
    RenderTarget tmprt;
    uint w = _width / 2, h = _height / 2;
    for (int a = 1; a < div; a++) {
        tmprt = RenderTarget::New(w, h, true);
        RenderTarget target2 = RenderTarget::New(w, h, true);
        ggxBlurMat->SetUniform("screenSize", Vec2(w, h));
        ggxBlurMat->SetUniform("isY", (int)0);
        _RenderTarget::Blit(tmp0, tmprt, ggxBlurMat);
        ggxBlurMat->SetUniform("isY", (int)1);
        _RenderTarget::Blit(static_cast<Texture>(tmprt), target2, ggxBlurMat);
        tmp0 = static_cast<Texture>(target2);
        mips[a - 1] = target2;
        w /= 2;
        h /= 2;
    }

    glGenTextures(1, &_pointer);
	glBindTexture(GL_TEXTURE_2D, _pointer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, _width, _height, 0, GL_RGB, GL_FLOAT, data.data());

    for (int a = 1; a < div; a++) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, mips[a - 1]->fbo());
        glCopyTexImage2D(GL_TEXTURE_2D, a, GL_RGB32F, 0, 0, mips[a - 1]->width(), mips[a - 1]->height(), 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    }
    
	SetTexParams<>(div - 1, GL_REPEAT, GL_MIRRORED_REPEAT,
		GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

CE_END_NAMESPACE