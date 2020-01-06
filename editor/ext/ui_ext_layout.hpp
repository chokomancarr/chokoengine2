#pragma once
#include "chokoeditor.hpp"
#include "ui_ext.hpp"

CE_BEGIN_ED_NAMESPACE

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
			Block& Get(int i);
		};

		struct Block {
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

	static void Block(const std::string& title, InfoSt&, std::function<void()>);

	static float EndLayout(InfoSt&);
};

CE_END_ED_NAMESPACE