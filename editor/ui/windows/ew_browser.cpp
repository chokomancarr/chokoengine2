#include "chokoeditor.hpp"

CE_BEGIN_ED_NAMESPACE

void EW_Browser::File::GetIcon() {
	if (type.exported) {

	}
	else {
		switch (type.assetType) {
		case AssetType::Texture:
			icon = (Texture)EAssetList::Get(type.assetType, sig, true);
			break;
		case AssetType::Material:

			break;
		default:
			break;
		}
	}
}


void EW_Browser::ScanFolder(Folder& pfd) {
	auto fls = IO::ListFiles(ChokoEditor::assetPath + pfd.path);
	for (auto& l : fls) {
		if (l[0] == '.') continue;
		const auto lsz = l.size();
		if (lsz > 5 && l.substr(lsz - 5) == ".meta") continue;
		File f(l);
		f.sig = pfd.path + l;
		f.type = EAssetList::TypeOf(l);
		pfd.files.push_back(f);
		auto& fl = pfd.files.back();
		const auto sfls = IO::ListFiles(ChokoEditor::assetPath + pfd.path + l);
		for (auto& s : sfls) {
			if (s[0] == '.') continue;
			const auto ssz = s.size();
			if (ssz > 5 && s.substr(ssz - 5) == ".meta") continue;
			File f(s);
			f.sig = pfd.path + s;
			f.type = EAssetList::TypeOf(s);
			fl.subfiles.push_back(File(s));
		}
		std::sort(fl.subfiles.begin(), fl.subfiles.end(), [](const File& a, const File& b) {
			return a.name < b.name;
		});
	}
	std::sort(pfd.files.begin(), pfd.files.end(), [](const File& a, const File& b) {
		return a.name < b.name;
	});

	auto fds = IO::ListDirectories(ChokoEditor::assetPath + pfd.path);
	for (auto& f : fds) {
		if (f[0] == '.') continue;
		pfd.children.push_back(Folder(f, pfd.path + f + "/"));
		ScanFolder(pfd.children.back());
	}
}

float EW_Browser::DrawFolder(float& off, Folder& fd, int level) {
	const auto dx = 5 * level + 2.f;
	const auto mw = 150.f;
	float ol = 0;
	auto isa = (&fd == currentFd);
	auto style = isa ?
		UIButtonStyle(Color(0.2f), Color(0.25f), Color(0.1f)) :
		UIButtonStyle(Color(0, 0), Color(0.3f, 0.5f), Color(0.1f));
	const bool hc = !!fd.children.size();
	bool prs = false;
	if (hc) {
		auto hb = UI::I::ButtonTr(Rect(position.x() + dx, off, 16, 16));
		prs = !!((int)(hb) & 0x0f);
		if (hb == InputMouseStatus::HoverUp) {
			fd.expanded = !fd.expanded;
		}
	}
	if (!prs && UI::I::Button(Rect(position.x() + 1, off, mw - 2, 16), style)
			== InputMouseStatus::HoverUp) {
		currentFd = &fd;
		isa = true;
	}
	if (hc) {
		UI::Texture(Rect(position.x() + dx, off, 16, 16), EIcons::icons[fd.expanded ? "minus" : "plus"], Color(0.8f));
	}
	UI::Label(Rect(position.x() + dx + 17, off, mw - dx - 18, 16), fd.name, Color(0.8f));
	
	if (level > 0) {
		ol = off + 8;
		UI::Rect(Rect(position.x() + dx - 4, ol, hc ? 5 : 15, 1), Color(0.7f, 0.3f));
	}

	const auto off0 = off += 17;
	if (hc && fd.expanded) {
		for (auto& f : fd.children) {
			auto ol = DrawFolder(off, f, level + 1);
			UI::Rect(Rect(position.x() + dx, off0, 1, ol - off0), Color(0.7f, 0.3f));
		}
	}
	return ol;
}

void EW_Browser::DrawFiles() {
	if (!currentFd || !currentFd->files.size()) return;
	static UIButtonStyle style(Color(0, 0), Color(0.3f, 0.5f), Color(0.1f, 0.5f));

	const float pd = 100;
	const float sz = 64;
	const auto x1 = position.x() + 154;
	const auto w1 = position.w() - 155;
	static EUILayout::ScrollState st = {};
	float off = EUILayout::BeginScroll(Rect(x1, position.y() + 21, w1, position.h() - 23), st);

	auto& fls = currentFd->files;
	int nx = (int)(w1 / pd);
	float dx = w1 / nx - 1;
	float x = x1;
	int i = 0;
	for (auto& f : fls) {
		if (i == nx) {
			off += sz + 24;
			x = x1;
			i = 0;
		}
		if (UI::I::Button(Rect(x, off, pd, sz + 24), style) == InputMouseStatus::HoverUp) {

		}
		f.GetIcon();
		UI::Texture(Rect(x + (pd - sz) / 2, off + 2, sz, sz), f.icon);
		UI::Label(Rect(x, off + sz + 4, pd, 16), f.name, Color::white());
		x += dx;
		i++;
	}
	off += sz + 20;

	EUILayout::EndScroll(st, off);
}

void EW_Browser::DrawMenu() {

	UI::Rect(Rect(position.x() + 1, position.y() + 20, 152, position.h() - 21), Color(0.1f, 0.6f));
	static EUILayout::ScrollState st = {};
	float off = EUILayout::BeginScroll(Rect(position.x() + 2, position.y() + 21, 150, position.h() - 23), st);
    DrawFolder(off, baseFd, 0);
	EUILayout::EndScroll(st, off);

	UI::defaultFont()->alignment(FontAlign::TopCenter);
	DrawFiles();
	UI::defaultFont()->alignment(FontAlign::TopLeft);
}

EW_Browser::EW_Browser() : EWindow("Assets") {}

bool EW_Browser::Init() {
	path = "";
	Refresh();
	return true;
}

void EW_Browser::Refresh() {
	currentFd = nullptr;
	baseFd = Folder("Assets");
	ScanFolder(baseFd);
}

CE_END_ED_NAMESPACE