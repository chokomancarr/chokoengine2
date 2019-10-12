#pragma once
#include "visual_shader.hpp"
#include "ce2/parsers/json.hpp"

CE_BEGIN_ED_NAMESPACE

class _VShaderNode;
typedef std::shared_ptr<_VShaderNode> VShaderNode;
typedef std::weak_ptr<_VShaderNode> pVShaderNode;

class _VShaderNode {
public:
	template <typename T>
	class Conn {
	public:
		std::string name;
		enum class Type {
			Scalar,
			Vector,
			Color
		} type;

		T target;
		float value_f;

		Conn(const std::string& nm, const Type tp) : name(nm), type(tp), value_f(0) {}
	};

	typedef Conn<pVShaderNode> ConnI;
	typedef Conn<std::vector<pVShaderNode>> ConnO;

	_VShaderNode(const std::string& name) : name(name), inputs({}), outputs({}) {}
	virtual ~_VShaderNode() = default;

	std::string name;
	std::vector<ConnI> inputs;
	std::vector<ConnO> outputs;

	Vec2 pos;

	static VShaderNode FromSig(const std::string& nm);

	void Parse(const JsonObject& data);
	virtual void ParseOptions(const JsonObject& data) = 0;
};


CE_END_ED_NAMESPACE

#include "vshader/inputs/color.hpp"

#include "vshader/outputs/surface.hpp"