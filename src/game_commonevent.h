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

#ifndef _GAME_COMMONEVENT_H_
#define _GAME_COMMONEVENT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <string>
#include <vector>
#include "game_interpreter.h"
#include "rpg_commonevent.h"

////////////////////////////////////////////////////////////
/// Game_CommonEvent class
////////////////////////////////////////////////////////////
class Game_CommonEvent {
public:
	////////////////////////////////////////////////////////
	/// Constructor.
	/// @param common_event_id : database common event id
	////////////////////////////////////////////////////////
	Game_CommonEvent(int common_event_id, bool battle = false);

	////////////////////////////////////////////////////////
	/// Destructor.
	////////////////////////////////////////////////////////
	~Game_CommonEvent();

	////////////////////////////////////////////////////////
	/// Refreshes the common event.
	////////////////////////////////////////////////////////
	void Refresh();

	////////////////////////////////////////////////////////
	/// Updates common event interpreter.
	////////////////////////////////////////////////////////
	void Update();

	/// @return common event index in list
	int GetIndex() const;

	/// @return event name
	std::string GetName() const;

	/// @return trigger condition
	int GetTrigger() const;

	/// @return trigger switch id
	int GetSwitchId() const;

	/// @return event commands list
	std::vector<RPG::EventCommand>& GetList();

private:
	int common_event_id;
	bool battle;

	// Interpreter for parallel common events
	Game_Interpreter* interpreter;
};

#endif
