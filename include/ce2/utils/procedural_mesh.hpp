#pragma once
#include "chokoengine.hpp"
#include "enums/subdiv_scheme.hpp"

CE_BEGIN_NAMESPACE

class ProceduralMesh { CE_CLASS_COMMON

public:
    static Mesh Plane(int x, int z, float sz);

    static Mesh UVSphere(int nr, int ny, float r);

    static Mesh Subdivide(const Mesh& m, SubdivScheme scheme);
};

CE_END_NAMESPACE