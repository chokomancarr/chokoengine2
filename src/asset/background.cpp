#include "chokoengine.hpp"
#include "texture_internal.hpp"
#include "glsl/minVert.h"
#include "glsl/presum_ggx.h"

CE_BEGIN_NAMESPACE

bool _Background::initd = false;

Shader _Background::ggxBlurShad;
Material _Background::ggxBlurMat;
TextureBuffer _Background::noiseTex;

#define SAMPLES 2000
#define SAMPLESN 1

void _Background::Init() {
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
    ggxBlurShad->AddUniform("samples", ShaderVariableType::Int);
	ggxBlurShad->AddUniform("level", ShaderVariableType::Int);

    ggxBlurMat = Material::New();
    ggxBlurMat->shader(ggxBlurShad);
    ggxBlurMat->SetUniform("samples", (int)SAMPLES);

    initd = true;
}

void _Background::LoadAsync() {
    std::vector<RenderTarget> mips(_layers - 1, nullptr);

    auto tmp0 = Texture::New(_width, _height, GL_RGB32F, TextureOptions(TextureWrap::Repeat, TextureWrap::Mirror, 0, true), _pixels.data(), GL_RGB, GL_FLOAT);
    uint w = _width / 2, h = _height / 2;
    ggxBlurMat->SetUniform("mainTex", tmp0);
    for (int a = 1; a < _layers; a++) {
        RenderTarget target2 = RenderTarget::New(w, h, true, false);
        ggxBlurMat->SetUniform("rough", a * 1.f / _layers);
        ggxBlurMat->SetUniform("screenSize", Vec2(w, h));
        tmp0->Blit(target2, ggxBlurMat);
        mips[a - 1] = target2;
        //tmp0 = static_cast<Texture>(target2); //this is wrong, but it reduces noise for now
        w /= 2;
        h /= 2;
		glFlush();
		glFinish();
		glfwPollEvents();
	}

    glGenTextures(1, &_pointer);
	glBindTexture(GL_TEXTURE_2D, _pointer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, _width, _height, 0, GL_RGB, GL_FLOAT, _pixels.data());

    for (int a = 1; a < _layers; a++) {
		mips[a - 1]->BindTarget(true);
        glCopyTexImage2D(GL_TEXTURE_2D, a, GL_RGB32F, 0, 0, mips[a - 1]->width(), mips[a - 1]->height(), 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    }
    
	SetTexParams<>(_layers - 1, GL_REPEAT, GL_MIRRORED_REPEAT,
		GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	std::vector<byte> empty(0);
	std::swap(_pixels, empty);

	_loading = false;
}

_Background::_Background(const std::string& path, int div, bool async)
	: _Background(std::ifstream(path, std::ios::binary), 0, div, async) {}

_Background::_Background(std::istream& strm, size_t sz, int div, bool async) : _Asset(AssetType::Background), _pointer(0), _width(0), _height(0), _layers(div), _brightness(1) {
    if (!initd)
        Init();

    //_asyncThread = std::thread([&](const std::string& path) {
	//	CE_OBJECT_SET_ASYNC_LOADING;

    if (!Texture_I::FromHDR(strm, sz, _width, _height, _channels, _pixels))
        return;

	//	CE_OBJECT_SET_ASYNC_READY;
	//}, path);

	//CE_OBJECT_INIT_ASYNC;

	uint szn = std::min(_width, _height) / 16;
	_layers = 1;
	while (szn > 0) {
		szn = szn >> 1;
		_layers++;
	}

	std::vector<RenderTarget> mips(_layers, nullptr);

    auto tmp0 = Texture::New(_width, _height, GL_RGB32F, TextureOptions(TextureWrap::Repeat, TextureWrap::Mirror, 0, true), _pixels.data(), GL_RGB, GL_FLOAT);
//        Texture::New(path, TextureOptions(TextureWrap::Repeat, TextureWrap::Mirror, _layers, true));
    uint w = _width / 2, h = _height / 2;
    ggxBlurMat->SetUniform("mainTex", tmp0);
    for (int a = 1; a <= _layers; a++) {
        RenderTarget target2 = RenderTarget::New(w, h, true, false);
		//RenderTarget target2t = RenderTarget::New(w, h, true, false);
        ggxBlurMat->SetUniform("rough", a * 1.0f / _layers);
        ggxBlurMat->SetUniform("screenSize", Vec2(w, h));
		ggxBlurMat->SetUniform("level", a);
		tmp0->Blit(target2, ggxBlurMat);
		glfwPollEvents();
		/*
		for (int b = 1; b < SAMPLESN; b++) {
			ggxBlurMat->SetUniform("resTex", target2);
			ggxBlurMat->SetUniform("resWeight", b / (b + 1.f));
			tmp0->Blit(target2t, ggxBlurMat);
			std::swap(target2, target2t);
			glfwPollEvents();
		}
		*/
        mips[a - 1] = target2;
        w /= 2;
        h /= 2;
    }

    glGenTextures(1, &_pointer);
	glBindTexture(GL_TEXTURE_2D, _pointer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, _width, _height, 0, GL_RGB, GL_FLOAT, _pixels.data());

    for (int a = 1; a < _layers; a++) {
		mips[a - 1]->BindTarget(true);
        glCopyTexImage2D(GL_TEXTURE_2D, a, GL_RGB32F, 0, 0, mips[a - 1]->width(), mips[a - 1]->height(), 0);
    }
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    
	SetTexParams<>(_layers - 1, GL_REPEAT, GL_MIRRORED_REPEAT,
		GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	//std::vector<byte> empty(0);
	//std::swap(_pixels, empty);
}

_Background::~_Background() {
    CE_OBJECT_FINALIZE_ASYNC;
	glDeleteTextures(1, &_pointer);
}

bool _Background::loaded() {
	CE_OBJECT_CHECK_ASYNC;
	return !!_pointer;
}

CE_END_NAMESPACE