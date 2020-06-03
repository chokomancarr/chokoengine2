#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

bool Cursor::_locked;
bool Cursor::_visible;

void Cursor::visible(const bool& v) {
	_visible = v;

}

CE_END_NAMESPACE