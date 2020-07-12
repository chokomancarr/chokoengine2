#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class _AnimGraph::Link {
public:
	class Cond {
	public:
		int paramId;
		_Var value;
		enum class Comp {
			Equals,
			NEquals,
			Greater,
			Less,
			GEquals,
			LEquals,

			_COUNT
		} comp;
	};

	int target;

	float length;
	float offset;
	bool useExitLength;
	float exitLength;
	std::vector<std::vector<Cond>> conditions;
};

CE_END_NAMESPACE