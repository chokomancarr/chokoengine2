#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template <class T>
class Ref {
public:
	typedef T _TpBase;
	typedef std::shared_ptr<T> _TpPtr;

private:
	Ref(_TpPtr o);

	_TpPtr _object;

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

	CE_GET_MEMBER(object);

	friend T;
	template <class U>
	friend class Ref;
	friend class _Component; //to allow object from weak pointer
};

CE_END_NAMESPACE

#include "detail/ptrext.inl"