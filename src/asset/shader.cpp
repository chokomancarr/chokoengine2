#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

bool _Shader::LoadShader(GLenum shaderType, std::string source, GLuint& shader, std::string* err) {
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
				//Debug::Error("_Shader", *err);
			}
		}
		glDeleteShader(shader);
		return false;
	}
	return true;
}

bool _Shader::LinkShader(GLuint pointer) {
	int link_result = 0;
	glLinkProgram(pointer);
	glGetProgramiv(pointer, GL_LINK_STATUS, &link_result);
	if (link_result == GL_FALSE) {
		int info_log_length = 0;
		glGetProgramiv(pointer, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> program_log(info_log_length);
		glGetProgramInfoLog(pointer, info_log_length, NULL, &program_log[0]);
		Debug::Error("_Shader", "Link error: " + std::string(&program_log[0]));
		return false;
	}
	return true;
}

GLuint _Shader::FromVF(const std::string& vert, const std::string& frag) {
	GLuint vertex_shader, fragment_shader;
	std::string err = "";
	if (vert == "" || frag == "") {
		Debug::Error("_Shader Compiler", "vert or frag is empty!");
		return 0;
	}

	if (!LoadShader(GL_VERTEX_SHADER, vert, vertex_shader, &err)) {
		Debug::Error("_Shader Compiler", "Vert error: " + err);
		return 0;
	}

	if (!LoadShader(GL_FRAGMENT_SHADER, frag, fragment_shader, &err)) {
		Debug::Error("_Shader Compiler", "Frag error: " + err);
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

void _Shader::UpdatePointer() {
	uint i = 0, j = 0;
	for (auto& v : options) {
		if (v.second)
			i |= (1 << j);
		j++;
	}
	pointer = pointers[i];
	for (auto& v : variables) {
		v._location = v._locations[i];
	}
}

void _Shader::ApplyFlags() {
	static const GLenum zt[] = {
		GL_NEVER,
		GL_ALWAYS,
		GL_LESS,
		GL_LEQUAL,
		GL_EQUAL,
		GL_GEQUAL,
		GL_GREATER
	};
	glDepthFunc(zt[(int)_ztest]);
	if (_queue == ShaderQueue::Opaque) {
		//glBlendFunc(GL_ONE, GL_ZERO);
	}
	else {
		static const GLenum bl[] = {
			GL_ZERO,
			GL_ONE,
			GL_SRC_ALPHA,
			GL_DST_ALPHA,
			GL_ONE_MINUS_SRC_ALPHA,
			GL_ONE_MINUS_DST_ALPHA
		};
		//glBlendFunc(bl[(int)_blendSrc], bl[(int)_blendDst]);
	}
}

_Shader::_Shader() : pointer(0), pointers({}), variables({}), options({}),
		_giFlags(0), _giParams({}), _queue(ShaderQueue::Opaque),
		_ztest(ShaderZTest::LessEqual), _blendSrc(ShaderBlend::SrcAlpha),
		_blendDst(ShaderBlend::OneMinusSrcAlpha) {}

_Shader::_Shader(const std::string& vert, const std::string& frag) : _Shader() {
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

_Shader::_Shader(std::vector<std::string> strs, const std::vector<ShaderType>& typs, const std::vector<std::string>& opts) : _Shader() {
	const auto n = strs.size();
	const GLenum t2e[] = {
		GL_VERTEX_SHADER,
		GL_GEOMETRY_SHADER,
		GL_TESS_CONTROL_SHADER,
		GL_TESS_EVALUATION_SHADER,
		GL_FRAGMENT_SHADER
	};

	int vers = 1;
	for (auto& o : opts) {
		options.push_back(std::make_pair(o, false));
		vers *= 2;
	}

	std::vector<GLuint> sps(n);
	std::vector<std::string> drcs(n);
	std::string err = "";

	for (int a = 0; a < n; a++) {
		auto& s = strs[a];
		const auto hl = s.find_first_of('#');
		const auto nl = s.find_first_of('\n', hl + 1);
		if (s.substr(hl + 1, 7) == "version") {
			drcs[a] = s.substr(hl, nl - hl + 1);
			s = s.substr(nl + 1);
		}
		else {
			Debug::Error("_Shader Compiler", "First '#' statement is not version statement!");
			return;
		}
	}

	for (int v = 0; v < vers; v++) {
		std::string str = "";
		for (int a = 0; a < options.size(); a++) {
			if (!!(v & (1 << a))) {
				str += "#define " + options[a].first + "\n";
			}
		}

		for (size_t a = 0; a < n; a++) {
			if (!LoadShader(t2e[(int)typs[a]], drcs[a] + str + strs[a], sps[a], &err)) {
				Debug::Error("_Shader Compiler", "Comp " + std::to_string(a) + " error: " + err);
				for (size_t b = 0; b <= a; b++) {
					glDeleteShader(sps[b]);
				}
			}
		}

		pointer = glCreateProgram();
		for (size_t a = 0; a < n; a++) {
			glAttachShader(pointer, sps[a]);
		}

		if (!LinkShader(pointer)) {
			glDeleteProgram(pointer);
			pointer = 0;
		}

		for (size_t a = 0; a < n; a++) {
			glDetachShader(pointer, sps[a]);
			glDeleteShader(sps[a]);
		}
		pointers.push_back(pointer);
	}

	pointer = pointers[0];
}

void _Shader::SetOptions(const std::initializer_list<std::string>& nms) {
	for (auto& v : options) {
		v.second = std::find(nms.begin(), nms.end(), v.first) != nms.end();
	}
	UpdatePointer();
}

void _Shader::SetOption(const std::string& nm, bool on) {
	for (auto& v : options) {
		if (v.first == nm) {
			v.second = on;
			break;
		}
	}
	UpdatePointer();
}

void _Shader::AddUniform(const std::string& s, ShaderVariableType t) {
	for (auto& v : variables) {
		if (v.name() == s) return;
	}
	std::vector<GLint> locs = {};
	for (auto& p : pointers) {
		locs.push_back(glGetUniformLocation(p, s.c_str()));
	}
	variables.push_back(ShaderVariable(s, t, locs));
}

void _Shader::AddUniforms(std::initializer_list<const std::string> ss) {
	for (auto& s : ss) {
		AddUniform(s, ShaderVariableType::Unknown);
	}
}

void _Shader::RegisterStandardUniforms() {
	AddUniform("_MV", ShaderVariableType::Matrix);
	AddUniform("_P", ShaderVariableType::Matrix);
	AddUniform("_MVP", ShaderVariableType::Matrix);
}

void _Shader::Bind() const {
	glUseProgram(pointer);
}

void _Shader::Unbind() const {
	glUseProgram(0);
}

GLint _Shader::Loc(int i) const {
	return variables[i]._location;
}

CE_END_NAMESPACE