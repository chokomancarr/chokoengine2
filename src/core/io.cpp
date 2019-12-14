#include "chokoengine.hpp"

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
#include <pwd.h>
#ifdef PLATFORM_MAC
#include <mach-o/dyld.h>
#include <uuid/uuid.h>
#endif
#endif

CE_BEGIN_NAMESPACE

std::string IO::_path;
std::string IO::_userPath;

bool IO::Init() {
	const int cpathsz = 200;
	char cpath[cpathsz];
#ifdef PLATFORM_WIN
	GetModuleFileName(NULL, cpath, cpathsz);
	_path = cpath;
	std::replace(_path.begin(), _path.end(), '\\', '/');
	_path = _path.substr(0, _path.find_last_of('/') + 1);
	
	WCHAR wpath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, wpath))) {
		_userPath = StrExt::Unwiden(wpath);
	}
	else return false;
#else
#if defined(PLATFORM_LNX)
	readlink("/proc/self/exe", cpath, 200);
	_path = cpath;
#else
	_NSGetExecutablePath(cpath, (uint32_t*)&cpathsz);
	char cpath2[300];
	_path = realpath(cpath, cpath2);
#endif
	_path = _path.substr(0, _path.find_last_of('/') + 1);
	struct passwd *pw = getpwuid(getuid());
	_userPath = std::string(pw->pw_dir) + "/";
#endif

#ifdef PLATFORM_WIN
	auto so = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD cm;
	if (!GetConsoleMode(so, &cm)) {
		return false;
	}

	cm |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
	if (!SetConsoleMode(so, cm)) {
		return false;
	}
#endif

	return true;
}

std::vector<std::string> IO::ListFiles(const std::string& dir, const std::string& ext) {
	if (dir == "") return std::vector<std::string>();
	std::vector<std::string> names = {};
	auto exts = ext.size();
#ifdef PLATFORM_WIN
	std::string search_path = dir + "/*" + ext;
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
			}
		} while (FindNextFile(hFind, &fd));
		FindClose(hFind);
	}
#else
	DIR* d = opendir(dir.c_str());
	if (!d) return {};
	struct dirent* ep;
	while ((ep = readdir(d))) {
		std::string nm(ep->d_name);
		if (ep->d_type == DT_REG) {
			if (!exts || ((nm.size() > (exts + 1)) && (nm.substr(nm.size() - exts) == ext)))
				names.push_back(nm);
		}
	}
#endif
	return names;
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

time_t IO::ModTime(const std::string& path) {
#ifdef PLATFORM_WIN
	#define stat _stat
#endif
	struct stat stt;
	auto rt = stat(path.c_str(), &stt);
	if (!!rt) return -1;
	return stt.st_mtime;
}

std::string IO::ReadFile(const std::string& path) {
	std::ifstream strm(path);
	if (!strm) {
		Debug::Warning("IO::ReadFile", "Cannot open file: \"" + path + "\"!");
		return "";
	}
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

std::vector<std::string> IO::ListDirectories(const std::string& dir) {
	std::vector<std::string> dirs = {};
#ifdef PLATFORM_WIN
	std::string search_path = dir + "/*";
	WIN32_FIND_DATAW fd;
	HANDLE hFind = FindFirstFileW(StrExt::Widen(search_path).c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			std::string nm(StrExt::Unwiden(fd.cFileName));
			if (!!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && nm != "." && nm != "..") {
				dirs.push_back(nm);
			}
		} while (::FindNextFileW(hFind, &fd));
		::FindClose(hFind);
	}
#else
	DIR* d = opendir(dir.c_str());
	if (!d) return {};
	struct dirent* ep;
	while ((ep = readdir(d))) {
		std::string nm(ep->d_name);
		if (ep->d_type == DT_DIR && nm != "." && nm != "..") {
			dirs.push_back(std::string(ep->d_name));
		}
	}
#endif
	return dirs;
}

std::string IO::ColorOutput(const std::string& s, TerminalColor c) {
#define esc(n) "\033[" #n "m"
	static const std::string cs[] = {
		esc(0),
		esc(30),
		esc(31),
		esc(32),
		esc(33),
		esc(34),
		esc(35),
		esc(36),
		esc(37),
		esc(30;1),
		esc(31;1),
		esc(32;1),
		esc(33;1),
		esc(34;1),
		esc(35;1),
		esc(36;1),
		esc(37;1),
	};
	return cs[(int)c] + s + cs[0];
}

CE_END_NAMESPACE