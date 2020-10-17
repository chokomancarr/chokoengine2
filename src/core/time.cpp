#include "chokoengine.hpp"
#include <chrono>

CE_BEGIN_NAMESPACE

uint64_t Time::_millis0 = 0;
uint64_t Time::_millis = 0;
uint64_t Time::_lastMillis = 0;
float Time::_time = 0;
float Time::_delta = 0;

uint64_t Time::GetTicks() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void Time::Reset() {
    _millis0 = _lastMillis = GetTicks();
	_millis = 0;
    _time = _delta = 0;
}

void Time::Update() {
    _lastMillis = _millis;
    _millis = actualMillis();
    _time = _millis * 0.001f;
    _delta = (_millis - _lastMillis) * 0.001f;
}

uint64_t Time::actualMillis() {
	return GetTicks() - _millis0;
}

CE_END_NAMESPACE