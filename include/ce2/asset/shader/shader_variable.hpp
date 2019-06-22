#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class ShaderVariable {
    ShaderVariable(const std::string&, ShaderVariableType, GLuint); //only allow shader to construct

    std::string _name;
    ShaderVariableType _type;
    GLint _location;

public:
    CE_GET_MEMBER(name);
    CE_GET_MEMBER(type);

    friend class Shader;
};

CE_END_NAMESPACE