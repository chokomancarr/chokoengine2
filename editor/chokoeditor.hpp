#pragma once
#include "chokolait.hpp"
#include "editor_defines.hpp"

CE_BEGIN_ED_NAMESPACE

class ChokoEditor {
	static void Init();

public:
	static std::string projectRoot;

	static Scene scene;

	static void Main();
};

#define CE_DIR_ASSET (ChokoEditor::projectRoot + "assets/")
#define CE_DIR_CONFIG (ChokoEditor::projectRoot + "config/")
#define CE_DIR_SYSTEM (ChokoEditor::projectRoot + "system/")
#define CE_DIR_USER (ChokoEditor::projectRoot + "user/")
#define CE_DIR_TEMP (ChokoEditor::projectRoot + "temp/")

CE_END_ED_NAMESPACE

#include "editor_includes.hpp"