/////////////////////////////////////////////////////////////////////////////
// This file is part of EasyRPG Player.
//
// EasyRPG Player is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// EasyRPG Player is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <string>
#include <iomanip>
#include <sstream>
#include "window_savefile.h"
#include "game_system.h"
#include "input.h"

////////////////////////////////////////////////////////////
Window_SaveFile::Window_SaveFile(int ix, int iy, int iwidth, int iheight) : 
	Window_Base(ix, iy, iwidth, iheight) {
	index = 0;

	hero_hp = 0;
	hero_level = 0;

	SetContents(Surface::CreateSurface(width - 8, height - 16));
	contents->SetTransparentColor(windowskin->GetTransparentColor());
	SetZ(9999);

	Refresh();
	UpdateCursorRect();
}

Window_SaveFile::~Window_SaveFile() {
}

void Window_SaveFile::UpdateCursorRect() {
	SetCursorRect(GetActive() ? Rect(0, 0, 48, 16) : Rect());
}

void Window_SaveFile::SetIndex(int id) {
	index = id;
}

void Window_SaveFile::SetParty(const std::vector<std::pair<int, std::string> >& actors,
	std::string name, int hp, int level) {
	party = actors;
	hero_name = name;
	hero_hp = hp;
	hero_level = level;
}

void Window_SaveFile::Refresh() {
	contents->Clear();

	std::ostringstream out;
	out << Data::terms.file << std::setw(2) << std::setfill(' ') << index + 1;
	contents->TextDraw(4, 0 + 2, Font::ColorDefault, out.str());

	if (party.empty())
		return;

	contents->TextDraw(8, 16 + 2, Font::ColorDefault, hero_name);

	contents->TextDraw(8, 32 + 2, 1, Data::terms.lvl_short);

	int lx = Surface::GetTextSize(Data::terms.lvl_short).width;
	out.str("");
	out << std::setw(2) << std::setfill(' ') << hero_level;
	contents->TextDraw(8 + lx, 32 + 2, Font::ColorDefault, out.str());

	contents->TextDraw(42, 32 + 2, 1, Data::terms.hp_short);

	int hx = Surface::GetTextSize(Data::terms.hp_short).width;
	out.str("");
	out << hero_hp;
	contents->TextDraw(42 + hx, 32 + 2, Font::ColorDefault, out.str());

	for (int i = 0; i < 4 && (size_t) i < party.size(); i++) {
		DrawFace(party[i].second, party[i].first, 88 + i * 56, 0);
	}
}

void Window_SaveFile::Update() {
	Window_Base::Update();
	UpdateCursorRect();
}

