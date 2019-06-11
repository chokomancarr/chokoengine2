#include "chokoengine.hpp"

BEGIN_CE_NAMESPACE

RefCnt::RefCnt() {
	_ptr = std::make_shared<char>(1);
}

RefCnt::RefCnt(const RefCnt& rhs) : _ptr(rhs._ptr) {}

RefCnt& RefCnt::operator= (const RefCnt& rhs) {
	CheckUniqueRef();
	_ptr = rhs._ptr;
	return *this;
}

void RefCnt::CheckUniqueRef() {
	if (_ptr.unique())
		DestroyRef();
}

END_CE_NAMESPACE