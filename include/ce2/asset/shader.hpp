#pragma once
#include "chokoengine.hpp"

#include "enums/shader_variable_type.hpp"
#include "shader/shader_variable.hpp"

CE_BEGIN_NAMESPACE

class _Shader : public _Asset { CE_OBJECT_COMMON

	GLuint pointer;
	std::vector<GLuint> pointers;
	std::vector<ShaderVariable> variables;
	std::vector<std::pair<std::string, bool>> options;

	static bool LoadShader(GLenum shaderType, std::string source, GLuint& shader, std::string* err = nullptr);
	static bool LinkShader(GLuint prog);

	static GLuint FromVF(const std::string& vert, const std::string& frag);

	GLint Loc(int);

	void UpdatePointer();
public:
	_Shader();
	_Shader(const std::string& vert, const std::string& frag);

	void SetOptions(const std::initializer_list<std::string>& nms);
	void SetOption(const std::string& nm, bool on);

	void AddUniform(const std::string& s, ShaderVariableType t);
	void AddUniforms(std::initializer_list<const std::string> ss);

	void Bind();
	void Unbind();

	friend class UI;
	friend class _Material;
};

CE_END_NAMESPACE