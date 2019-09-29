#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _Material : public _Asset { CE_OBJECT_COMMON

	_Material (const _Material& rhs) = delete;

    Shader _shader;

	std::vector<ShaderVariable> _variables;
public:
    _Material();

    CE_GET_MEMBER(shader);
    CE_SET_MEMBER_F(shader);

	const float& GetUniform(const std::string& s, float f);
	const int& GetUniform(const std::string& s, int i);
	const Vec2& GetUniform(const std::string& s, Vec2 v);
	const Vec3& GetUniform(const std::string& s, Vec3 v);
	const Vec4& GetUniform(const std::string& s, Vec4 v);
	const Mat4x4& GetUniform(const std::string& s, Mat4x4 m);
	const Color& GetUniform(const std::string& s, Color c);
	const Texture& GetUniform(const std::string& s, Texture t);

	void SetUniform(const std::string& s, float f);
	void SetUniform(const std::string& s, int i);
	void SetUniform(const std::string& s, Vec2 v);
	void SetUniform(const std::string& s, Vec3 v);
	void SetUniform(const std::string& s, Vec4 v);
	void SetUniform(const std::string& s, Mat4x4 m);
	void SetUniform(const std::string& s, Color c);
	void SetUniform(const std::string& s, Texture t);
	void SetUniform(const std::string& s, CubeMap c);

	void Bind();
	void Unbind();
};

CE_END_NAMESPACE