#include "chokoeditor.hpp"
#include "blender.hpp"

CE_BEGIN_ED_NAMESPACE

#ifdef PLATFORM_WIN
std::string BlenderExporter::exePath = "C:\\Program Files\\Blender Foundation\\Blender\\blender.exe";
#else
std::string BlenderExporter::exePath = "/usr/local/bin/blender";
#endif

void BlenderExporter::ExportBlend(const std::string& file, const std::string& root_fd, const std::string& rel_fd) {
	exePath = EPaths::Get("BLENDER_EXE", exePath);
	const auto p = file.find_last_of('/') + 1;
	Subprocess::Run(exePath, {
		file,
		"-b",
		"-P",
		IO::path() + "scripts/exporters/export_blender.py",
		"--",
		root_fd,
		rel_fd,
		file.substr(p)
	});
}

void BlenderExporter::ExportImage(const std::string& file, const std::string& dst_fd, const std::string& ext) {
	
}

CE_END_ED_NAMESPACE