#pragma once
#include "backend/chokoengine_backend.hpp"
#include "glsl/particlesys_tf.h"
#include "glsl/particleout_tf.h"
#include <iomanip>

#define NBUF 3

CE_BEGIN_BK_NAMESPACE

namespace {
	inline void Bind(GLint loc, const CRValue& v) {
		glUniform2f(loc, v.constant, v.random);
	}
	inline void Bind(GLint loc, const Vec3& v) {
		glUniform3f(loc, v.x, v.y, v.z);
	}
}

TransformFeedback Renderer::Particles::_simProg = nullptr;
TransformFeedback Renderer::Particles::_outProg = nullptr;

void Renderer::Particles::InitProgs() {
	(_simProg = TransformFeedback_New(glsl::particlesys_tf, { "outbuf0", "outbuf1", "outbuf2" }))
		->AddUniforms({ "numParticles", "randSeed", "DT", "emissionShape",
			"radius", "angle", "length",
			"pLifetime", "pRotation0", "pScale0", "pSpeed0", "pASpeed0",
			"pForce" });
	(_outProg = TransformFeedback_New(glsl::particleout_tf_v, glsl::particleout_tf_g,
		{ "out_pos", "out_texCoord", "out_color" }))
		->AddUniforms({ "camUp", "camRight", "camFwd", "pColorGrad" });
}

void Renderer::Particles::InitBuffers(_ParticleSystem& par) {
	par._data = { VertexArray_New(), VertexArray_New() };

	const std::vector<Vec4> buf(par._maxparticles);

	for (int i = 0; i < NBUF; i++) {
		par._data[0]->AddBuffer(VertexBuffer_New(true, 4, par._maxparticles, buf.data()));
		par._data[1]->AddBuffer(VertexBuffer_New(true, 4, par._maxparticles, buf.data()));
	}

	par._mesh = VertexArray_New();
	par._mesh->AddBuffer(VertexBuffer_New(true, 3, par._maxparticles * 6, nullptr));
	par._mesh->AddBuffer(VertexBuffer_New(true, 3, par._maxparticles * 6, nullptr)); //not used
	par._mesh->AddBuffer(VertexBuffer_New(true, 3, par._maxparticles * 6, nullptr)); //not used
	par._mesh->AddBuffer(VertexBuffer_New(true, 2, par._maxparticles * 6, nullptr));
	par._mesh->AddBuffer(VertexBuffer_New(true, 4, par._maxparticles * 6, nullptr));
}

void Renderer::Particles::CleanDeadPar(_ParticleSystem& par) {
	DataBuf data = {
		par._data[0]->buffer(0)->Get<Vec4>(),
		par._data[0]->buffer(1)->Get<Vec4>(),
		par._data[0]->buffer(2)->Get<Vec4>()
	};

	auto L0 = data[0].begin();
	auto L = L0;
	auto R = data[0].begin() + par._activenumpar - 1;

	while (R->x <= 0) {
		R->x = R->y = 0;
		R--;
		if (!(--par._activenumpar))
			break;
	}

	while (L < R) {
		if (L->x <= 0) {
			std::swap(*L, *R);
			std::swap(data[1][L - L0], data[1][R - L0]);
			std::swap(data[2][L - L0], data[2][R - L0]);

			while (R->x <= 0) {
				R->x = R->y = 0;
				R--;
				if (!(--par._activenumpar))
					break;
			}
		}
		L++;
	}

	for (int a = 0; a < NBUF; a++) {
		par._data[0]->buffer(a)->Set(data[a].data(), par._maxparticles);
	}
}

void Renderer::Particles::ExecTimestep(_ParticleSystem& par) {
	if (!_simProg) {
		InitProgs();
	}

	_simProg->vao(par._data[0]);
	_simProg->outputs(par._data[1]->buffers());
	_simProg->Bind();

	int i = 0;
#define LOC _simProg->Loc(i++)
	glUniform1i(LOC, par._activenumpar);
	glUniform1i(LOC, rand());
	glUniform1f(LOC, Time::delta());
	glUniform1i(LOC, (int)par._shape);
	glUniform1f(LOC, par._radius);
	glUniform1f(LOC, par._angle * Math::deg2rad);
	glUniform1f(LOC, par._length);
	Bind(LOC, par._lifetime);
	Bind(LOC, par._initialRotation);
	Bind(LOC, par._initialSize);
	Bind(LOC, par._initialSpeed);
	Bind(LOC, par._initialRotSpeed);
	Bind(LOC, par._force);
#undef LOC

	_simProg->Exec(par._activenumpar);
	_simProg->Unbind();

	std::swap(par._data[0], par._data[1]);
}

void Renderer::Particles::UpdateData(_ParticleSystem& par) {
	if (!par._data[0] || (par._data[0]->buffer(0)->num() != par._maxparticles)) {
		InitBuffers(par);
	}

	if (par._activenumpar > 0)
		CleanDeadPar(par);

	par._partial_par += Time::delta() * par._emissionRate;
	const int pnum = (int)std::floor(par._partial_par);
	par._partial_par -= pnum;
	par._activenumpar = std::min(par._activenumpar + pnum, par._maxparticles);

	if (par._activenumpar > 0)
		ExecTimestep(par);
}

void Renderer::Particles::Render(const ParticleSystem& par, const Mat4x4& p, const Mat4x4& ip) {
	if (!par->_activenumpar || !par->_material || !par->_material->_shader) return;
	if (!_outProg) {
		InitProgs();
	}

	const auto mv = par->object()->transform()->worldMatrix();
	const auto imv = mv.inverse();

	Vec4 up = imv * (ip * Vec4::up()).normalized();
	Vec4 rht = imv * (ip * Vec4::right()).normalized();
	Vec4 fwd = imv * (ip * Vec4::front()).normalized();
	
	_outProg->vao(par->_data[0]);
	const auto bufs = par->_mesh->buffers();
	_outProg->outputs({ bufs[0], bufs[3], bufs[4] });
	_outProg->Bind();

	int i = 0;
#define LOC _outProg->Loc(i++)
	Bind(LOC, (Vec3)up);
	Bind(LOC, (Vec3)rht);
	Bind(LOC, (Vec3)fwd);
	glUniform1i(LOC, 0);
	glActiveTexture(GL_TEXTURE0);
	par->_lifetimeColor.GetLookupTexture()->Bind();
#undef LOC

	_outProg->Exec(par->_activenumpar, GL_TRIANGLES);
	_outProg->Unbind();

	// -----------  actual drawing  --------------
	const auto& mat = par->_material;

	par->_mesh->Bind();
	mat->shader()->ApplyFlags(); glDepthMask(GL_FALSE); glDepthFunc(GL_ALWAYS);
	mat->SetUniform("_MV", mv);
	mat->SetUniform("_P", p);
	mat->SetUniform("_MVP", p * mv);
	mat->Bind();
	glDrawArrays(GL_TRIANGLES, 0, par->_activenumpar * 6);
	mat->Unbind();
	par->_mesh->Unbind(); glDepthMask(GL_TRUE); glDepthFunc(GL_ALWAYS);
}

CE_END_BK_NAMESPACE
