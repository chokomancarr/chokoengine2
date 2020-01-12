#pragma once
#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

class EW_Browser : public EWindow {
	class File {
	public:
		File(const std::string& nm = "") : name(nm) {}

		std::string name;
		std::string sig;
		EAssetList::TypeOfSt type;
		Texture icon = nullptr;

		std::vector<File> subfiles;

		void GetIcon();
	};
	class Folder {
	public:
		Folder(const std::string& nm = "", const std::string& pth = "")
				: name(nm), path(pth) {}

		std::string name;
		std::string path;
		std::vector<Folder> children = {};
		std::vector<File> files = {};
		bool expanded = false;
	};

	std::string path;
	Folder baseFd;
	Folder* currentFd;

	void ScanFolder(Folder& v);

	float DrawFolder(float& off, Folder& fd, int level);
	void DrawFiles();

protected:

	void DrawMenu() override;

public:
	Asset selectedAsset;

	EW_Browser();

	bool Init() override;
	void Refresh() override;

};

CE_END_ED_NAMESPACE