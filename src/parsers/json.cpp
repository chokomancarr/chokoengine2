#include "chokoengine.hpp"
#include "ce2/parsers/json.hpp"
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

std::string JsonParser::Export(const JsonObject& o, bool min) {
	auto res = o.DoExport(min ? -1 : 0);
	res.pop_back();
	return res;
}


JsonObject::JsonObject(Type t) : type(t), group({}), list({}), string("") {}

JsonObject::JsonObject(const std::string& val) : type(Type::String), group({}), list({}), string(val) {}

JsonObject::JsonObject(const std::vector<JsonPair>& pairs) : type(Type::Group), group(pairs), list({}), string("") {}

JsonObject::JsonObject(const std::vector<JsonObject>& items) : type(Type::List), group({}), list(items), string("") {}

const JsonObject& JsonObject::Get(const std::string& k) const {
	static JsonObject null = {};
	if (type != Type::Group) {
		Debug::Warning("JsonObject", "[] operator cannot be used on non-group object!");
		return null;
	}
	for (auto& g : group) {
		if (g.key.string == k) return g.value;
	}
	return null;
}

bool JsonObject::ToBool() const {
	if ((string == "true") || (string == "1"))
		return true;
	if ((string == "false") || (string == "0"))
		return false;
	Debug::Warning("JsonObject", "Value \"" + string + "\" cannot be casted to bool, assuming false!");
	return false;
}

int JsonObject::ToInt() const {
	return std::stoi(string);
}

float JsonObject::ToFloat() const {
	return std::stof(string);
}

Vec3 JsonObject::ToVec3() const {
	return Vec3(
		std::stof(list[0].string),
		std::stof(list[1].string),
		std::stof(list[2].string)
	);
}

Quat JsonObject::ToQuat() const {
	return Quat(
		std::stof(list[0].string),
		std::stof(list[1].string),
		std::stof(list[2].string),
		std::stof(list[3].string)
	);
}

Color JsonObject::ToColor() const {
	return Color(
		std::stof(list[0].string),
		std::stof(list[1].string),
		std::stof(list[2].string),
		std::stof(list[3].string)
	);
}

JsonObject JsonObject::FromVec2(const Vec2& v) {
	return JsonObject(std::vector<JsonObject>{
		JsonObject(std::to_string(v.x)),
		JsonObject(std::to_string(v.y))
	});
}

JsonObject JsonObject::FromVec3(const Vec3& v) {
	return JsonObject({
		JsonObject(std::to_string(v.x)),
		JsonObject(std::to_string(v.y)),
		JsonObject(std::to_string(v.z))
	});
}

JsonObject JsonObject::FromVec4(const Vec4& v) {
	return JsonObject({
		JsonObject(std::to_string(v.x)),
		JsonObject(std::to_string(v.y)),
		JsonObject(std::to_string(v.z)),
		JsonObject(std::to_string(v.w))
	});
}

JsonObject JsonObject::FromQuat(const Quat& q) {
	return JsonObject({
		JsonObject(std::to_string(q.w)),
		JsonObject(std::to_string(q.x)),
		JsonObject(std::to_string(q.y)),
		JsonObject(std::to_string(q.z))
	});
}

JsonObject JsonObject::FromColor(const Color& c) {
	return JsonObject({
		JsonObject(std::to_string(c.r)),
		JsonObject(std::to_string(c.g)),
		JsonObject(std::to_string(c.b)),
		JsonObject(std::to_string(c.a))
	});
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

#pragma optimize( "", off )
std::string JsonObject::DoExport(int ind) const {
	const bool min = ind < 0;
	std::string res;
#define nl if (!min) res += "\n"
#define tb(i) if (!min) res.insert(res.end(), (i) * 2, ' ');
#define nltb(i) nl;tb(i)
	switch (type) {
	case Type::Group:
		if (group.empty()) return "{},";
		res = "{";
		for (auto& g : group) {
			nltb(ind + 1);
			res += "\"" + g.key.string + "\":" + g.value.DoExport(ind + 1);
		}
		res.pop_back(); nltb(ind);
		res += "},";
		return res;
	case Type::List:
		if (list.empty()) return "[],";
		res = "[";
		for (auto& s : list) {
			nltb(ind + 1);
			res += s.DoExport(ind + 1);
		}
		res.pop_back(); nltb(ind);
		res += "],";
		return res;
	case Type::String:
		return "\"" + string + "\",";
	default:
		return "\"???\",";
		break;
	}
}

CE_END_NAMESPACE
