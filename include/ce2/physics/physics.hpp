#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Physics { CE_CLASS_COMMON
	
	static bool Init();

	static void Update();

	/* Call this on:
	 * - rigidbody added / removed
	 * - collider added / type changed
	 */
	static void OnBodyAdded(const SceneObject&);
	/* Call this on parameter change
	 */
	static void OnBodyChanged(const SceneObject&);
	/* Call this on object delete
	 */
	static void OnBodyRemoved(const SceneObject&);
public:
	class Ray {
	public:
		Vec3 origin;
		Vec3 direction;
		float length;
	};

	class RayHit {
	public:
		Vec3 point;
		float distance;
		Vec3 normal;
		Collider collider;

		operator bool() const {
			return !!collider;
		}
	};
	
	static RayHit Raycast(const Ray&);

	friend class _Rigidbody;
	friend class _Collider;
};

CE_END_NAMESPACE
