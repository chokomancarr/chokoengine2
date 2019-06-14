#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

template <class T>
template <class... Args>
Ref<T>::Ref(Args&&... args)
        : _object(std::make_shared<T>(std::forward<Args>(args)...)) {}

template <class T>
void Ref<T>::operator =(const std::shared_ptr<T>& rhs) {
    _object = rhs;
}

template <class T>
T* Ref<T>::operator ->() const {
    return _object.get();
}

template <class T>
Ref<T>::operator bool() const {
    return !!_object;
}

template <class T>
Ref<T>::operator std::shared_ptr<T>() {
    return _object;
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