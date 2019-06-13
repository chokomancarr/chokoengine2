#pragma once
#include "chokoengine.hpp"

BEGIN_CE_NAMESPACE

class UI { CE_CLASS_COMMON

    static uint _vboSz;
	static GLuint _vao, _vboV, _vboU, _tvbo;
    static GLuint _quadElo;

    static Shader texShad;

    static float _alpha;
    static glm::mat3 matrix;
    static bool matrixIsI;

    static bool Init();

    static void InitVao();
    static void SetVao(uint sz, void* vts, void* uvs);

    static float Dw(float f);
    static float Dh(float f);
    static Vec3 Ds(Vec3 v);
    static Vec2 Ds2(Vec2 v);

    static void TexQuad(const CE_NS Rect& q, 
        GLuint tex, Color col = Color::white(), int mip = 0, 
        Vec2 uv0 = Vec2(0, 1), Vec2 uv1 = Vec2(1, 1), 
        Vec2 uv2 = Vec2(0, 0), Vec2 uv3 = Vec2(1, 0));
public:
    GET_SET_ST_MEMBER_FUNC(float, alpha);

    /* Draws an image with the provided dimensions
     */
    static void Texture(const CE_NS Rect& rect, const CE_NS Texture& tex, const Color& color = Color::white());

    /* Draws a rectangle with the provided dimensions
     */
    static void Rect(const CE_NS Rect& rect, const Color& color);
};

END_CE_NAMESPACE