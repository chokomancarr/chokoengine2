#include "chokoengine.hpp"
#include "parsers/json.hpp"
#include <cctype>

CE_BEGIN_NAMESPACE

#define RETERR(str) {\
	Debug::Error("Json Parser", "Failed to parse json: " str);\
	obj.type = JsonObject::Type::Unknown;\
	return obj;\
}

JsonObject JsonParser::Parse(std::string text) {
	if (text.size() > 3 && //check utf-8 bom
		byte(text[0]) == 0xef &&
		byte(text[1]) == 0xbb &&
		byte(text[2]) == 0xbf) {
		text = text.substr(3);
	}
	
	text.erase(std::remove_if(text.begin(), text.end(), std::isspace), text.end());

	std::istringstream ss(text);

	return JsonObject::ParseNext(ss);
}

JsonObject JsonObject::ParseNext(std::istringstream& ss) {
	JsonObject obj = JsonObject();
	char c;
	ss.read(&c, 1);
	if (c == '[') { //array
		obj.type = JsonObject::Type::List;
		if (ss.peek() == ']') {
			ss.read(&c, 1);
			return obj;
		}
		while (1) {
			if (ss.peek() == '"') {
				obj.list.push_back(ParseString(ss));
			}
			else {
				obj.list.push_back(ParseNext(ss));
			}

			ss.read(&c, 1);
			if (c == ']') break;
			else if (c != ',')
				RETERR("expected ','!");
		}
		return obj;
	}
	else if (c == '{') { //object group
		obj.type = JsonObject::Type::Group;
		if (ss.peek() == '}') {
			ss.read(&c, 1);
			return obj;
		}
		while (1) {
			auto s1 = ParseString(ss);
			ss.read(&c, 1);
			if (c != ':')
				RETERR("expected ':'!");
			if (ss.peek() == '"') {
				obj.group.push_back(JsonPair(s1, ParseString(ss)));
			}
			else {
				obj.group.push_back(JsonPair(s1, ParseNext(ss)));
			}

			ss.read(&c, 1);
			if (c == '}') break;
			else if (c != ',')
				RETERR("expected ','!");
		}
		return obj;
	}

	RETERR("expected data block!");
}

JsonObject JsonObject::ParseString(std::istringstream& ss) {
	JsonObject obj = JsonObject();
	obj.type = JsonObject::Type::String;
	char c;
	ss.read(&c, 1);
	if (c != '"')
		RETERR("expected '\"'!");

	std::string s2;
	do {
		std::getline(ss, s2, '"');
		obj.string += s2;
	} while (s2.back() == '\\');
	return obj;
}

CE_END_NAMESPACE