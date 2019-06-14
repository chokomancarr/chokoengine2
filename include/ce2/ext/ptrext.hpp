#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template <class T>
class Ref {
	std::shared_ptr<T> _object;

public:
	template <class... Args>
	Ref(Args&&... args);

	void operator =(const std::shared_ptr<T>& rhs);

	T* operator ->() const;

	operator bool() const;

	operator std::shared_ptr<T>();

	bool operator ==(const Ref<T>& rhs) const;

	bool operator !=(const Ref<T>& rhs) const;
};

CE_END_NAMESPACE

#include "detail/ptrext.inl"