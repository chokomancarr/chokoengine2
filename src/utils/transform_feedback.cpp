#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

namespace {
	bool trycompile(GLuint& shad) {
		glCompileShader(shad);
		int compile_result;
		glGetShaderiv(shad, GL_COMPILE_STATUS, &compile_result);
		if (!compile_result) {
			int info_log_length = 0;
			glGetShaderiv(shad, GL_INFO_LOG_LENGTH, &info_log_length);
			if (!!info_log_length) {
				std::vector<char> shader_log(info_log_length);
				glGetShaderInfoLog(shad, info_log_length, NULL, &shader_log[0]);
				shad = 0;
				Debug::Error("_TransformFeedback", std::string(shader_log.data(), info_log_length));
			}
			glDeleteShader(shad);
			return false;
		}
		return true;
	}
}

_TransformFeedback::_TransformFeedback(const std::string& shader, std::initializer_list<const char*> outNms)
	: _TransformFeedback(shader, "", outNms) {}

_TransformFeedback::_TransformFeedback(const std::string& vert, const std::string& geom, std::initializer_list<const char*> outNms) {
	const bool hasg = geom != "";

	const char* cc[2] = { vert.c_str(), geom.c_str() };
	GLuint _vshad = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(_vshad, 1, cc, nullptr);
	if (!trycompile(_vshad)) {
		return;
	}
	GLuint _gshad;
	if (hasg) {
		 _gshad = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(_gshad, 1, cc + 1, nullptr);
		if (!trycompile(_gshad)) {
			glDeleteShader(_vshad);
			return;
		}
	}
	
	_program = glCreateProgram();
	glAttachShader(_program, _vshad);
	if (hasg) {
		glAttachShader(_program, _gshad);
	}
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
	glDetachShader(_program, _vshad);
	glDeleteShader(_vshad);
	if (hasg) {
		glDetachShader(_program, _gshad);
		glDeleteShader(_gshad);
	}
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

void _TransformFeedback::Exec(int n, GLuint type) {
	glEnable(GL_RASTERIZER_DISCARD);
	glBeginTransformFeedback(type);
	glDrawArrays(GL_POINTS, 0, (n > 0) ? n : _vao->buffer(0)->num() * 3);
	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);
	glFlush();
}

TransformFeedback TransformFeedback_New(const std::string& shader, std::initializer_list<const char*> outNms) {
	return std::make_shared<_TransformFeedback>(shader, outNms);
}

TransformFeedback TransformFeedback_New(const std::string& vert, const std::string& geom, std::initializer_list<const char*> outNms) {
	return std::make_shared<_TransformFeedback>(vert, geom, outNms);
}

CE_END_NAMESPACE