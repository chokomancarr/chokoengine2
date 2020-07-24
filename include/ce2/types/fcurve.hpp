#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class FCurve {
	Texture _lookuptex;

public:

	FCurve(const std::vector<Vec2>& = {});
	FCurve(const std::vector<float>&);

	std::vector<Vec2> entries;

	/* If this object is used in GPU calculations,
	 * this must be called to update GPU memory
	 */
	void Apply();

	float Eval(const float t);

	Texture& GetLookupTexture();
	const Texture& GetLookupTexture() const;

	static FCurve EaseInOut(int reso = 10);
};

CE_END_NAMESPACE