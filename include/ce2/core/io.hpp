#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class IO { CE_CLASS_COMMON
	static std::string _path;

	static bool Init();
public:
	CE_GET_ST_MEMBER(path);

	/* Checks if \p path is a valid file on disk
	*/
	static bool FileExists(const std::string& path);

	/* Returns the contents of \p path as text
	*/
	static std::string ReadFile(const std::string& path);
	
	/* Returns the contents of \p path as binary
	*/
	static std::vector<unsigned char> ReadFileBinary(const std::string& path);
	
	/* Deletes the file specified by \p path
	*/
	static void RemoveFile(const std::string& path);
};

CE_END_NAMESPACE
