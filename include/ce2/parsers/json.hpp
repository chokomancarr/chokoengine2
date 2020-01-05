#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class JsonObject;
class JsonPair;

namespace JsonParser {
	JsonObject Parse(std::string);

	std::string Export(const JsonObject&, bool min = false);
}

class JsonObject {
public:
	enum class Type {
		Unknown,
		List,
		Group,
		String
	};

	JsonObject() = default;
	JsonObject(Type t);
	JsonObject(const std::string& val);
	JsonObject(const std::vector<JsonPair>& pairs);
	JsonObject(const std::vector<JsonObject>& items);

	Type type;

	std::vector<JsonPair> group;
	std::vector<JsonObject> list;
	std::string string;

	const JsonObject& Get(const std::string& k) const;
	bool ToBool() const;
	int ToInt() const;
	float ToFloat() const;
	Vec2 ToVec2() const;
	Vec3 ToVec3() const;
	Vec4 ToVec4() const;
	Quat ToQuat() const;
	Color ToColor() const;
	template <typename T>
	T ToEnum(const std::initializer_list<std::string>& entries) const;

	static JsonObject FromVec2(const Vec2& v);
	static JsonObject FromVec3(const Vec3& v);
	static JsonObject FromVec4(const Vec4& v);
	static JsonObject FromQuat(const Quat& q);
	static JsonObject FromColor(const Color& c);

	std::string DoExport(int ind) const;
};

class JsonPair {
public:
	JsonPair(const JsonObject& k, const JsonObject& v) : key(k), value(v) {}

	JsonObject key;
	JsonObject value;
};

CE_END_NAMESPACE

#include "detail/json.inl"