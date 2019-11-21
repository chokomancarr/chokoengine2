#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

ESerializedComponent::ESerializedComponent(const Component& c) : type(c->componentType) {
#define CS(tp) case ComponentType::tp: Set ## tp((tp)c); break;
	switch (type) {
		CS(Camera)
		CS(Animator)
		CS(Light)
		CS(MeshRenderer)
		CS(Rig)
	default:
		CE_NOT_IMPLEMENTED
	}
}

CE_END_ED_NAMESPACE