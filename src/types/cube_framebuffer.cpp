#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_FrameBufferCube::_FrameBufferCube(uint r, std::vector<GLenum> types) 
		: _maps(types.size(), nullptr), _depth(nullptr) {

	std::vector<GLenum> bufs(types.size());
	for (size_t a = 0; a < types.size(); a++) {
		_maps[a] = CubeMap::New(r, types[a], TextureOptions(
			TextureWrap::Clamp, TextureWrap::Clamp, 10, true
		), 0);
		bufs[a] = GL_COLOR_ATTACHMENT0 + (GLsizei)a;
	}
	_depth = DepthCubeMap_New(r);

	for (int f = 0; f < 6; f++) {
		GLuint fbo;
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
		for (size_t a = 0; a < types.size(); a++) {
			glFramebufferTexture2D(GL_FRAMEBUFFER, bufs[a], GL_TEXTURE_CUBE_MAP_POSITIVE_X + f, _maps[a]->_pointer, 0);
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + f, _depth->_pointer, 0);
		glDrawBuffers((GLsizei)bufs.size(), bufs.data());
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			Debug::Error("FrameBuffer", "gl error " + std::to_string(status));
		}
		//this is not good, but will do for now
		//pass null textures to prevent double cleanup
		_pointers[f] = RenderTarget::FromPtr(new _RenderTarget(_depth->_reso, _depth->_reso, 0, 0, fbo));
	}
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

const CubeMap& _FrameBufferCube::map(int i) {
	return _maps[i];
}

void _FrameBufferCube::Clear() const {
	float zeros[4] = {};
	float one = 1;
	for (int a = 0; a < _maps.size(); a++) {
		glClearBufferfv(GL_COLOR, a, zeros);
	}
	glClearBufferfv(GL_DEPTH, 0, &one);
}

void _FrameBufferCube::Bind(CubeMapFace f) const {
	_pointers[(int)f]->BindTarget();
}

void _FrameBufferCube::Unbind() const {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

FrameBufferCube FrameBufferCube_New(uint r, std::vector<GLenum> types) {
	return std::make_shared<_FrameBufferCube>(r, types);
}

CE_END_NAMESPACE