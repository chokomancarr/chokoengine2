#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

DataStream::DataStream(std::ifstream& strm, size_t len, const std::string& path)
	:_usefstrm(false), _fstrm(), _istrm(strm), path(path), len(len) {}

DataStream::DataStream(std::ifstream&& strm, size_t len, const std::string& path)
	: _usefstrm(true), _fstrm(std::move(strm)), _istrm(_fstrm), path(path), len(len) {}

DataStream::DataStream(const std::string& path) : DataStream(path.c_str()) {}

DataStream::DataStream(const char* path) : _usefstrm(true),
		_fstrm(std::ifstream(path, std::ios::binary | std::ios::ate)),
		_istrm(_fstrm), path(path), len(0) {
	len = _fstrm.tellg();
	_fstrm.seekg(0);
	_fstrm.clear();
}

bool DataStream::operator !() const {
	return !strm();
}

std::istream& DataStream::operator ()() const {
	return strm();
}

std::istream* DataStream::operator ->() const {
	return &strm();
}

DataStream::operator std::istream &() const {
	return strm();
}

std::istream& DataStream::strm() const {
	return _usefstrm ? (std::istream&)_fstrm : _istrm;
}

CE_END_NAMESPACE