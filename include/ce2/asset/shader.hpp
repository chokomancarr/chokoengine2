#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _Shader : public _Asset { CE_OBJECT_COMMON

	GLuint pointer;
	std::vector<GLuint> pointers;
	std::vector<GLint> uniforms;
	std::vector<std::pair<std::string, bool>> options;

	static bool LoadShader(GLenum shaderType, std::string source, GLuint& shader, std::string* err = nullptr);
	static bool LinkShader(GLuint prog);

	static GLuint FromVF(const std::string& vert, const std::string& frag);

	GLint Loc(int);

	void UpdatePointer();
public:
	_Shader();
	_Shader(const std::string& vert, const std::string& frag);

	operator bool() const;

	void AddUniform(const std::string& s);
	void AddUniforms(std::initializer_list<const std::string> ss);

	void SetOptions(const std::initializer_list<std::string>& nms);
	void SetOption(const std::string& nm, bool on);

	void Bind();
	void Unbind();

	friend class UI;
};

CE_END_NAMESPACE