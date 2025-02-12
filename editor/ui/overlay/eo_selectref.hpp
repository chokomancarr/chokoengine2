#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EO_SelectRef : public EOverlay {
	class _State {
	public:
		virtual ~_State() = default;

		enum class Type {
			Asset,
			Comp,
			Scr
		} type;

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

	AssetType _assetType;

	std::unique_ptr<_State> state;

	std::vector<std::string> _assets;
	std::vector<Component> _comps;

	template <typename T>
	void ScanComps();

public:
	static std::shared_ptr<EO_SelectRef> instance;

	void Draw() override;

	template <typename T>
	static void RegAsset(T& slot, AssetType t = EAssetTypeOf<T>::value);
	template <typename T, typename F>
	static void RegAsset(const T& slot, std::function<void(F)> setter, AssetType t = EAssetTypeOf<T>::value);
	template <typename T>
	static void RegComp(T& slot);
	template <typename T, typename F>
	static void RegComp(const T& slot, std::function<void(F)> setter);

	/* this is used for selecting script source.
	 * script component references should use RegComp
	 */
	static void RegScr(const ScriptInfo& info, std::function<void(ScriptInfo)> setter);
};

CE_END_ED_NAMESPACE

#include "eo_selectref.inl"