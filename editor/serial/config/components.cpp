#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

#define CE_E_REGSERIAL(TP, C, V) {\
	auto e = std::make_shared<ESerializedEntry_ ## TP<_ ## C>>();\
	e->funcSignature = #V;\
	e->getter = static_cast<decltype(e->getter)>(&_ ## C::V);\
	e->setter = static_cast<decltype(e->setter)>(&_ ## C::V);\
	auto oid = _ ## C::_object_id();\
	ESerializer::AddEntry(#C, oid, e);\
}

void ES_Components::Register() {
	CE_E_REGSERIAL(F, Camera, fov);
	CE_E_REGSERIAL(F, Camera, orthoSize);
	CE_E_REGSERIAL(F, Camera, nearClip);
	CE_E_REGSERIAL(F, Camera, farClip);
}

CE_END_ED_NAMESPACE