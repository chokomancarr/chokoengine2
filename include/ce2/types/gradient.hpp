#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Gradient { CE_CLASS_COMMON
	Texture _lookuptex;

public:
	class Entry {
	public:
		Entry() = default;
		Entry(const float pos, const Color& val)
			: position(pos), value(val) {}

		float position;
		Color value;
	};

	Gradient(const std::vector<Entry>& = {});
	/* constructs a gradient with colors evenly spaced
	 */
	Gradient(const std::vector<Color>&);

	std::vector<Entry> entries;

	/* If this object is used in GPU calculations,
	 * this must be called to update GPU memory
	 */
	void Apply();

	Color Eval(const float t);

	Texture& GetLookupTexture();
	const Texture& GetLookupTexture() const;

	static Gradient Rainbow(int reso = 10);
};

CE_END_NAMESPACE