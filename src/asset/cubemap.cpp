#include "chokoengine.hpp"
#include "texture_internal.hpp"
#include "backend/chokoengine_backend.hpp"
#include "glsl/minVert.h"
#include "glsl/presum_ggx_cube.h"

CE_BEGIN_NAMESPACE

#define SAMPLES 1024

bool _CubeMap::initd;

Shader _CubeMap::ggxBlurShad;
TextureBuffer _CubeMap::noiseTex;

void _CubeMap::Init() {
	(ggxBlurShad = Shader::New(glsl::minVert, glsl::presumGGXCube))
		->AddUniforms({ "cubemap", "rough", "screenSize", "noise", "samples", "face", "dirx", "diry", "level" });
	std::vector<Vec2> noise(65535);
    for (auto& n : noise) {
        n = Vec2(rand() * 1.f / RAND_MAX, rand() * 1.f / RAND_MAX);
    }
    const auto& noiseBuf = VertexBuffer_New(true, 2, 65535, noise.data());
    noiseTex = TextureBuffer::New(noiseBuf, GL_RG32F);
	initd = true;
}

_CubeMap::_CubeMap(uint res, GLenum type, const TextureOptions& opts, int div) : _Texture(nullptr), _layers(div) {
	glGenTextures(1, &_pointer);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _pointer);
	Color cs[] = {
		Color::red(), Color::yellow(), Color::green(), Color(0.5f), Color::blue(), Color::cyan()
	};
	for (int a = 0; a < 6; a++) {
		std::vector<Color> cols(res * res, cs[a]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + a, 0, type, (int)res, (int)res, 0, GL_RGBA, GL_FLOAT, cols.data());
	}
	SetTexParams<GL_TEXTURE_CUBE_MAP>(-1,
		GL_CLAMP_TO_EDGE,
		GL_CLAMP_TO_EDGE,
		(opts.linear) ? (
		(opts.mipmaps > 0) ?
			GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR
			) : GL_NEAREST,
			(opts.linear) ?
		GL_LINEAR : GL_NEAREST
		);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	if (opts.mipmaps > 0) {
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	_width = _height = res;
}

void _CubeMap::ComputeGlossMipmaps() {
	if (!initd)
		Init();

	const float vec[] = {
		1, 0, 0,	-1, 0, 0,
		0, 1, 0,	0, -1, 0,
		0, 0, 1,	0, 0, -1
	};
	const float dx[] = {
		0, 0, -1,	0, 0, 1,
		0, 0, -1,	0, 0, -1,
		1, 0, 0,	-1, 0, 0
	};
	const float dy[] = {
		0, 1, 0,	0, 1, 0,
		-1, 0, 0,	1, 0, 0,
		0, 1, 0,	0, 1, 0
	};
	int res = _width / 2;
	for (int a = 1; a < _layers; a++) {
		glViewport(0, 0, res, res);
		std::array<RenderTarget, 6> tars;
		for (int f = 0; f < 6; f++) {
			tars[f] = RenderTarget::New(res, res, true, false);
			ggxBlurShad->Bind();
			glUniform1i(ggxBlurShad->Loc(0), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, _pointer);
			glUniform1f(ggxBlurShad->Loc(1), a * 1.f / _layers);
			glUniform2f(ggxBlurShad->Loc(2), res, res);
			glUniform1i(ggxBlurShad->Loc(3), 1);
			glActiveTexture(GL_TEXTURE1);
			noiseTex->Bind();
			glUniform1i(ggxBlurShad->Loc(4), SAMPLES);
			glUniform3f(ggxBlurShad->Loc(5), vec[a * 3], vec[a * 3 + 1], vec[a * 3 + 2]);
			glUniform3f(ggxBlurShad->Loc(6), dx[a * 3], dx[a * 3 + 1], dx[a * 3 + 2]);
			glUniform3f(ggxBlurShad->Loc(7), dy[a * 3], dy[a * 3 + 1], dy[a * 3 + 2]);
			glUniform1f(ggxBlurShad->Loc(8), a - 1);
			tars[f]->BindTarget();
			Backend::Renderer::emptyVao()->Bind();
			glDrawArrays(GL_TRIANGLES, 0, 6);
			Backend::Renderer::emptyVao()->Unbind();
			tars[f]->UnbindTarget();
			ggxBlurShad->Unbind();
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, _pointer);
		for (int f = 0; f < 6; f++) {
			tars[f]->BindTarget(true);
			glCopyTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + f, a, 0, 0, 0, 0, res, res);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		}
		SetTexParams<GL_TEXTURE_CUBE_MAP>(-1, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		res /= 2;
	}
}

void _CubeMap::Bind() {
	glBindTexture(GL_TEXTURE_CUBE_MAP, _pointer);
}

void _CubeMap::Unbind() const {
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

CE_END_NAMESPACE
