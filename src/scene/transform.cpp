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
        _worldMatrix = _localMatrix * pm;
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
    UpdateLocalMatrix();
}

void Transform::worldRotation(const Quat& q) {
    CE_NOT_IMPLEMENTED;
}

void Transform::localScale(const Vec3& s) {
    _localScale = s;
    UpdateLocalMatrix();
}

CE_END_NAMESPACE