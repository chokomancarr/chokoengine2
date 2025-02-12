#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

namespace {
	float RadicalInverse(uint bits) {
		bits = (bits << 16u) | (bits >> 16u);
		bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
		bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
		bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
		bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
		return float(bits) * 2.3283064365386963e-10; // / 0x100000000
	}
}

float Random::Value() {
	return ((float)rand()) / ((float)RAND_MAX);
}

float Random::Range(float min, float max) {
	return min + Random::Value()*(max - min);
}

std::vector<Vec2> Random::Hammersley(uint n) {
    std::vector<Vec2> result(n);
    for (uint a = 0; a < n; a++) {
        result[a] = Vec2(float(a)/n, RadicalInverse(a));
    }
    return result;
}

 CE_END_NAMESPACE