/*
 * Generated by the preprocessor script
 * DO NOT EDIT
 */
#pragma once
#include "chokoengine.hpp"

namespace std { template <> struct hash<CE_NS LightProbeUpdateFrequency> { size_t operator() (const CE_NS LightProbeUpdateFrequency &t) const { return size_t(t); } }; }

CE_BEGIN_NAMESPACE

const std::unordered_map<LightProbeUpdateFrequency, std::string> LightProbeUpdateFrequencyStr = {
	{ LightProbeUpdateFrequency::OnStart, "OnStart" },
	{ LightProbeUpdateFrequency::Realtime, "Realtime" }
};

CE_END_NAMESPACE
