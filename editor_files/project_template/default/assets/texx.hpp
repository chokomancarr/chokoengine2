#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

CE_SCRIPT_DEF(Texx) {
public:
	CE_SERIALIZE Texture tx;

	void OnUpdate() override {
		if (!!tx) {
			_object->scene()->objects()[0]->children()[0]
				->GetComponent<MeshRenderer>()->materials()[0]->SetUniform("tex", tx);
			tx = nullptr;
		}
	}
};

CE_END_NAMESPACE
