#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EO_SelectEnum : public EOverlay {
	class _State {
	public:
		virtual ~_State() = default;

		virtual void set(const int vl) = 0;
	};
	template <typename V, typename F>
	class State : public _State {
	public:
		typedef std::function<void(F)> TpFunc;
		typedef F TpFuncVal;
		State(const V& v, TpFunc f) : value(v), func(f) {}

		const V value;
		const TpFunc func;

		void set(const int vl) override { func((TpFuncVal)vl); }
	};
	template <typename V>
	class StateR : public _State {
	public:
		StateR(V& v) : value(v) {}

		V& value;

		void set(const int vl) override { value = (V)vl; }
	};

	Vec2 pos;
	std::unique_ptr<_State> state;
	std::vector<std::pair<std::string, int>> entries;

public:
	static std::shared_ptr<EO_SelectEnum> instance;

	void Draw() override;

	template <typename E>
	static void RegEnumStr(const Rect& r, E& slot, const std::unordered_map<E, std::string>& strs);
	template <typename E, typename F>
	static void RegEnumStr(const Rect& r, const E& slot, const std::unordered_map<E, std::string>& strs, std::function<void(F)> setter);
};

CE_END_ED_NAMESPACE

#include "eo_selectenum.inl"