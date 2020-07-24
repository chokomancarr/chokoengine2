#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

namespace {
	std::vector<Vec2> _vls2entries(const std::vector<float>& vls) {
		const auto n = vls.size();
		if (!n) return {};
		else if (n == 1) {
			return { Vec2(0, vls[0]) };
		}
		else {
			std::vector<Vec2> res(n);
			for (size_t a = 0; a < n; a++) {
				res[a].x = a / (n - 1.f);
				res[a].y = vls[a];
			}
			return res;
		}
	}
}

FCurve::FCurve(const std::vector<Vec2>& es)
	: entries(es) {}

FCurve::FCurve(const std::vector<float>& vls)
	: FCurve(_vls2entries(vls)) {}

void FCurve::Apply() {
	if (!_lookuptex) {
		_lookuptex = Texture::New(256, 1, true, TextureOptions());
	}
	std::vector<float> vals(1024);
	for (int a = 0; a < 256; a++) {
		const auto res = Eval(a / 255.f);
		vals[a * 4] =
			vals[a * 4 + 1] =
			vals[a * 4 + 2] =
			vals[a * 4 + 3] = res;
	}
	_lookuptex->SetPixelsRaw(vals);
}

float FCurve::Eval(const float t) {
	if (entries[0].x >= t) {
		return entries[0].y;
	}
	for (auto& k : entries) {
		if (k.x > t) {
			const auto& kb = (&k)[-1];
			return Math::Lerp(kb.y, k.y, Math::ILerp(kb.x, k.x, t));
		}
	}
	return entries.back().y;
}

Texture& FCurve::GetLookupTexture() {
	if (!_lookuptex) {
		Apply();
	}
	return _lookuptex;
}
const Texture& FCurve::GetLookupTexture() const {
	return _lookuptex;
}

/*
Gradient FCurve::EaseInOut(int reso) {
	std::vector<Color> res(reso);
	for (int a = 0; a < reso; a++) {
		res[a] = Color::FromHSV(a / (reso - 1.f), 1, 1);
	}
	return Gradient(res);
}
*/

CE_END_NAMESPACE