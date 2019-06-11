#include "chokoengine.hpp"

BEGIN_CE_NAMESPACE

bool Shader::LoadShader(GLenum shaderType, std::string source, GLuint& shader, std::string* err) {
	int compile_result = 0;

	shader = glCreateShader(shaderType);
#ifdef PLATFORM_WIN
	const char *shader_code_ptr = source.c_str();
	const int shader_code_size = source.size();
	glShaderSource(shader, 1, &shader_code_ptr, &shader_code_size);
#else
	const char *strings[1] = { NULL };
	strings[0] = source.c_str();
	glShaderSource(shader, 1, strings, NULL);
#endif
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);

	//check for errors
	if (!compile_result)
	{
		if (err) {
			int info_log_length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
			if (!!info_log_length) {
				std::vector<char> shader_log(info_log_length);
				glGetShaderInfoLog(shader, info_log_length, NULL, &shader_log[0]);
				shader = 0;
				*err += std::string(&shader_log[0]);
				Debug::Error("Shader", *err);
			}
		}
		glDeleteShader(shader);
		return false;
	}
	return true;
}

bool Shader::LinkShader(GLuint pointer) {
	int link_result = 0;
	glLinkProgram(pointer);
	glGetProgramiv(pointer, GL_LINK_STATUS, &link_result);
	if (link_result == GL_FALSE) {
		int info_log_length = 0;
		glGetProgramiv(pointer, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> program_log(info_log_length);
		glGetProgramInfoLog(pointer, info_log_length, NULL, &program_log[0]);
		Debug::Error("Shader", "Link error: " + std::string(&program_log[0]));
		return false;
	}
	return true;
}

GLuint Shader::FromVF(const std::string& vert, const std::string& frag) {
	GLuint vertex_shader, fragment_shader;
	std::string err = "";
	if (vert == "" || frag == "") {
		Debug::Error("Shader Compiler", "vert or frag is empty!");
		return 0;
	}

	if (!LoadShader(GL_VERTEX_SHADER, vert, vertex_shader, &err)) {
		Debug::Error("Shader Compiler", "Vert error: " + err);
		return 0;
	}

	if (!LoadShader(GL_FRAGMENT_SHADER, frag, fragment_shader, &err)) {
		Debug::Error("Shader Compiler", "Frag error: " + err);
		return 0;
	}

	GLuint pointer = glCreateProgram();
	glAttachShader(pointer, vertex_shader);
	glAttachShader(pointer, fragment_shader);

	if (!LinkShader(pointer)) {
		glDeleteProgram(pointer);
		return 0;
	}

	glDetachShader(pointer, vertex_shader);
	glDetachShader(pointer, fragment_shader);
	glDeleteShader(fragment_shader);
	glDeleteShader(vertex_shader);
	return pointer;
}

GLint Shader::Loc(int i) {
	return uniforms[i];
}

void Shader::UpdatePointer() {
	uint i = 0, j = 0;
	for (auto& v : options) {
		if (v.second)
			i |= (1 << j);
		j++;
	}
	pointer = pointers[j];
}

Shader::Shader(const std::string& vert, const std::string& frag) {
	uint vers = 1;
	std::istringstream vstrm(vert);
	std::string s;
	while (std::getline(vstrm, s)) {
		if (s[0] != '#') continue;
		auto ss = StrExt::Split(s, ' ', true);
		if (ss[0] == "#pragma") {
			if (ss[1] == "option") {
				options.push_back(std::make_pair(ss[2], false));
				vers *= 2;
			}
		}
	}

	for (uint v = 0; v < vers; v++) {
		std::string prep = "#version 330 core\n";
		for (int a = 0; a < options.size(); a++) {
			if (!!(v & (1 << a))) {
				prep += "#define " + options[a].first + "\n";
			}
		}
		pointers.push_back(FromVF(prep + vert, prep + frag));
	}
	pointer = pointers[0];
}

void Shader::DestroyRef() {
	if (!!pointer)
		glDeleteProgram(pointer);
}

Shader& Shader::AddUniform(const std::string& s) {
	uniforms.push_back(glGetUniformLocation(pointer, s.c_str()));
	return *this;
}

Shader& Shader::AddUniforms(std::initializer_list<const std::string> ss) {
	for (auto& s : ss)
		AddUniform(s);
	return *this;
}

void Shader::SetOptions(const std::initializer_list<std::string>& nms) {
	for (auto& v : options) {
		v.second = std::find(nms.begin(), nms.end(), v.first) != nms.end();
	}
	UpdatePointer();
}

void Shader::SetOption(const std::string& nm, bool on) {
	for (auto& v : options) {
		if (v.first == nm) {
			v.second = on;
			break;
		}
	}
	UpdatePointer();
}

void Shader::Bind() {
	glUseProgram(pointer);
}

void Shader::Unbind() {
	glUseProgram(0);
}

END_CE_NAMESPACE