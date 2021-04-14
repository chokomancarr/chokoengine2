#include "chokoengine.hpp"
#include <cctype>

CE_BEGIN_NAMESPACE

#define RETERR(str) {\
	Debug::Error("Json Parser", "Failed to parse json: " str);\
	obj.type = JsonObject::Type::Unknown;\
	return obj;\
}

#define EXPECT(e) RETERR("unexpected '" + std::string(1, c) + "'; expected " #e "!");

namespace {
	struct strmSt {
		const char* pos;
		const char* last;

		strmSt(const std::string& s) {
			pos = s.data();
			last = &s.back();
		}

		char next() {
			return *pos++;
		}
		char peek() {
			return *pos;
		}
		void inc() {
			pos++;
		}

		bool operator !() const {
			return pos > last;
		}
	};

	JsonObject ParseString(strmSt& ss) {
		JsonObject obj = JsonObject();
		obj.type = JsonObject::Type::String;
		char c = ss.next();
		if (c != '"')
			EXPECT('\"');

		auto& s2 = obj.string;
		bool esc = false;
		for (;;) {
			c = ss.next();
			if (esc) {
				s2 += c;
				esc = false;
			}
			else {
				if (!(esc = (c == '\\'))) {
					if (c == '\"')
						break;
					else s2 += c;
				}
			}
		}
		return obj;
	}

	JsonObject ParseNext(strmSt& ss) {
		JsonObject obj = JsonObject();
		char c = ss.next();
		if (c == '[') { //array
			obj.type = JsonObject::Type::List;
			if (ss.peek() == ']') {
				ss.inc();
				return obj;
			}
			while (1) {
				if (ss.peek() == '"') {
					obj.list.push_back(ParseString(ss));
				}
				else {
					obj.list.push_back(ParseNext(ss));
				}

				c = ss.next();
				if (c == ']') break;
				else if (c != ',')
					EXPECT(',');
			}
			return obj;
		}
		else if (c == '{') { //object group
			obj.type = JsonObject::Type::Group;
			if (ss.peek() == '}') {
				ss.inc();
				return obj;
			}
			while (1) {
				auto s1 = ParseString(ss);
				c = ss.next();
				if (c != ':')
					EXPECT(':');
				if (ss.peek() == '"') {
					obj.group.push_back(JsonPair(s1, ParseString(ss)));
				}
				else {
					obj.group.push_back(JsonPair(s1, ParseNext(ss)));
				}

				c = ss.next();
				if (c == '}') break;
				else if (c != ',')
					EXPECT(',');
			}
			return obj;
		}

		EXPECT('"' '[' or '{');
	}
}

JsonObject JsonParser::Parse(std::string text) {
	if (text.size() > 3 && //check utf-8 bom
		byte(text[0]) == 0xef &&
		byte(text[1]) == 0xbb &&
		byte(text[2]) == 0xbf) {
		text = text.substr(3);
	}

	if (text.empty()) return {};
	
	std::string text2;
	text2.reserve(text.size());
	bool intext = false;
	bool esc = false;
	for (auto& c : text) {
		if (c == '\\') {
			esc = true;
			text2.push_back(c);
		}
		else {
			if (c == '\"' && !esc) intext = !intext;
			if (intext || !::isspace(c)) {
				text2.push_back(c);
			}
			esc = false;
		}
	}

	strmSt ss(text2);

	return ParseNext(ss);
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

bool JsonObject::ToBool(const bool def) const {
	if ((string == "true") || (string == "1"))
		return true;
	if ((string == "false") || (string == "0"))
		return false;
	return def;
}

int JsonObject::ToInt(const int def) const {
	return StrExt::ToInt(string, def);
}

float JsonObject::ToFloat(const float def) const {
	return StrExt::ToFloat(string, def);
}

Vec2 JsonObject::ToVec2(const Vec2 def) const {
	return Vec2(
		StrExt::ToFloat(list[0].string, def.x),
		StrExt::ToFloat(list[1].string, def.y)
	);
}

Vec3 JsonObject::ToVec3(const Vec3 def) const {
	return Vec3(
		StrExt::ToFloat(list[0].string, def.x),
		StrExt::ToFloat(list[1].string, def.y),
		StrExt::ToFloat(list[2].string, def.z)
	);
}

Vec4 JsonObject::ToVec4(const Vec4 def) const {
	return Vec4(
		StrExt::ToFloat(list[0].string, def.x),
		StrExt::ToFloat(list[1].string, def.y),
		StrExt::ToFloat(list[2].string, def.z),
		StrExt::ToFloat(list[3].string, def.w)
	);
}

Quat JsonObject::ToQuat(const Quat def) const {
	return Quat(
		StrExt::ToFloat(list[0].string, def.w),
		StrExt::ToFloat(list[1].string, def.x),
		StrExt::ToFloat(list[2].string, def.y),
		StrExt::ToFloat(list[3].string, def.z)
	);
}

Color JsonObject::ToColor(const Color def) const {
	return Color(
		StrExt::ToFloat(list[0].string, def.r),
		StrExt::ToFloat(list[1].string, def.g),
		StrExt::ToFloat(list[2].string, def.b),
		StrExt::ToFloat(list[3].string, def.a)
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
