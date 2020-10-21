#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class DataStream {
	bool _usefstrm;
	std::ifstream _fstrm;
	std::istream& _istrm;

public:
	DataStream(std::ifstream& strm, size_t len = -1, const std::string& path = "");
	DataStream(std::ifstream&& strm, size_t len = -1, const std::string& path = "");
	DataStream(const std::string& path);
	DataStream(const char* path);

	DataStream(const DataStream&) = delete;
	DataStream(DataStream&&) = default;
	DataStream& operator= (const DataStream&) = delete;

	bool operator! () const;

	std::istream& operator()() const;

	std::istream* operator->() const;

	operator std::istream& () const;

	std::istream& strm() const;

	std::string path;
	size_t len;
};

CE_END_NAMESPACE
