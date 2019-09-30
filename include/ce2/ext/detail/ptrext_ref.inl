#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template <class T>
Ref<T>::Ref(const std::shared_ptr<T>& o) : _object(o) {}

template <class T>
Ref<T>::Ref() : _object(nullptr) {}

template <class T>
Ref<T>::Ref(std::nullptr_t) : _object(nullptr) {}

template <class T>
Ref<T> Ref<T>::FromPtr(T* ptr) {
    return Ref<T>(std::shared_ptr<T>(ptr));
}

template <class T>
template <class U>
Ref<T>::Ref(const Ref<U>& ref) : _object(std::dynamic_pointer_cast<T>(ref._object)) {}

template <class T>
template <class... Args>
Ref<T> Ref<T>::New(Args&&... args) {
    return Ref<T>(std::make_shared<T>(std::forward<Args>(args)...));
}

template <class T>
Ref<T>& Ref<T>::operator =(const Ref<T>& rhs) {
    _object = rhs._object;
    return *this;
}

template <class T>
T* Ref<T>::operator ->() const {
    if (!_object) {
        Debug::Error("Object Reference", "Cannot dereference: reference is empty!");
        return nullptr;
    }
    else if (_object->_deleted) {
        Debug::Error("Object Reference", "Cannot dereference: object is deleted!");
        return nullptr;
    }
    return _object.get();
}

template <class T>
bool Ref<T>::operator !() const {
    return !_object || _object->_deleted;
}

template <class T>
bool Ref<T>::operator ==(const Ref<T>& rhs) const {
    return this->_object == rhs._object;
}

template <class T>
bool Ref<T>::operator !=(const Ref<T>& rhs) const {
    return this->_object != rhs._object;
}

CE_END_NAMESPACE