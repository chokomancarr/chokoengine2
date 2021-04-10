#include "chokolait.hpp"
#include "chokoballs.hpp"

namespace CB = ChokoBalls;

CE_BEGIN_NAMESPACE

namespace {
	bool initd = false;
	bool ignore = false;
	CB::World world;
	Scene _scene = nullptr;

	constexpr float updaterate = 1.f / 24;

	float timetoupdate = 0;

	size_t bodycount = 0;
	std::vector<SceneObject> bodies = {}, bodies_old = {};

	std::vector<CB::Object> cb_objects = {};

#define _set(slot, val) slot(*(std::remove_reference<decltype(slot())>::type*)&(val))
	void b2w() {
		ignore = true;
		for (size_t a = 0; a < bodies_old.size(); a++) {
			auto& src = world->objects[a];
			auto& tar = bodies_old[a];

			if (src->has_rigidbody) {
				if (src->dynamic) {
					auto tr = tar->transform();
					_set(tr->worldPosition, src->position);
					_set(tr->worldRotation, src->rotation);
				}

				auto rig = tar->GetComponent<Rigidbody>();
				_set(rig->velocity, src->rigidbody.velocity);
				_set(rig->angularVelocity, src->rigidbody.rotVelocity);
				_set(rig->acceleration, src->rigidbody.accel);
				_set(rig->torque, src->rigidbody.torque);
			}
		}
		ignore = false;
	}
#undef _set
}

float Physics::_timeScale = 1;

void Physics::timeScale(const float& t) {
	world->timeScale = _timeScale = t;
}

bool Physics::Init() {
	auto ret = CB::ChokoBalls::Init();
	if (ret != CB::CB_STATUS::OK) return false;

	world = CB::World::New();

	world->deltaTime = updaterate;

	initd = true;
	return true;
}

void Physics::Update(Scene& scene) {
	if (!initd) return;
	if (!_scene) _scene = scene;
	else if (_scene != scene) return;
	if ((timetoupdate -= Time::delta()) < 0) {
		timetoupdate += updaterate;

		//apply the simulated result
		world->FinishUpdate();
		b2w();

		scene->PhysicsUpdate();

		for (size_t a = bodycount; a > 0; a--) {
			auto& body = bodies[a - 1];
			auto& cbo = cb_objects[a - 1];
			if (!body) {
				std::swap(body, bodies.back());
				bodies.pop_back();
				std::swap(cbo, cb_objects.back());
				cb_objects.pop_back();

				bodycount--;
			}
			else if (!cbo->has_rigidbody || !cbo->dynamic) {
#define _set(src, tar) cbo->tar = *(decltype(cbo->tar)*)&(body->transform()->src())
				_set(worldPosition, position);
				_set(worldRotation, rotation);
#undef _set
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
			break;
		case ColliderType::Sphere:
			obj->collider = CB::Collider_Sphere::New(col->radius());
			break;
		default:
			obj->collider = nullptr;
			break;
		}
	}

#define _set(src, tar) obj->tar = *(decltype(obj->tar)*)&(o->transform()->src())
	_set(worldPosition, position);
	_set(worldRotation, rotation);
#undef _set

	OnBodyChanged(o);
}
void Physics::OnBodyChanged(const SceneObject& o) {
	if (!initd || ignore) return;
	const auto& it = std::find(bodies.begin(), bodies.end(), o);
	auto& obj = cb_objects[it - bodies.begin()];

#define _set(src, tar) obj->tar = *(decltype(obj->tar)*)&(o->transform()->src())
	auto rig = (*it)->GetComponent<Rigidbody>();
	if (!obj->has_rigidbody || !rig->dynamic()) {
		_set(worldPosition, position);
		_set(worldRotation, rotation);
	}
#undef _set
	if (obj->has_rigidbody) {
#define _set(src, tar) obj->rigidbody.tar = *(decltype(obj->rigidbody.tar)*)&(rig->src())
		obj->dynamic = rig->dynamic();
		obj->rigidbody.inverseMass = 1.f / rig->mass();
		obj->rigidbody.inverseMoment = 1.f / rig->moment();
		obj->rigidbody.bounce = rig->bounce();
		_set(velocity, velocity);
		_set(angularVelocity, rotVelocity);
		_set(acceleration, accel);
		_set(torque, torque);
#undef _set
	}
	if (!!obj->collider) {
#define _set(src, tar) ctr->tar = *(decltype(ctr->tar)*)&(col->src())
		auto col = (*it)->GetComponent<Collider>();
		switch (col->type()) {
		case ColliderType::InfPlane: {
			//auto& ctr = (CB::Collider_InfPlane)obj->collider;
			break;
		}
		case ColliderType::Sphere: {
			auto ctr = (CB::Collider_Sphere)obj->collider;
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
