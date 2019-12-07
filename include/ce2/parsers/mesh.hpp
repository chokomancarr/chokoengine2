#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

namespace MeshLoader {
	Mesh LoadObj(const std::string& path);
	Mesh LoadMesh(const std::string& path);
}

CE_END_NAMESPACE