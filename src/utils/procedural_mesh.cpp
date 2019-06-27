#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

Mesh ProceduralMesh::UVSphere(int nr, int ny, float r) {
    Mesh m = Mesh::New();

    const int n = nr * ny;
    const int nt = (ny-1) * nr * 2;

    std::vector<Vec3> poss, nrms, tgts;
    std::vector<Vec2> uvs;
    std::vector<Int3> tris;

    poss.reserve(n);
    nrms.reserve(n);
    tgts.reserve(n);
    uvs.reserve(n);
    tris.reserve(nt);

    const float dx = 1.f / (nr - 1);
    const float dy = 1.f / (ny - 1);
    for (int x = 0; x < nr; x++) {
        const float cx = std::cos(x * 2 * 3.14159f / nr);
        const float sx = std::sin(x * 2 * 3.14159f / nr);
        for (int y = 0; y < ny; y++) {
            const float sy = std::sin(((y * 1.f / (ny - 1)) - 0.5f) * 3.14159f);
            const float cy = std::sqrt(1 - sy * sy);
            const Vec3 v = Vec3(cx * cy, sy, sx * cy);
            poss.push_back(v * r);
            nrms.push_back(v);
            tgts.push_back(Vec3(-sx, 0, cx));
            uvs.push_back(Vec2(x * dx, y * dy));
        }
    }

    for (int x = 0; x < nr; x++) {
        for (int y = 0; y < ny - 1; y++) {
            const int i = x * ny + y;
            if (x < nr - 1) {
                tris.push_back(Int3(i, i + 1, i + ny));
                tris.push_back(Int3(i + 1, i + ny + 1, i + ny));
            }
            else {
                tris.push_back(Int3(i, i + 1, y));
                tris.push_back(Int3(i + 1, y + 1, y));
            }
        }
    }

    m->positions(poss);
    m->normals(nrms);
    m->tangents(tgts);
    m->texcoords(uvs);
    m->triangles(tris);
    m->matTriangles(std::vector<std::vector<Int3>>(1, tris));
    m->Apply();
    return m;
}

CE_END_NAMESPACE