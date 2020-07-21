#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _AnimGraph::Node {
public:
	enum class Type {
		Single,
		Linear1D,
		//Linear2D,

		_COUNT
	} type;

private:

	AnimClip _clip;

	std::vector<std::pair<float, AnimClip>> _clips1D;

public:
	Node();

	std::string name;

	CE_GET_SET_MEMBER(clip);

	CE_GET_SET_MEMBER_MUT(clips1D);

	float length() const;

	float speed;
	bool repeat;

	int var1D;
	//std::array<int, 2> var2D;

	std::vector<Link> links;

	_AnimClip::VQ Get(const State& st, const std::string& sig, float t) const;
};

CE_END_NAMESPACE