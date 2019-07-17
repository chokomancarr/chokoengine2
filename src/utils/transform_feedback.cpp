#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

_TransformFeedback::_TransformFeedback(const std::string& shader, std::initializer_list<const char*> outNms) {
	auto _shader = glCreateShader(GL_VERTEX_SHADER);
	const char* cc[1] = { shader.c_str() };
	glShaderSource(_shader, 1, cc, nullptr);
	glCompileShader(_shader);
	int compile_result;
	glGetShaderiv(_shader, GL_COMPILE_STATUS, &compile_result);
	if (!compile_result) {
		int info_log_length = 0;
		glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &info_log_length);
		if (!!info_log_length) {
			std::vector<char> shader_log(info_log_length);
			glGetShaderInfoLog(_shader, info_log_length, NULL, &shader_log[0]);
			_shader = 0;
			Debug::Error("_TransformFeedback", std::string(shader_log.data(), info_log_length));
		}
		glDeleteShader(_shader);
		return;
	}
	_program = glCreateProgram();
	glAttachShader(_program, _shader);
	glTransformFeedbackVaryings(_program, outNms.size(), outNms.begin(), GL_SEPARATE_ATTRIBS);
	int link_result = 0;
	glLinkProgram(_program);
	glGetProgramiv(_program, GL_LINK_STATUS, &link_result);
	if (!link_result) {
		int info_log_length = 0;
		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> program_log(info_log_length);
		glGetProgramInfoLog(_program, info_log_length, NULL, &program_log[0]);
		Debug::Error("_TransformFeedback", "Link error: " + std::string(program_log.data(), info_log_length));
		glDeleteProgram(_program);
		_program = 0;
		return;
	}
	glDetachShader(_program, _shader);
	glDeleteShader(_shader);
}

void _TransformFeedback::AddUniforms(std::initializer_list<const char*> nms) {
	for (auto nm : nms) {
		_uniforms.push_back(glGetUniformLocation(_program, nm));
	}
}

GLint _TransformFeedback::Loc(int i) const {
	return _uniforms[i];
}

void _TransformFeedback::Bind() {
	glUseProgram(_program);
	_vao->Bind();
	for (int a = 0; a < _outputs.size(); a++) {
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, a, _outputs[a]->_pointer);
	}
}

void _TransformFeedback::Unbind() {
	for (int a = 0; a < _outputs.size(); a++) {
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, a, 0);
	}
	glBindVertexArray(0);
	glUseProgram(0);
}

void _TransformFeedback::Exec(int n) {
	glEnable(GL_RASTERIZER_DISCARD);
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, (n > 0) ? n : _vao->buffer(0)->num() * 3);
	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);
	glFlush();
}

TransformFeedback TransformFeedback_New(const std::string& shader, std::initializer_list<const char*> outNms) {
	return std::make_shared<_TransformFeedback>(shader, outNms);
}

CE_END_NAMESPACE