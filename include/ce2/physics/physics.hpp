#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Physics {	CE_CLASS_COMMON
	static CE_EXPORT float _timeScale;

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

	CE_GET_SET_ST_MEMBER_F(timeScale);

	/* Initializes the physics engine
	 * ChokoLait should call this for you
	 */
	static bool Init();

	static void Update(Scene&);
	
	static RayHit Raycast(const Ray&);

	friend class _Rigidbody;
	friend class _Collider;
};

CE_END_NAMESPACE
