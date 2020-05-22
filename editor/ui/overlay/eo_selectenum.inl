#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

template <typename E>
void EO_SelectEnum::RegEnumStr(const Rect& r, E& slot, const std::unordered_map<E, std::string>& strs) {
	static_assert(std::is_enum<E>::value, "Target is not an enum!");
	auto i = GetInstance<EO_SelectEnum>();
	i->pos = Vec2(r.x(), r.y2());
	i->state = std::unique_ptr<StateR<E>>(new StateR<E>(slot));
	i->entries.clear();
	for (auto& s : strs) {
		i->entries.push_back(std::make_pair(s.second, s.first));
	}
	i->active = true;
}

template <typename E, typename F>
void EO_SelectEnum::RegEnumStr(const Rect& r, const E& slot, const std::unordered_map<E, std::string>& strs, std::function<void(F)> setter) {
	static_assert(std::is_enum<E>::value, "Target is not an enum!");
	auto i = GetInstance<EO_SelectEnum>();
	i->pos = Vec2(r.x(), r.y2());
	i->state = std::unique_ptr<State<E, F>>(new State<E, F>(slot, setter));
	i->entries.clear();
	for (auto& s : strs) {
		i->entries.push_back(std::make_pair(s.second, (int)s.first));
	}
	i->active = true;
}

CE_END_ED_NAMESPACE
