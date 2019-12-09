Material
==============

Definition
----------

.. class:: _Material : public _Asset
.. type:: Ref<_Material> Material
    Ref_w<_Material> pMaterial

    A material object that saves a shader configuration.

Constructors
------------

.. function:: Material Material::New()

    Creates a new material.

Variable functions
------------------

.. function:: const Shader& _Material::shader() const
    void _Material::shader(const Shader&)

    The shader target. Setting this variable clears all assigned variables.

.. function:: std::vector<ShaderVariable>& _Material::variables()
    void _Material::variables(const std::vector<ShaderVariable>&)

    Rendering variables.

Method functions
----------------

.. function:: const float& _Material::GetUniform(const std::string& s, float f)
    int& _Material::GetUniform(const std::string& s, int i)
    Vec2& _Material::GetUniform(const std::string& s, Vec2 v)
    Vec3& _Material::GetUniform(const std::string& s, Vec3 v)
    Vec4& _Material::GetUniform(const std::string& s, Vec4 v)
    Mat4x4& _Material::GetUniform(const std::string& s, Mat4x4 m)
    Color& _Material::GetUniform(const std::string& s, Color c)
    Texture& _Material::GetUniform(const std::string& s, Texture t)

    Gets the value assigned to the shader uniform variable :expr:`s`.

.. function:: void _Material::SetUniform(const std::string& s, float f)
    void _Material::SetUniform(const std::string& s, int i)
    void _Material::SetUniform(const std::string& s, Vec2 v)
    void _Material::SetUniform(const std::string& s, Vec3 v)
    void _Material::SetUniform(const std::string& s, Vec4 v)
    void _Material::SetUniform(const std::string& s, Mat4x4 m)
    void _Material::SetUniform(const std::string& s, Color c)
    void _Material::SetUniform(const std::string& s, Texture t)
    void _Material::SetUniform(const std::string& s, CubeMap c)

    Assigns the value to the shader unifor mvariable :expr:`s`