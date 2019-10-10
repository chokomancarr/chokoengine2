#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class BlenderExporter {

public:
	static std::string exePath;

	static bool ExportBlend(const std::string& file, const std::string& root_fd, const std::string& rel_fd);
	static bool ExportImage(const std::string& file, const std::string& dst_fd, const std::string& ext = "PNG");
};

CE_END_ED_NAMESPACE