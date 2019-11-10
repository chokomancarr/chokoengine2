#pragma once
#include "chokoengine.hpp"

#include "enums/shader_blend.hpp"
#include "enums/shader_giflags.hpp"
#include "enums/shader_queue.hpp"
#include "enums/shader_type.hpp"
#include "enums/shader_variable_type.hpp"
#include "enums/shader_ztest.hpp"
#include "shader/shader_variable.hpp"

CE_BEGIN_NAMESPACE

class _Shader : public _Asset { CE_OBJECT_COMMON
public:
	struct GIParams {
		std::string emissionStrVar;
		std::string emissionColVar;
		std::string emissionTexVar;
	};

private:
	GLuint pointer;
	std::vector<GLuint> pointers;
	std::vector<ShaderVariable> variables;
	std::vector<std::pair<std::string, bool>> options;

	SHADER_GI_FLAGS _giFlags;
	GIParams _giParams;

	ShaderQueue _queue;
	ShaderZTest _ztest;
	ShaderBlend _blendSrc, _blendDst;

	static bool LoadShader(GLenum shaderType, std::string source, GLuint& shader, std::string* err = nullptr);
	static bool LinkShader(GLuint prog);

	static GLuint FromVF(const std::string& vert, const std::string& frag);

	void UpdatePointer();

	void ApplyFlags();

public:
	_Shader();
	_Shader(const std::string& vert, const std::string& frag);
	_Shader(std::vector<std::string> strs, const std::vector<ShaderType>& typs, const std::vector<std::string>& options = {});

	void SetOptions(const std::initializer_list<std::string>& nms);
	void SetOption(const std::string& nm, bool on);

	void AddUniform(const std::string& s, ShaderVariableType t);
	void AddUniforms(std::initializer_list<const std::string> ss);

	void RegisterStandardUniforms();

	void Bind() const;
	void Unbind() const;

	CE_GET_SET_MEMBER(giFlags);
	CE_GET_SET_MEMBER(giParams);

	CE_GET_SET_MEMBER(queue);
	CE_GET_SET_MEMBER(ztest);
	CE_GET_SET_MEMBER(blendSrc);
	CE_GET_SET_MEMBER(blendDst);

	GLint Loc(int) const;

	friend class UI;
	friend class _Material;
};

CE_END_NAMESPACE