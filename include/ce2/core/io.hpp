#pragma once
#include "chokoengine.hpp"
#include "enums/io_terminal_color.hpp"

CE_BEGIN_NAMESPACE

class IO { CE_CLASS_COMMON
	static CE_EXPORT std::string _path;
	static CE_EXPORT std::string _userPath;

	static bool Init();
public:
	/* Directory of the current executable
	 */
	CE_GET_ST_MEMBER(path);
	
	/* Directory of the local user
	 * For windows, it is usually <drive>:/Users/<username>/
	 * For unix, it is equivalent to ~/
	 */
	CE_GET_ST_MEMBER(userPath);

	/* Returns all files in directory \p dir,
	 * optionally with an extension filter
	 */
	static std::vector<std::string> ListFiles(const std::string& dir, const std::string& ext = "");

	/* Checks if \p path is a valid file on disk
	*/
	static bool FileExists(const std::string& path);

	/* Returns the last modification time of the file
	 */
	static time_t ModTime(const std::string& path);

	/* Returns the contents of \p path as text
	*/
	static std::string ReadFile(const std::string& path);
	
	/* Returns the contents of \p path as binary
	*/
	static std::vector<unsigned char> ReadFileBinary(const std::string& path);
	
	/* Deletes the file specified by \p path
	*/
	static void RemoveFile(const std::string& path);

	/* Returns all sub-directories in directory \p dir
	 */
	static std::vector<std::string> ListDirectories(const std::string& dir);
	
	static std::string ColorOutput(const std::string& s, TerminalColor c);
};

CE_END_NAMESPACE
