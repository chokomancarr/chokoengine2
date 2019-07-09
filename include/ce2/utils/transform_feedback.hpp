#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _TransformFeedback { CE_CLASS_COMMON
	VertexArray _vao;
	std::vector<VertexBuffer> _outputs;
	GLuint _program;
	std::vector<GLint> _uniforms;

public:
	_TransformFeedback(const std::string& shader, std::initializer_list<const char*> outNms);
	
	void AddUniforms(std::initializer_list<const char*> nms);

	CE_GET_SET_MEMBER(vao);
	CE_GET_SET_MEMBER(outputs);

	void Bind(), Unbind();
	void Exec(int n = 0);
};

typedef std::shared_ptr<_TransformFeedback> TransformFeedback;
TransformFeedback TransformFeedback_New(const std::string& shader, std::initializer_list<const char*> outNms);

CE_END_NAMESPACE