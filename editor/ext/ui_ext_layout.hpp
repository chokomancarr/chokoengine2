#pragma once
#include "chokoeditor.hpp"
#include "ui_ext.hpp"

CE_BEGIN_ED_NAMESPACE

typedef uint CE_UI_BUTTON_MASK;

//variables used for additional buttons in layout
constexpr uint CE_UI_BUTTON_CLOSE	= 1 << 0;
constexpr uint CE_UI_BUTTON_UPDOWN	= 1 << 1;

//these are only for return values, do not pass as argument
constexpr uint CE_UI_BUTTON_UP		= 1 << 2;
constexpr uint CE_UI_BUTTON_DOWN	= 1 << 3;

typedef uint CE_UI_BLOCK_FLAGS;

constexpr uint CE_UI_BLOCK_CLOSED	= 1 << 0;

class UI_Ext::Layout {

public:
	class InfoSt {
	public:
		struct Block;

		class BlockGrp {
		public:
			std::vector<Block> blks = {};
			int n = 0;
			void Clear();
			Block& Get(int i, bool expanded = true);
		};

		struct Block {
			Block(bool e = true) : expanded(e) {}

			bool expanded = true;
			float y0 = 0;
			float h = 0;
			int i = 0;
			BlockGrp children;
		};

		Block base = Block();
		Block* current = nullptr;

		float x = 0, y = 0, w = 0;
	};

	static void BeginLayout(const Rect&, InfoSt&);

	static const InfoSt::Block& GetNextBlock(InfoSt& st);

	static CE_UI_BUTTON_MASK Block(const std::string& title, InfoSt&, std::function<void()>, CE_UI_BUTTON_MASK = 0, CE_UI_BLOCK_FLAGS = 0);

	static void IBlock(bool toggle, InfoSt&, std::function<void()>, CE_UI_BLOCK_FLAGS = 0);

	static float EndLayout(InfoSt&);

	template <typename T>
	static bool HandleButtons(CE_UI_BUTTON_MASK, std::vector<T>&, const T&);

	template <typename T>
	static bool HandleButtons(CE_UI_BUTTON_MASK, std::vector<T>&, typename std::vector<T>::const_iterator);
};

CE_END_ED_NAMESPACE

#include "ui_ext_layout.inl"