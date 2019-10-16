#include "backend/chokoengine_backend.hpp"

CE_BEGIN_BK_NAMESPACE

bool GI::Init() {
	return Voxelizer::InitShaders();
}

CE_END_BK_NAMESPACE