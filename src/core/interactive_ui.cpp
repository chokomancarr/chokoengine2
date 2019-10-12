#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

UniqueCallerList UI::I::textFieldCallers;

UI::I::_TextFieldInfo UI::I::_textFieldInfo = {};

void UI::I::PreLoop() {
	textFieldCallers.Preloop();
}

InputMouseStatus UI::I::Button(const CE_NS Rect& r, const UIButtonStyle& s, const CE_NS Texture& t, const Color& c) {
	const auto ret = ButtonTr(r);
	const auto col = (ret == InputMouseStatus::None) ? s.normal() :
		(((ret == InputMouseStatus::Hover) || (ret == InputMouseStatus::HoverUp)) ? s.hover() : s.pressed());
	Rect(r, col);
	
	Texture(r, t, c);

	return ret;
}

InputMouseStatus UI::I::Button(const CE_NS Rect& r, const UIButtonStyle& s, const std::string& t, const Font& font) {
	const auto ret = ButtonTr(r);
	const auto col = (ret == InputMouseStatus::None) ? s.normal() :
		(((ret == InputMouseStatus::Hover) || (ret == InputMouseStatus::HoverUp)) ? s.hover() : s.pressed());
	Rect(r, col);

	if (!t.empty()) {
		const auto tcol = (ret == InputMouseStatus::None) ? s.textNormal() :
			(((ret == InputMouseStatus::Hover) || (ret == InputMouseStatus::HoverUp)) ? s.textHover() : s.textPressed());
		Label(r, t, tcol, font);
	}

	return ret;
}

InputMouseStatus UI::I::ButtonTr(const CE_NS Rect& r) {
	uint ret = 0;
	if (r.Contains(Input::mousePosition())) {
		ret = 0x10;
		const auto mst = Input::mouseStatus(InputMouseButton::Left);
		if (mst != InputMouseStatus::None) {
			if (Input::mousePosition() == Input::mouseDownPosition()) {
				ret |= (uint)mst;
			}
			else {
				ret = 0;
			}
		}
	}
	return (InputMouseStatus)ret;
}

std::string UI::I::TextField(const CE_NS Rect& r, const std::string& s, const UITextFieldStyle& style) {
	bool active = textFieldCallers.Add();

	if (active) {
		Rect(r, style.editing());

		bool chg = false;
		bool lmb = Input::mouseStatus(InputMouseButton::Left) == InputMouseStatus::Down;
		//check for quit
		if ((lmb && (ButtonTr(r) == InputMouseStatus::None))
			|| (Input::KeyDown(InputKey::Escape))
			|| (Input::KeyDown(InputKey::Enter))) {
			textFieldCallers.Clear();
			_textFieldInfo.editing = false;
			return _textFieldInfo.buffer;
		}
		//check for deletion
		if (Input::KeyDown(InputKey::Backspace)) {
			if (_textFieldInfo.cursor == _textFieldInfo.cursor2) { //remove 1 character
				if (!!_textFieldInfo.cursor) {
					_textFieldInfo.ubuffer = _textFieldInfo.ubuffer.substr(0, _textFieldInfo.cursor - 1) + _textFieldInfo.ubuffer.substr(_textFieldInfo.cursor);
					_textFieldInfo.cursor--;
					_textFieldInfo.cursor2--;
				}
			}
			else { //remove all selected
				_textFieldInfo.ubuffer = _textFieldInfo.ubuffer.substr(0, std::min(_textFieldInfo.cursor, _textFieldInfo.cursor2)) + _textFieldInfo.ubuffer.substr(std::max(_textFieldInfo.cursor, _textFieldInfo.cursor2));
				_textFieldInfo.cursor2 = _textFieldInfo.cursor = std::min(_textFieldInfo.cursor, _textFieldInfo.cursor2);
			}
			chg = true;
		}
		//insert input string
		const auto& istr = Input::inputUnicodeString();
		const auto istrsz = istr.size();
		if (istrsz > 0) {
			if (_textFieldInfo.cursor == _textFieldInfo.cursor2) { //add string at position
				_textFieldInfo.ubuffer = _textFieldInfo.ubuffer.substr(0, _textFieldInfo.cursor) + istr + _textFieldInfo.ubuffer.substr(_textFieldInfo.cursor);
				_textFieldInfo.cursor += istrsz;
				_textFieldInfo.cursor2 += istrsz;
			}
			else { //replace selection with string
				const auto cmn = std::min(_textFieldInfo.cursor, _textFieldInfo.cursor2);
				const auto cmx = std::max(_textFieldInfo.cursor, _textFieldInfo.cursor2);
				_textFieldInfo.ubuffer = _textFieldInfo.ubuffer.substr(0, cmn) + istr + _textFieldInfo.ubuffer.substr(cmx);
				_textFieldInfo.cursor2 = _textFieldInfo.cursor = cmn + istrsz;
			}
			chg = true;
		}
		//move cursor
		if (Input::KeyDown(InputKey::LeftArrow)) {
			_textFieldInfo.cursor = (uint)std::max((int)_textFieldInfo.cursor - 1, 0);
			if (!Input::KeyHold(InputKey::LeftShift)) {
				_textFieldInfo.cursor2 = _textFieldInfo.cursor;
			}
		}
		if (Input::KeyDown(InputKey::RightArrow)) {
			_textFieldInfo.cursor = std::min(_textFieldInfo.cursor + 1U, (uint)_textFieldInfo.ubuffer.size());
			if (!Input::KeyHold(InputKey::LeftShift)) {
				_textFieldInfo.cursor2 = _textFieldInfo.cursor;
			}
		}
		if (chg) {
			_textFieldInfo.buffer = StrExt::FromUnicode(_textFieldInfo.ubuffer);
			_textFieldInfo.time = 0;
		}
		
		Label(r, _textFieldInfo.buffer, style.textEditing());

		//move cursor (requires label data)
		const auto lmbs = Input::mouseStatus(InputMouseButton::Left);
		if (lmbs != InputMouseStatus::None) {
			_textFieldInfo.cursor = _textFieldInfo.ubuffer.size();
			for (size_t a = 0; a < _textFieldInfo.ubuffer.size(); a++) {
				const auto x0 = _defaultFont->poss[a * 4].x;
				const auto x1 = _defaultFont->poss[a * 4 + 1].x;
				const auto ps = (((x0 + x1) / 2)
					* 0.5f + 0.5f) * (float)Display::width();
				if (Input::mousePosition().x < ps) {
					_textFieldInfo.cursor = a;
					break;
				}
			}
			if (lmb) {
				_textFieldInfo.cursor2 = _textFieldInfo.cursor;
			}
		}
	}
	else {
		if (Button(r, style.normal()) == InputMouseStatus::HoverUp) {
			textFieldCallers.Set();
			_textFieldInfo.editing = true;
			_textFieldInfo.buffer = s;
			_textFieldInfo.ubuffer = StrExt::ToUnicode(s);
			_textFieldInfo.cursor = _textFieldInfo.ubuffer.size();
			_textFieldInfo.cursor2 = 0;
			_textFieldInfo.time = 0;
		}
		Label(r, s, style.textNormal());
	}
	return s;
}

float UI::I::Slider(const CE_NS Rect& r, const Vec2& range, float value, const Color& color) {
	Rect(r, color);
	const auto v = SliderTr(r, range, value);
	Rect(CE_NS Rect(r.x() + 1, r.y() + 1, (r.w() - 2) * Math::Clamp(Math::ILerp(range.x, range.y, value), 0.f, 1.f), r.h() - 2), Color::white());
	return v;
}

float UI::I::SliderTr(const CE_NS Rect& r, const Vec2& range, float value) {
	const auto mst = Input::mouseStatus(InputMouseButton::Left);
	if (mst != InputMouseStatus::None) {
		if (r.Contains(Input::mouseDownPosition())) {
			value = Math::Lerp(range.x, range.y, Math::ILerp(r.x(), r.x2(), Input::mousePosition().x));
			value = Math::Clamp(value, range.x, range.y);
		}
	}
	return value;
}

bool UI::I::Toggle(const CE_NS Rect& r, bool val, const Color& color) {
	Rect(r, color);
	if (val) Rect(r.sub(3, 3, 3, 3), Color::white());
	return ToggleTr(r, val);
}

bool UI::I::ToggleTr(const CE_NS Rect& r, bool val) {
	if (ButtonTr(r) == InputMouseStatus::HoverUp) return !val;
	else return val;
}

CE_END_NAMESPACE