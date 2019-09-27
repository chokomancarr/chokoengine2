#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

/* The Transform class contains the transformation
 * data and methods of a SceneObject.
 * A Transform object cannot be created directly.
 * Use `SceneObject::New()->transform()` instead
 */
class Transform { CE_CLASS_COMMON
    Transform();
    Transform& operator =(const Transform&) = default;

    _SceneObject* _object;

    Vec3 _localPosition;
    Vec3 _worldPosition;
    Quat _localRotation;
    Vec3 _localRotationEuler;
    Quat _worldRotation;
    Vec3 _worldRotationEuler;
    Vec3 _localScale;

    Mat4x4 _localMatrix;
    Mat4x4 _worldMatrix;

    void UpdateLocalMatrix();
    void UpdateParentMatrix();
public:
    CE_GET_MEMBER(localPosition);
    CE_SET_MEMBER_F(localPosition);
    CE_GET_MEMBER(worldPosition);
    CE_SET_MEMBER_F(worldPosition);

    CE_GET_MEMBER(localRotation);
    CE_SET_MEMBER_F(localRotation);
    CE_GET_MEMBER(worldRotation);
    CE_SET_MEMBER_F(worldRotation);
    
    CE_GET_MEMBER(localRotationEuler);
    CE_SET_MEMBER_F(localRotationEuler);
    CE_GET_MEMBER(worldRotationEuler);
    CE_SET_MEMBER_F(worldRotationEuler);

    CE_GET_MEMBER(localScale);
    CE_SET_MEMBER_F(localScale);

    CE_GET_MEMBER(localMatrix);
    CE_GET_MEMBER(worldMatrix);

	Vec3 forward() const;
	void forward(const Vec3&);

	Vec3 right() const;
	void right(const Vec3&);

	Vec3 up() const;
	void up(const Vec3&);

    friend class _SceneObject;
};

CE_END_NAMESPACE