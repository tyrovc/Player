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

#ifdef USE_SDL

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <ctime>
#include "time.hpp"
#include "SDL.h"

////////////////////////////////////////////////////////////
uint32 Time::GetTicks() {
	return SDL_GetTicks();
}

////////////////////////////////////////////////////////////
uint64 Time::GetTimestamp() {
	return (uint64)std::time(NULL);
}

////////////////////////////////////////////////////////////
void Time::Sleep(uint32 time) {
	SDL_Delay(time);
}

#endif
