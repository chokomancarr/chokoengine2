#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

namespace MeshLoader {
	Mesh LoadObj(const std::string& data);
	Mesh LoadMesh(const std::string& path);
}

CE_END_NAMESPACE