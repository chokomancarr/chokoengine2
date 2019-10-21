#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EO_SelectRef : public EOverlay {
	class _State {
	public:
		virtual ~_State() = default;

		bool comp;

		virtual void set(const Object& o) = 0;
	};
	template <typename V, typename F>
	class State : public _State {
	public:
		typedef std::function<void(F)> TpFunc;
		typedef F TpFuncVal;
		State(const V& v, TpFunc f) : value(v), func(f) {}

		const V value;
		const TpFunc func;

		void set(const Object& o) override { func((TpFuncVal)o); }
	};
	template <typename V>
	class StateR : public _State {
	public:
		StateR(V& v) : value(v) {}

		V& value;

		void set(const Object& o) override { value = (V)o; }
	};

	EAssetType _assetType;

	std::unique_ptr<_State> state;

	std::vector<std::string> _assets;
	std::vector<Component> _comps;

	template <typename T>
	void ScanComps();

public:
	void Draw() override;

	template <typename T>
	static void RegAsset(T& slot);
	template <typename T, typename F>
	static void RegAsset(const T& slot, std::function<void(F)> setter);
	template <typename T>
	static void RegComp(T& slot);
	template <typename T, typename F>
	static void RegComp(const T& slot, std::function<void(F)> setter);
};

CE_END_ED_NAMESPACE

#include "eo_selectref.inl"