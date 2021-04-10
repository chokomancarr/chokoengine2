#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template <class T>
class Ref_w {
public:
	typedef T _TpBase;
	typedef std::weak_ptr<T> _TpPtr;

private:
	Ref_w(_TpPtr o);

	_TpPtr _object;

public:
	Ref_w();

	Ref_w(std::nullptr_t);

	template <class U, typename std::enable_if<
		std::is_base_of<T, U>::value, U>::type* = nullptr>
	Ref_w(const Ref<U>& ref);

	template <class U, typename std::enable_if<
		!std::is_base_of<T, U>::value, U>::type* = nullptr>
	explicit Ref_w(const Ref<U>& ref);

	template <class U, typename std::enable_if<
		std::is_base_of<T, U>::value, U>::type* = nullptr>
	Ref_w(const Ref_w<U>& ref);

	template <class U, typename std::enable_if<
		!std::is_base_of<T, U>::value, U>::type* = nullptr>
	explicit Ref_w(const Ref_w<U>& ref);

	Ref_w<T>& operator =(std::nullptr_t);

	Ref_w<T>& operator =(const Ref<T>& rhs);

	Ref_w<T>& operator =(const Ref_w<T>& rhs);

	T* operator ->() const;

	bool operator !() const;

	bool operator ==(const Ref<T>& rhs) const;

	bool operator ==(const Ref_w<T>& rhs) const;

	bool operator !=(const Ref<T>& rhs) const;

	bool operator !=(const Ref_w<T>& rhs) const;

	CE_GET_MEMBER(object);

	Ref<T> lock() const;

	friend T;
	template <class U>
	friend class Ref_w;
};

CE_END_NAMESPACE

#include "detail/ptrext_ref_weak.inl"