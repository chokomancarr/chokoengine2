#pragma once
#include "backend/chokoengine_backend.hpp"

CE_BEGIN_BK_NAMESPACE

class Renderer::Particles {
	struct Data {
		struct Buffer0 {
			float hp; //life starts at max, <=0 is dead
			float maxhp; //0 maxhp is invalid
			float rotation;
			float dummy;
		};
		struct Buffer1 {
			Vec3 pos;
			float scale;
		};
		struct Buffer2 {
			Vec3 vel;
			float avel;
		};
	};

	typedef std::array<std::vector<Vec4>, 3> DataBuf;

	static TransformFeedback _simProg, _outProg;
public:

private:

	static void InitProgs();

	static void InitBuffers(_ParticleSystem&);

	static void CleanDeadPar(_ParticleSystem&);

	static void ExecTimestep(_ParticleSystem&);

public:
	static void UpdateData(_ParticleSystem&);

	static void Render(const ParticleSystem&, const Mat4x4& p, const Mat4x4& ip);
};

CE_END_BK_NAMESPACE
