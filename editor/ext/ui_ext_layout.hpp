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
			int n;
			void Clear();
			Block& Get(int i);
		};

		struct Block {
			bool expanded;
			float y0;
			float h;
			int i;
			BlockGrp children;
		};

		Block base;
		std::stack<Block*> current;

		Rect* r;
		float x, y, w;
	};

	static void BeginLayout(Rect&, InfoSt&);

	static void Push(const std::string& title, InfoSt&);

	static void Pop(InfoSt&);

	static void EndLayout(InfoSt&);
};

CE_END_ED_NAMESPACE