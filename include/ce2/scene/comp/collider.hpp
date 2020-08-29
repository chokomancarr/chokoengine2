#pragma once
#include "chokoengine.hpp"
#include "collider/enums/collider_type.hpp"

CE_BEGIN_NAMESPACE

class _Collider : public _Component { CE_COMPONENT_COMMON

	ColliderType _type;

	float _radius;

	float _length;

	Vec3 _size;

	Mesh _mesh;

	bool _convex;

public:
	_Collider();

	_Collider(const _Collider& rhs);

	void OnStart() override;

	CE_GET_SET_MEMBER_F(type);

	CE_GET_SET_MEMBER(radius);
	CE_GET_SET_MEMBER(length);
	CE_GET_SET_MEMBER(size);
	CE_GET_SET_MEMBER(mesh);
	CE_GET_SET_MEMBER(convex);

private:
	Mesh _mesh_convex;
};

CE_END_NAMESPACE