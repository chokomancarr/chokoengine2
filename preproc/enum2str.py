import os
import sys

path = sys.argv[1]
path2 = path[:-4] + "_str.hpp"
src_file = open(path, "r")
dst_file = open(path2, "w")

print("generating: " + path2)

dst_file.write(R"""/*
 * Generated by the preprocessor script
 * DO NOT EDIT
 */
#pragma once
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

const std::unordered_map<""")

first_word = ""

while first_word != "enum":
    line = src_file.readline().strip()
    lsplit = line.split()
    if len(lsplit) > 0:
        first_word = lsplit[0]

enum_name = lsplit[1]
if enum_name == "class":
    enum_name = lsplit[2]
if enum_name[-1] == '{':
    enum_name = enum_name[:-1]

print("  enum: " + enum_name)

dst_file.write(enum_name + ", std::string> " + enum_name + "Str = {\n")

strs = []

while True:
    line = src_file.readline().strip()
    if line[0] == '}':
        break
    lsplit = line.split(",")
    for l in lsplit:
        l = l.strip()
        if not len(l):
            continue
        l = l.split('=')[0].strip()
        strs.append("{ " + enum_name + "::" + l + ", \"" + l + "\" }")

for s in strs[:-1]:
    dst_file.write("\t" + s + ",\n")
dst_file.write("\t" + strs[-1])

dst_file.write(R"""
};

CE_END_NAMESPACE
""")
