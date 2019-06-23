#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

MVP::stack MVP::MV = MVP::stack();
MVP::stack MVP::P = MVP::stack();
Mat4x4 MVP::_mv, MVP::_p;
bool MVP::changedMv = true, MVP::changedP = true;
const Mat4x4 MVP::identity = Mat4x4(1.f);
bool MVP::isProj = false;

bool MVP::Init() {
	ClearAll();
	return true;
}

void MVP::Switch(bool proj) {
	isProj = proj;
}

void MVP::Push() {
	if (isProj) P.push(identity);
	else MV.push(identity);
}

void MVP::Pop() {
	if (isProj) {
		if (P.size() > 0) {
			P.pop();
			changedP = true;
		}
	}
	else {
		if (MV.size() > 0) {
			MV.pop();
			changedMv = true;
		}
	}
}

void MVP::Clear() {
	if (isProj) {
		P = stack();
		P.push(Mat4x4(1));
		changedP = true;
	}
	else {
		MV = stack();
		P.push(Mat4x4(1));
		changedMv = true;
	}
}

void MVP::ClearAll() {
		MV = stack();
		changedMv = true;
		P = stack();
		changedP = true;
}

void MVP::Mul(const Mat4x4& mat) {
	if (isProj) {
		P.top() = mat * P.top();
		changedP = true;
	}
	else {
		MV.top() = mat * MV.top();
		changedMv = true;
	}
}

Mat4x4 MVP::modelview() {
	if (changedMv) {
		changedMv = false;
		_mv = identity;
		for (uint i = 0; i < MV.size(); i++) {
			_mv = _mv * MV.c[i];
		}
	}
	return _mv;
}

Mat4x4 MVP::projection() {
	if (changedP) {
		changedP = false;
		_p = identity;
		for (uint i = 0; i < P.size(); i++) {
			_p = _p * P.c[i];
		}
	}
	return _p;
}

CE_END_NAMESPACE