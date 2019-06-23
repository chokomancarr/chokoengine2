#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class ShaderVariable {
    ShaderVariable(const std::string&, ShaderVariableType, GLuint); //only allow shader to construct

    std::string _name;
    ShaderVariableType _type;
    GLint _location;

	float _val_f;
	int _val_i;
	Vec2 _val_v2;
	Vec3 _val_v3;
	Vec4 _val_v4;
	Mat4x4 _val_m;
	Texture _val_t;

public:
    CE_GET_MEMBER(name);
    CE_GET_MEMBER(type);

	CE_GET_SET_MEMBER(val_f);
	CE_GET_SET_MEMBER(val_i);
	CE_GET_SET_MEMBER(val_v2);
	CE_GET_SET_MEMBER(val_v3);
	CE_GET_SET_MEMBER(val_v4);
	CE_GET_SET_MEMBER(val_m);
	CE_GET_SET_MEMBER(val_t);

    friend _Shader;
	friend _Material;
};

CE_END_NAMESPACE