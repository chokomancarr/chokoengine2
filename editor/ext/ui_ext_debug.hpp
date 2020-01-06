#pragma once
#include "chokoeditor.hpp"
#include "ui_ext.hpp"

CE_BEGIN_ED_NAMESPACE

class UI_Ext::Debug {
public:
	class GraphSt {
	private:
		std::vector<float> _vals;
		float min, max;

	public:
		GraphSt(int len, float pad);

		void push(float);
	};

	void Graph(const Rect&, const GraphSt&);
};

CE_END_ED_NAMESPACE