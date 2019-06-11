#include "chokoengine_common.hpp"

#ifdef PLATFORM_WIN
#include <io.h>
#include <Shlobj.h>
#include <shellapi.h>
#pragma comment(lib, "Shell32.lib")
#else
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>
#ifdef PLATFORM_OSX
#include <mach-o/dyld.h>
#endif
#endif

BEGIN_CE_NAMESPACE

std::string IO::_path;

void IO::Init() {
	const int cpathsz = 200;
	char cpath[cpathsz];
#ifdef PLATFORM_WIN
	GetModuleFileName(NULL, cpath, cpathsz);
	_path = cpath;
	std::replace(_path.begin(), _path.end(), '\\', '/');
#elif defined(PLATFORM_LNX)
	readlink("/proc/self/exe", cpath, 200);
	_path = cpath;
#else
	_NSGetExecutablePath(cpath, &cpathsz);
	path = realpath(cpath, 0);
#endif
	_path = _path.substr(0, _path.find_last_of('/') + 1);
}

bool IO::FileExists(const std::string& path) {
#ifdef PLATFORM_WIN
	auto pathh = path;
	std::replace(pathh.begin(), pathh.end(), '/', '\\');
	DWORD dwAttrib = GetFileAttributesW(StrExt::Widen(pathh).c_str());
	return (dwAttrib != INVALID_FILE_ATTRIBUTES);
#else
	return (access(&path[0], F_OK) != -1);
#endif
}

std::string IO::ReadFile(const std::string& path) {
	std::ifstream strm(path);
	std::stringstream ss;
	ss << strm.rdbuf();
	return ss.str();
}

std::vector<unsigned char> IO::ReadFileBinary(const std::string& path) {
	std::ifstream strm(path, std::ios::binary);
	return std::vector<unsigned char>(std::istreambuf_iterator<char>(strm), {});
}

void IO::RemoveFile(const std::string& path) {
	::remove(path.c_str());
}

END_CE_NAMESPACE