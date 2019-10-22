#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EO_ColorPicker : public EOverlay {
	Shader hShad, svShad;
	Material hMat, svMat;
	
	Vec2 pos;

	class _State {
	public:
		virtual ~_State() = default;

		virtual const Color& get() = 0;
		virtual void set(const Color& o) = 0;
	};
	template <typename F>
	class State : public _State {
	public:
		typedef std::function<void(F)> TpFunc;
		State(const Color& v, TpFunc f) : value(v), func(f) {
			value.ComputeHSV();
		}

		Color value;
		const TpFunc func;

		const Color& get() override { return value; }
		void set(const Color& o) override { func(value = o); }
	};

	class StateR : public _State {
	public:
		StateR(Color& v) : value(v) {}

		Color& value;

		const Color& get() override { return value; }
		void set(const Color& o) override { value = o; }
	};

	std::unique_ptr<_State> state;

public:
	static std::shared_ptr<EO_ColorPicker> instance;

	EO_ColorPicker();

    void Draw() override;
    static void Reg(const Rect& pos, Color& tar);
	template <typename F>
	static void Reg(const Rect& pos, const Color& val, std::function<void(F)> setter);
};

CE_END_ED_NAMESPACE

#include "eo_colorpicker.inl"