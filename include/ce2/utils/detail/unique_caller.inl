#include "chokoengine.hpp"

template <typename T, size_t N>
size_t std::hash<std::array<T, N>>::operator()(const std::array<T, N>& a) const {
    std::hash<T> hasher;
    size_t h = 0;
    for (size_t i = 0; i < N; i++) {
        h = h * 31 + hasher(a[i]);
    }
    return h;
}