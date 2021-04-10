#pragma once
#include "prefab_inc.hpp"

CE_BEGIN_NAMESPACE

template <typename T>
T Prefab_ObjRef::Seek(T root,
		const std::function<std::string(T)> nameof,
		const std::function<size_t(T)> numchildren,
		const std::function<T(T, size_t)> childof) const {
	
	typedef decltype(path)::const_iterator _it;
	static typename std::remove_reference<T>::type null = {};
	
	if (empty) {
		return null;
	}
	
	if (!path.size()) {
		return root;
	}

	const std::function<T(_it, T)> seek =
			[&](_it it, T o) -> T {
		const auto& s = it->first;
		int i = it->second;
		const size_t n = numchildren(o);
		for (size_t a = 0; a < n; a++) {
			T o2 = childof(o, a);
			if (nameof(o2) == s) {
				if (!(i--)) {
					it++;
					if (it == path.end()) return o2;
					else {
						return seek(it, o2);
					}
				}
			}
		}
		return null;
	};

	return seek(path.begin(), root);
}

CE_END_NAMESPACE