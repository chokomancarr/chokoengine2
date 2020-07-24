#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

namespace {
	std::vector<Gradient::Entry> _cols2entries(const std::vector<Color>& cols) {
		const auto n = cols.size();
		if (!n) return {};
		else if (n == 1) {
			return { Gradient::Entry(0, cols[0]) };
		}
		else {
			std::vector<Gradient::Entry> res(n);
			for (size_t a = 0; a < n; a++) {
				res[a].position = a / (n - 1.f);
				res[a].value = cols[a];
			}
			return res;
		}
	}
}

Gradient::Gradient(const std::vector<Entry>& es)
	: entries(es) {}

Gradient::Gradient(const std::vector<Color>& cols)
	: Gradient(_cols2entries(cols)) {}

void Gradient::Apply() {
	if (!_lookuptex) {
		_lookuptex = Texture::New(256, 1, true, TextureOptions());
	}
	std::vector<float> vals(1024);
	for (int a = 0; a < 256; a++) {
		const auto res = Eval(a / 255.f);
		vals[a * 4] = res.r;
		vals[a * 4 + 1] = res.g;
		vals[a * 4 + 2] = res.b;
		vals[a * 4 + 3] = res.a;
	}
	_lookuptex->SetPixelsRaw(vals);
}

Color Gradient::Eval(const float t) {
	if (entries[0].position >= t) {
		return entries[0].value;
	}
	for (auto& k : entries) {
		if (k.position > t) {
			const auto& kb = (&k)[-1];
			return Math::Lerp(kb.value, k.value, Math::ILerp(kb.position, k.position, t));
		}
	}
	return entries.back().value;
}

Texture& Gradient::GetLookupTexture() {
	if (!_lookuptex) {
		Apply();
	}
	return _lookuptex;
}
const Texture& Gradient::GetLookupTexture() const {
	return _lookuptex;
}

Gradient Gradient::Rainbow(int reso) {
	std::vector<Color> res(reso);
	for (int a = 0; a < reso; a++) {
		res[a] = Color::FromHSV(a / (reso - 1.f), 1, 1);
	}
	return Gradient(res);
}

CE_END_NAMESPACE