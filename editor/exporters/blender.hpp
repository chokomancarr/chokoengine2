#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class BlenderExporter {

public:
	static std::string exePath;

	void Export(const std::string& file, const std::string& dst_fd);
};

CE_END_ED_NAMESPACE