#pragma once
#include "chokolait.hpp"
#include "chokoballs.hpp"

namespace CB = ChokoBalls;

CE_BEGIN_NAMESPACE

namespace {
	bool initd = false;
	CB::World world;

	constexpr float updaterate = 1.f / 24;

	float timetoupdate = 0;

	size_t bodycount = 0;
	std::vector<SceneObject> bodies = {}, bodies_old = {};

	std::vector<CB::Object> cb_objects = {};

#define _set(slot, val) slot(*(std::remove_reference<decltype(slot())>::type*)&(val))
	void b2w() {
		for (size_t a = 0; a < bodycount; a++) {
			auto& src = world->objects[a];
			auto& tar = bodies_old[a];
			auto tr = tar->transform();
			_set(tr->worldPosition, src->position);
			_set(tr->worldRotation, src->rotation);

			if (src->has_rigidbody) {
				auto& rig = tar->GetComponent<Rigidbody>();
				_set(rig->velocity, src->rigidbody.velocity);
				_set(rig->angularVelocity, src->rigidbody.rotVelocity);
				_set(rig->acceleration, src->rigidbody.accel);
				_set(rig->torque, src->rigidbody.torque);
			}
		}
	}
#undef _set
}

bool Physics::Init() {
	auto ret = CB::ChokoBalls::Init();
	if (ret != CB::CB_STATUS::OK) return false;

	world = CB::World::New();

	initd = true;
	return true;
}

void Physics::Update() {
	if (!initd) return;
	if ((timetoupdate -= updaterate) < 0) {
		timetoupdate += updaterate;

		//apply the simulated result
		world->FinishUpdate();
		b2w();

		ChokoLait::scene->PhysicsUpdate();

		for (size_t a = bodycount; a > 0; a--) {
			if (!bodies[a+1]) {
				std::swap(bodies[a+1], bodies.back());
				bodies.pop_back();
				std::swap(cb_objects[a + 1], cb_objects.back());
				cb_objects.pop_back();

				bodycount--;
			}
		}

		//copy parameters for next simulation
		bodies_old = bodies;
		world->objects = cb_objects;
		world->BeginUpdate();
	}
}

void Physics::OnBodyAdded(const SceneObject& o) {
	if (!initd) return;
	OnBodyRemoved(o);

	bodies.push_back(o);
	CB::Object obj = CB::Object::New();
	cb_objects.push_back(obj);

	bodycount++;

	auto rig = o->GetComponent<Rigidbody>();
	auto col = o->GetComponent<Collider>();

	if ((obj->has_rigidbody = !!rig)) {
		obj->has_rigidbody = true;
	}
	if (!!col) {
		switch (col->type()) {
		case ColliderType::InfPlane:
			obj->collider = CB::Collider_InfPlane::New();
		case ColliderType::Sphere:
			obj->collider = CB::Collider_Sphere::New(col->radius());
			break;
		default:
			obj->collider = nullptr;
			break;
		}
	}

	OnBodyChanged(o);
}
void Physics::OnBodyChanged(const SceneObject& o) {
	if (!initd) return;
	const auto& it = std::find(bodies.begin(), bodies.end(), o);
	auto& obj = cb_objects[it - bodies.begin()];

	if (obj->has_rigidbody) {
#define _set(src, tar) obj->rigidbody.tar = *(decltype(obj->rigidbody.tar)*)&(rig->src())
		auto& rig = (*it)->GetComponent<Rigidbody>();
		_set(velocity, velocity);
		_set(angularVelocity, rotVelocity);
		_set(acceleration, accel);
		_set(torque, torque);
#undef _set
	}
	if (!!obj->collider) {
#define _set(src, tar) ctr->tar = *(decltype(ctr->tar)*)&(col->src())
		auto& col = (*it)->GetComponent<Collider>();
		switch (col->type()) {
		case ColliderType::InfPlane: {
			//auto& ctr = (CB::Collider_InfPlane)obj->collider;
			break;
		}
		case ColliderType::Sphere: {
			auto& ctr = (CB::Collider_Sphere)obj->collider;
			_set(radius, radius);
			break;
		}
		default:
			break;
		}
#undef _set
	}
}
void Physics::OnBodyRemoved(const SceneObject& o) {
	if (!initd) return;
	const auto& it = std::find(bodies.begin(), bodies.end(), o);
	if (it != bodies.end()) {
		std::swap(*it, bodies.back());
		bodies.pop_back();
		std::swap(cb_objects[it - bodies.begin()], cb_objects.back());
		cb_objects.pop_back();

		bodycount--;
	}
}

Physics::RayHit Physics::Raycast(const Ray& ray) {
	return {};
}

CE_END_NAMESPACE
