#pragma once

namespace std {
	template<class _Ty>
	struct remove_const_ref {
		using type = _Ty;
	};
	template<class _Ty>
	struct remove_const_ref<const _Ty&> {
		using type = _Ty;
	};
}
