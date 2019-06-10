#pragma once
#include "chokoengine.hpp"

BEGIN_CE_NAMESPACE

class IO {
	static std::string _path;
public:
	GET_MEMBER_FUNC(std::string, path);

	static void Init();

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

END_CE_NAMESPACE
