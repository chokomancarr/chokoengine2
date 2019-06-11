#pragma once
#include "chokoengine.hpp"

BEGIN_CE_NAMESPACE

class Shader : public RefCnt {
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
	CE_OBJECT_COMMON;

	Shader() : pointer(0) {}
	Shader(const std::string& vert, const std::string& frag);

	REFCNT_DESTRUCTOR(Shader);

	operator bool() const {
		return !!pointer;
	}
	operator GLuint() const {
		return pointer;
	}

	Shader& AddUniform(const std::string& s);
	Shader& AddUniforms(std::initializer_list<const std::string> ss);

	void SetOptions(const std::initializer_list<std::string>& nms);
	void SetOption(const std::string& nm, bool on);

	void Bind();
	void Unbind();
};

END_CE_NAMESPACE