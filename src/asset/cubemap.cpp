#include "chokoengine.hpp"
#include "texture_internal.hpp"

CE_BEGIN_NAMESPACE

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
	std::cout << glGetError() << std::endl;
}

void _CubeMap::Bind() const {
	glBindTexture(GL_TEXTURE_CUBE_MAP, _pointer);
}

void _CubeMap::Unbind() const {
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

CE_END_NAMESPACE