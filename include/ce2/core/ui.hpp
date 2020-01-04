#pragma once
#include "chokoengine.hpp"
#include "enums/ui_scaling.hpp"

CE_BEGIN_NAMESPACE

class UI { CE_CLASS_COMMON

	static CE_EXPORT uint _vboSz;
	static CE_EXPORT VertexBuffer _vboV, _vboU;
	static CE_EXPORT VertexArray _vao;
	static CE_EXPORT TextureBuffer _tvbo;
	static CE_EXPORT VertexBuffer _quadElo;

	static CE_EXPORT Shader colShad;
	static CE_EXPORT Shader texShad;

	static CE_EXPORT float _alpha;
	static CE_EXPORT glm::mat3 matrix;
	static CE_EXPORT bool matrixIsI;

	static CE_EXPORT Font _defaultFont;

	static CE_NS Rect _stencilRect;
	static std::vector<CE_NS Rect> _stencilRects;

	static bool Init();

	static void InitVao();
	static void SetVao(uint sz, const void* vts, const void* uvs);

	static float Dw(float f);
	static float Dh(float f);
	static Vec3 Ds(Vec3 v);
	static Vec2 Ds2(Vec2 v);

	static std::array<Vec3, 4> GetQuadVecs(const CE_NS Rect& r);

	static void TexQuad(const CE_NS Rect& q,
		GLuint tex, Color col = Color::white(), int mip = 0,
		Vec2 uv0 = Vec2(0, 1), Vec2 uv1 = Vec2(1, 1),
		Vec2 uv2 = Vec2(0, 0), Vec2 uv3 = Vec2(1, 0));
public:
	CE_GET_SET_ST_MEMBER(alpha);
	CE_GET_SET_ST_MEMBER(defaultFont);

	/* The current active portion of the screen.
	 * If no stencil is set, the fullscreen rect is returned.
	 */
	static Rect stencilRect();

	/* Draws an image with the provided dimensions.
	 */
	static void Texture(const CE_NS Rect& rect, const CE_NS Texture& tex, const Color& color = Color::white());
	/* Draws an image with the provided dimensions.
	 */
	static void Texture(const CE_NS Rect& rect, const CE_NS Texture& tex, const UIScaling& scaling, const Color& color = Color::white());

	/* Draws a rectangle with the provided dimensions
	 */
	static void Rect(const CE_NS Rect& rect, const Color& color);

	/* Draws a rectangle with the provided dimensions,
	 * using the specified material
	 * The following inputs will be provided to the shader:
	 *   layout(location = 0) in vec3 pos;
	 *   layout(location = 1) in vec2 uv;
	 */
	static void Rect(const CE_NS Rect& rect, const Material& mat);

	/* Draws a straight line connecting two points
	 */
	static void Line(const Vec2& p1, const Vec2& p2, const Color& col);

	/* Draws a UTF-8 string within the provided dimensions
	 * The text will be abbreviated (The quick fox -> The q...)
	 * if the text is longer than the width provided
	 */
	static void Label(const CE_NS Rect& rect, const std::string& str, const Color& col, const Font& font = _defaultFont);

	/* Draws a unicode string within the provided dimensions
	 * The text will be abbreviated (The quick fox -> The q...)
	 * if the text is longer than the width provided
	 */
	static void Label(const CE_NS Rect& rect, const std::u32string& str, const Color& col, const Font& font = _defaultFont);

	/* Defines the area to limit subsequent drawings
	 */
	static void BeginStencil(const CE_NS Rect& rect);

	/* Defines the area to limit subsequent drawings
	 * If \p useAlpha is true, the alpha channel controls visibility,
	 * otherwise the red channel is used
	 */
	static void BeginStencil(const CE_NS Texture& tex, bool useAlpha);

	/* Adds a stencil layer
	 * This maintains an internal stack of Rects
	 * If the previous stencil is a texture, it is removed
	 */
	static void PushStencil(const CE_NS Rect& rect);

	/* Removes the topmost stencil layer
	 */
	static void PopStencil();

	/* Clears all stencil layers
	 */
	static void EndStencil();

    /* Interactive UI elements
     * (see interactive_ui.hpp)
     */
    class I;
	/* World-space UI elements
	 * (see world_ui.hpp)
	 */
	class W;
};

CE_END_NAMESPACE

#include "interactive_ui.hpp"
#include "world_ui.hpp"