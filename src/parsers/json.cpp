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
	
	std::string text2;
	text2.reserve(text.size());
	bool intext = false;
	bool esc = false;
	for (auto& c : text) {
		if (c == '\\') esc = true;
		else {
			if (c == '\"' && !esc) intext = !intext;
			if (intext || !::isspace(c)) {
				text2.push_back(c);
			}
			esc = false;
		}
	}

	std::istringstream ss(text2);

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

bool JsonObject::ToBool() {
	if ((string == "true") || (string == "1"))
		return true;
	if ((string == "false") || (string == "0"))
		return false;
	Debug::Warning("JsonObject", "Value \"" + string + "\" cannot be casted to bool, assuming false!");
	return false;
}

int JsonObject::ToInt() {
	return std::stoi(string);
}

float JsonObject::ToFloat() {
	return std::stof(string);
}

Color JsonObject::ToColor() {
	return Color(
		std::stof(list[0].string),
		std::stof(list[1].string),
		std::stof(list[2].string),
		std::stof(list[3].string)
	);
}

CE_END_NAMESPACE
