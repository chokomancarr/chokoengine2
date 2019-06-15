#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class JsonObject;
class JsonPair;

class JsonParser {
public:
	static JsonObject Parse(std::string text);
};

class JsonObject {
public:
	enum class Type {
		Unknown,
		List,
		Group,
		String
	} type;

	std::vector<JsonPair> group;
	std::vector<JsonObject> list;
	std::string string;

	static JsonObject ParseNext(std::istringstream& ss);
	static JsonObject ParseString(std::istringstream& ss);
};

class JsonPair {
public:
	JsonPair(const JsonObject& k, const JsonObject& v) : key(k), value(v) {}

	JsonObject key;
	JsonObject value;
};

CE_END_NAMESPACE