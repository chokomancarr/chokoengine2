#include "chokoengine.hpp"
#include "ext/glmext.hpp"

CE_BEGIN_NAMESPACE

Mat3x3::Mat3x3(float f) : data({}) {
	data[0] = data[4] = data[8] = f;
}

Mat3x3::Mat3x3(float a, float b, float c,
		float d, float e, float f,
		float g, float h, float i)
	: data({ a, b, c, d, e, f, g, h, i }) {}

float& Mat3x3::operator[](const int i) {
	return data[i];
}

Vec3 Mat3x3::operator*(const Vec3& rhs) const {
	return glm_cast((*(glm::mat3*)this) * (*(glm::vec4*)&rhs));
}

Mat3x3 Mat3x3::operator*(const Mat3x3& rhs) const {
	return glm_cast((*(glm::mat3*)this) * (*(glm::mat3*)&rhs));
}

Mat3x3& Mat3x3::operator*=(const Mat3x3& rhs) {
	return (*this = *this * rhs);
}

Mat3x3 Mat3x3::inverse() const {
	return glm_cast(glm::inverse(*(glm::mat3*)this));
}

Mat3x3 Mat3x3::Identity() {
    return Mat3x3(1);
}

CE_END_NAMESPACE