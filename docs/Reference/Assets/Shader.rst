Shader
==================================

Definition
----------

.. class:: _Shader : public _Asset

.. type:: Ref<_Shader> Shader
    Ref_w<_Shader> pShader

    desc

Constructors
------------

.. function:: Shader Shader::New()

    desc

.. function:: Shader Shader::New(const std::string& vert, const std::string& frag)

    desc

.. function:: Shader Shader::New(const std::vector<std::string>& strs, const std::vector<ShaderType>& typs)

    desc

Variable functions
------------------

.. function:: const ShaderQueue& _Shader::queue() const
    void _Shader::queue(const ShaderQueue&)

    desc

.. function:: const ShaderZTest& _Shader::ztest() const
    void _Shader::ztest(const ShaderZTest&)

    desc

.. function:: const ShaderBlend& _Shader::blendSrc() const
    void _Shader::blendSrc(const ShaderBlend&)
    const ShaderBlend& _Shader::blendDst() const
    void _Shader::blendDst(const ShaderBlend&)

    desc

Method functions
----------------

.. function:: void _Shader::SetOptions(const std::initializer_list<std::string>& nms)

    desc

.. function:: void _Shader::SetOptions(const std::string& nm, bool on)

    desc

.. function:: void _Shader::AddUniforms(const std::string& s, ShaderVariableType t)

    desc

.. function:: void _Shader::AddUniforms(std::initializer_list<const std::string> ss)

    desc

.. function:: void _Shader::RegisterStandardUniforms()

    desc

.. function:: void _Shader::Bind()

    Binds the shader for rendering.

.. function:: void _Shader::Unbind() const

    Unbinds the shader.

.. function:: GLint _Shader::Loc(int i) const

    Returns the OpenGL uniform location of the variable at index :expr:`i`.
    Example: :expr:`glUniform1i(shader->Loc(0), 1)`.


Shader Type
=================

Definition
----------

.. enum-class:: ShaderType

.. enumerator:: ShaderType::Vertex

.. enumerator:: ShaderType::Geometry

.. enumerator:: ShaderType::TesselCtrl

.. enumerator:: ShaderType::TesselEval

.. enumerator:: ShaderType::Fragment

Shader Queue
=================

Definition
----------

.. enum-class:: ShaderQueue

.. enumerator:: ShaderQueue::Opaque

.. enumerator:: ShaderQueue::Transparent

Shader Z Test
=================

Definition
----------

.. enum-class:: ShaderZTest

Shader Blend
=================

Definition
----------

.. enum-class:: ShaderBlend