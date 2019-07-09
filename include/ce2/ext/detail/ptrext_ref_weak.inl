#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template <class T>
Ref_w<T>::Ref_w(std::weak_ptr<T> o) : _object(o) {}

template <class T>
Ref_w<T>::Ref_w() : _object(_TpPtr()) {}

template <class T>
Ref_w<T>::Ref_w(std::nullptr_t) : _object(_TpPtr()) {}

template <class T>
template <class U>
Ref_w<T>::Ref_w(const Ref<U>& ref) : _object(std::dynamic_pointer_cast<T>(ref._object)) {}

template <class T>
template <class U>
Ref_w<T>::Ref_w(const Ref_w<U>& ref) : _object(std::dynamic_pointer_cast<T>(ref._object)) {}

template <class T>
Ref_w<T>& Ref_w<T>::operator =(const Ref<T>& rhs) {
    _object = rhs._object;
    return *this;
}

template <class T>
Ref_w<T>& Ref_w<T>::operator =(const Ref_w<T>& rhs) {
    _object = rhs._object;
    return *this;
}

template <class T>
T* Ref_w<T>::operator ->() const {
    std::shared_ptr<T> o = 0;
    if (!(o = _object.lock())) {
        Debug::Error("Object Reference", "Cannot dereference: reference is empty!");
        return nullptr;
    }
    else if (o->_deleted) {
        Debug::Error("Object Reference", "Cannot dereference: object is deleted!");
        return nullptr;
    }
    return o.get();
}

template <class T>
bool Ref_w<T>::operator !() const {
    return _object.expired() || _object->_deleted;
}

template <class T>
bool Ref_w<T>::operator ==(const Ref_w<T>& rhs) const {
    return this->_object == rhs._object;
}

template <class T>
bool Ref_w<T>::operator !=(const Ref_w<T>& rhs) const {
    return this->_object != rhs._object;
}

template <class T>
Ref<T> Ref_w<T>::lock() const {
    return _object.lock();
}

CE_END_NAMESPACE