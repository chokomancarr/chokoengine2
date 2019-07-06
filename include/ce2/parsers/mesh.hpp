#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class MeshLoader {
public:
	static Mesh LoadObj(const std::string& path);
	static Mesh LoadMesh(const std::string& path);
};

CE_END_NAMESPACE