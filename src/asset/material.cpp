#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

Color _Material::GetGIDiffuseCol() {
	for (auto& v : _variables) {
		if (v.name() == _shader->_giParams.diffuseColVar) {
			return v.val_v4();
		}
	}
	return Color(1);
}
const Texture& _Material::GetGIDiffuseTex() {
	for (auto& v : _variables) {
		if (v.name() == _shader->_giParams.diffuseTexVar) {
			return v.val_t();
		}
	}
}
float _Material::GetGIEmissionStr() {
	for (auto& v : _variables) {
		if (v.name() == _shader->_giParams.emissionStrVar) {
			return v.val_f();
		}
	}
	return 1;
}
Color _Material::GetGIEmissionCol() {
	for (auto& v : _variables) {
		if (v.name() == _shader->_giParams.emissionColVar) {
			return v.val_v4();
		}
	}
	return Color(1);
}
const Texture& _Material::GetGIEmissionTex() {
	for (auto& v : _variables) {
		if (v.name() == _shader->_giParams.emissionTexVar) {
			return v.val_t();
		}
	}
}


_Material::_Material() : _shader(nullptr), _variables({}) {}

void _Material::shader(const Shader& s) {
	_shader = s;
	_variables = s->variables;
}

#define GETUNIFORM(type, enm, nm)\
const type& _Material::GetUniform(const std::string& s, type nm) {\
	/*_shader->AddUniform(s, ShaderVariableType::enm);  make sure the variable exists */\
	for (auto& v : _variables) {\
		if (v.name() == s) {\
			return v.val_##nm();\
		}\
	}\
	Debug::Error("Material::GetUniform", "Uniform \"" + s + "\" is not in material!");\
	return _variables[0].val_##nm();\
}

GETUNIFORM(float, Float, f);
GETUNIFORM(int, Int, i);
GETUNIFORM(Vec2, Vec2, v2);
GETUNIFORM(Vec3, Vec3, v3);
GETUNIFORM(Vec4, Vec4, v4);
GETUNIFORM(Mat4x4, Matrix, m);
GETUNIFORM(Color, Color, v4);
GETUNIFORM(Texture, Texture, t);

#define SETUNIFORM(type, enm, nm)\
void _Material::SetUniform(const std::string& s, type nm) {\
	/*_shader->AddUniform(s, ShaderVariableType::enm);  make sure the variable exists */\
	for (auto& v : _variables) {\
		if (v.name() == s) {\
			v.val_##nm(nm);\
			return;\
		}\
	}\
	Debug::Error("Material::GetUniform", "Uniform \"" + s + "\" is not in material!");\
}

SETUNIFORM(float, Float, f);
SETUNIFORM(int, Int, i);
SETUNIFORM(Vec2, Vec2, v2);
SETUNIFORM(Vec3, Vec3, v3);
SETUNIFORM(Vec4, Vec4, v4);
SETUNIFORM(Mat4x4, Matrix, m);
SETUNIFORM(Color, Color, v4);
SETUNIFORM(Texture, Texture, t);
SETUNIFORM(CubeMap, CubeMap, cm);

void _Material::Bind() {
	_shader->Bind();
	int tid = 0;
	for (auto& v : _variables) {
		switch (v._type) {
		case ShaderVariableType::Float:
			glUniform1f(v._location, v._val_f);
			break;
		case ShaderVariableType::Int:
			glUniform1i(v._location, v._val_i);
			break;
		case ShaderVariableType::Vec2:
			glUniform2f(v._location, v._val_v2.x, v._val_v2.y);
			break;
		case ShaderVariableType::Vec3:
			glUniform3f(v._location, v._val_v3.x, v._val_v3.y, v._val_v3.z);
			break;
		case ShaderVariableType::Vec4:
		case ShaderVariableType::Color:
			glUniform4f(v._location, v._val_v4.x, v._val_v4.y, v._val_v4.z, v._val_v4.w);
			break;
		case ShaderVariableType::Matrix:
			glUniformMatrix4fv(v._location, 1, false, &v._val_m[0][0]);
			break;
		case ShaderVariableType::Texture:
			if (!v._val_t) break;
			glUniform1i(v._location, tid);
			glActiveTexture(GL_TEXTURE0 + tid);
			v._val_t->Bind();
			tid++;
			break;
		case ShaderVariableType::CubeMap:
			if (!v._val_cm) break;
			glUniform1i(v._location, tid);
			glActiveTexture(GL_TEXTURE0 + tid);
			v._val_cm->Bind();
			tid++;
			break;
		}
	}
}

void _Material::Unbind() {
	_shader->Unbind();
}

CE_END_NAMESPACE