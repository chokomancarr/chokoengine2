#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

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
    CE_NOT_IMPLEMENTED;
}

void Transform::localRotation(const Quat& q) {
    _localRotation = q;
    _localRotationEuler = Quat::ToEuler(q) * Math::rad2deg;
    UpdateLocalMatrix();
}

void Transform::worldRotation(const Quat& q) {
    CE_NOT_IMPLEMENTED;
}

void Transform::localRotationEuler(const Vec3& v) {
    _localRotationEuler = v;
    _localRotation = Quat::FromEuler(v);
    UpdateLocalMatrix();
}

void Transform::worldRotationEuler(const Vec3& q) {
    CE_NOT_IMPLEMENTED;
}

void Transform::Rotate(const Vec3& v, TransformSpace sp) {
	_localRotation = _localRotation * Quat::FromEuler(v * Math::deg2rad);
	_localRotationEuler = Quat::ToEuler(_localRotation) * Math::rad2deg;
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