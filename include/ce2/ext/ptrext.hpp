#pragma once
#include "chokoengine.hpp"

BEGIN_CE_NAMESPACE

template <class T> class Ref {
	std::shared_ptr<T> _object;

public:
	template<class... Args>
	Ref(Args&&... args) : _object(std::make_shared<T>(std::forward<Args>(args)...)) {}

	void operator= (const std::shared_ptr<T>& rhs) {
		_object = rhs;
	}

	T* operator->() {
		return _object.get();
	}

	operator bool() const {
		return _object;
	}

	operator std::shared_ptr<T>() {
		return _object;
	}

	bool operator ==(const Ref<T>& rhs) const {
		return this->_object == rhs._object;
	}

	bool operator !=(const Ref<T>& rhs) const {
		return this->_object != rhs._object;
	}
};

END_CE_NAMESPACE