#pragma once
#include "chokoeditor.hpp"
#include "blender.hpp"

CE_BEGIN_ED_NAMESPACE

#ifdef PLATFORM_WIN
std::string BlenderExporter::exePath = "C:\\Program Files\\Blender Foundation\\Blender\\blender.exe";
#else
std::string BlenderExporter::exePath = "/usr/local/bin/blender";
#endif

void BlenderExporter::Export(const std::string& file, const std::string& dst_fd) {
	std::string cmd("\"" + exePath + "\" \"" + file + "\" -b -P \"" + IO::path() + "/scripts/exporters/export_blender.py\" -- \"" + "\"\n");
}

CE_END_ED_NAMESPACE