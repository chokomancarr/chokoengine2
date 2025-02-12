#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

/* Render Target object used for off-screen rendering
 */
class _RenderTarget : public _Texture { CE_OBJECT_COMMON
	GLuint _depth;
	GLuint _fbo;

	_RenderTarget(uint w, uint h, GLuint tex, GLuint depth, GLuint fbo);
public:
	/* Constructs a new render target with dimensions \p w and \p h
	 * If \p hdr is true, the buffer will be 32-bit instead of 8-bit
	 * \p depth specifies if a depth buffer is created
	 */
	_RenderTarget(uint w, uint h, bool hdr, bool depth, const TextureOptions opts = TextureOptions());

	~_RenderTarget();

	void Clear(const Color& c, const float d) const;

	void BindTarget(bool read = false) const;
	void UnbindTarget(bool read = false) const;

	friend class _FrameBufferCube;
};

CE_END_NAMESPACE