#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

namespace {
	Mat4x4 getParentMat(_SceneObject* o) {
		const auto& pr = o->parent();
		return !pr ? Mat4x4::Identity() : pr->transform()->worldMatrix();
	}

	Quat getParentRot(_SceneObject* o) {
		const auto& pr = o->parent();
		return !pr ? Quat::identity() : pr->transform()->worldRotation();
	}

	template <typename T>
	inline Vec3 _xyz(const T& v) {
		return *(Vec3*)&v;
	}
}

#define L2WROT() \
	_worldRotation = getParentRot(_object) * _localRotation;\
	_worldRotationEuler = Quat::ToEuler(_worldRotation) * Math::rad2deg

#define W2LROT() \
	_localRotation = getParentRot(_object).inverse() * _worldRotation;\
	_localRotationEuler = Quat::ToEuler(_localRotation) * Math::rad2deg

Transform::Transform() :
    _localPosition(Vec3()), _worldPosition(Vec3()),
    _localRotation(Quat::identity()), _worldRotation(Quat::identity()),
    _localScale(Vec3(1)), _localMatrix(Mat4x4::Identity()),
    _worldMatrix(Mat4x4::Identity())
{}

void Transform::UpdateLocalMatrix() {
    _localMatrix = Mat4x4::FromTRS(_localPosition, _localRotation, _localScale);
    UpdateParentMatrix();
}

void Transform::UpdateParentMatrix() {
    if (!_object->parent()) {
        _worldMatrix = _localMatrix;
        _worldPosition = _localPosition;
    }
    else {
        const auto& pm = _object->parent()->transform()->_worldMatrix;
        _worldMatrix = pm * _localMatrix;
        Vec4 wpos = pm * Vec4(_localPosition, 1);
        _worldPosition = Vec3(wpos.x, wpos.y, wpos.z) / wpos.w;
    }

    for (auto& c : _object->children()) {
        c->transform()->UpdateParentMatrix();
    }
}

void Transform::localPosition(const Vec3& v) {
    _localPosition = v;
    UpdateLocalMatrix();
}

void Transform::worldPosition(const Vec3& v) {
	_worldPosition = v;
	_localPosition = _xyz(getParentMat(_object).inverse() * Vec4(_worldPosition, 1));
	UpdateLocalMatrix();
}

void Transform::localRotation(const Quat& q) {
    _localRotation = q;
    _localRotationEuler = Quat::ToEuler(q) * Math::rad2deg;
	L2WROT();
    UpdateLocalMatrix();
}

void Transform::worldRotation(const Quat& q) {
    _worldRotation = q;
	_worldRotationEuler = Quat::ToEuler(q) * Math::rad2deg;
	W2LROT();
	UpdateLocalMatrix();
}

void Transform::localRotationEuler(const Vec3& v) {
    _localRotationEuler = v;
    _localRotation = Quat::FromEuler(v);
	L2WROT();
    UpdateLocalMatrix();
}

void Transform::worldRotationEuler(const Vec3& v) {
	_worldRotationEuler = v;
	_worldRotation = Quat::FromEuler(v);
	W2LROT();
}

void Transform::Rotate(const Vec3& v, TransformSpace sp) {
	_localRotation = _localRotation * Quat::FromEuler(v * Math::deg2rad);
	_localRotationEuler = Quat::ToEuler(_localRotation) * Math::rad2deg;
	L2WROT();
	UpdateLocalMatrix();
}

void Transform::localScale(const Vec3& s) {
    _localScale = s;
    UpdateLocalMatrix();
}

Vec3 Transform::forward() const {
	return *(Vec3*)glm::value_ptr(_worldMatrix * Vec4(0, 0, 1, 0));
}

Vec3 Transform::right() const {
	return *(Vec3*)glm::value_ptr(_worldMatrix * Vec4(1, 0, 0, 0));
}

Vec3 Transform::up() const {
	return *(Vec3*)glm::value_ptr(_worldMatrix * Vec4(0, 1, 0, 0));
}

CE_END_NAMESPACE