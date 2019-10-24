#include "chokoengine.hpp"
#include "texture_internal.hpp"
#include "backend/chokoengine_backend.hpp"

CE_BEGIN_NAMESPACE

_Texture::_Texture(std::nullptr_t)
		: _pointer(0), _width(0), _height(0), _hdr(false) {}

_Texture::_Texture(uint w, uint h, GLuint ptr)
		: _pointer(ptr), _width(w), _height(h), _hdr(false) {}

_Texture::_Texture(uint w, uint h, bool hdr)
		: _width(w), _height(h), _channels(4), _hdr(hdr) {
	glGenTextures(1, &_pointer);
	glBindTexture(GL_TEXTURE_2D, _pointer);
	std::vector<byte> data(w * h * 4);
	glTexImage2D(GL_TEXTURE_2D, 0, hdr? GL_RGBA32F : GL_RGBA, (int)w, (int)h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
	SetTexParams<>(0, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

_Texture::_Texture(uint w, uint h, GLenum type, const TextureOptions& opts, const void* pixels, const GLenum pixelFmt, const GLenum pixelType) {
	glGenTextures(1, &_pointer);
	glBindTexture(GL_TEXTURE_2D, _pointer);
	glTexImage2D(GL_TEXTURE_2D, 0, type, (int)w, (int)h, 0, pixelFmt, pixelType, pixels);
	const GLenum wraps[] = { GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT };
	SetTexParams<>(0,
		wraps[(int)opts.xwrap],
		wraps[(int)opts.ywrap],
		(opts.linear) ? (
		(opts.mipmaps > 0) ?
			GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR
			) : GL_NEAREST,
			(opts.linear) ?
		GL_LINEAR : GL_NEAREST
		);
	if (opts.mipmaps > 0) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

_Texture::_Texture(const std::string& path, const TextureOptions& opts, bool async)
			: _pointer(0), _width(0), _height(0), _hdr(false), _opts(opts), _pixels({}) {
	_asyncThread = std::thread([&](const std::string& path) {
		CE_OBJECT_SET_ASYNC_LOADING;
		std::string ss = path.substr(path.find_last_of('.') + 1, std::string::npos);

		GLenum rgb = GL_RGB, rgba = GL_RGBA;

		if (ss == "jpg") {
			if (!Texture_I::FromJPG(path, _width, _height, _channels, _pixels))
				return;
		}
		else if (ss == "png") {
			if (!Texture_I::FromPNG(path, _width, _height, _channels, _pixels))
				return;
			//rgb = GL_BGR;
			//rgba = GL_BGRA;
		}
		else if (ss == "bmp") {
			if (!Texture_I::FromBMP(path, _width, _height, _channels, _pixels))
				return;
		}
		else if (ss == "hdr") {
			if (!Texture_I::FromHDR(path, _width, _height, _channels, _pixels))
				return;
			_hdr = true;
		}
		else {
			Debug::Error("Texture", "Cannot determine format of \"" + path + "\"!");
			return;
		}

		CE_OBJECT_SET_ASYNC_READY;
	}, path);

	CE_OBJECT_INIT_ASYNC;
}

_Texture::~_Texture() {
	CE_OBJECT_FINALIZE_ASYNC;
	glDeleteTextures(1, &_pointer);
}

bool _Texture::loaded() {
	CE_OBJECT_CHECK_ASYNC;
	return !!_pointer;
}

void _Texture::LoadAsync() {
	glGenTextures(1, &_pointer);
	glBindTexture(GL_TEXTURE_2D, _pointer);
	if (_channels == 1) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, _hdr ? GL_R32F : GL_RED, _width, _height, 0, GL_RED, _hdr ? GL_FLOAT : GL_UNSIGNED_BYTE, _pixels.data());
	}
	else if (_channels == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, _hdr ? GL_RGB32F : GL_RGB, _width, _height, 0, GL_RGB, _hdr ? GL_FLOAT : GL_UNSIGNED_BYTE, _pixels.data());
	else if (_channels == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, _hdr ? GL_RGBA32F : GL_RGBA, _width, _height, 0, GL_RGBA, _hdr ? GL_FLOAT : GL_UNSIGNED_BYTE, _pixels.data());
	else {
		Debug::Error("Texture", "Unexpected channel size " + std::to_string(_channels) + "!");
	}
	if (_opts.mipmaps > 0) glGenerateMipmap(GL_TEXTURE_2D);
	const GLenum wraps[] = { GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT };
	SetTexParams<>(0,
		wraps[(int)_opts.xwrap],
		wraps[(int)_opts.ywrap],
		(_opts.linear) ? (
		(_opts.mipmaps > 0) ?
			GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR
			) : GL_NEAREST,
			(_opts.linear) ?
		GL_LINEAR : GL_NEAREST
		);
	glBindTexture(GL_TEXTURE_2D, 0);
	std::vector<byte> empty(0);
	std::swap(_pixels, empty);

	_loading = false;
}

void _Texture::Bind() {
	CE_OBJECT_CHECK_ASYNC;
	glBindTexture(GL_TEXTURE_2D, _pointer);
}

void _Texture::Unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void _Texture::Blit(const RenderTarget& dst, const Material& mat) {
	dst->BindTarget();
	glViewport(0, 0, dst->width(), dst->height());
	Ref<_Texture> tex(std::static_pointer_cast<_Texture>(shared_from_this()));
	glBlendFunc(GL_ONE, GL_ZERO);
	if (!mat) {
		UI::Texture(Rect(0, 0, Display::width(), Display::height()), tex);
	}
	else {
		mat->SetUniform("mainTex", tex);
		mat->Bind();
		Backend::Renderer::emptyVao()->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		Backend::Renderer::emptyVao()->Unbind();
		mat->Unbind();
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, Display::width(), Display::height());
	dst->UnbindTarget();
}

CE_END_NAMESPACE
