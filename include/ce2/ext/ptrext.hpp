#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template <class T>
class Ref {
	typedef T TpBase;

	Ref(std::shared_ptr<T> o);

	std::shared_ptr<T> _object;

public:
	Ref();

	Ref(std::nullptr_t);

	template <class U>
	explicit Ref(const Ref<U>& ref);

	template <class... Args>
	static Ref<T> New(Args&&... args);

	Ref<T>& operator =(const Ref<T>& rhs);

	T* operator ->() const;

	bool operator !() const;

	bool operator ==(const Ref<T>& rhs) const;

	bool operator !=(const Ref<T>& rhs) const;

	friend T;
	template <class U>
	friend class Ref;
};

CE_END_NAMESPACE

#include "detail/ptrext.inl"