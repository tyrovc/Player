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

#ifndef _GAME_SCREEN_
#define _GAME_SCREEN_

#include <vector>
#include "plane.h"
#include "surface.h"
#include "game_picture.h"
#include "game_character.h"
#include "battle_animation.h"

class Game_Screen {

public:
	Game_Screen();
	~Game_Screen();

	Picture* GetPicture(int id);

	void Reset();
	void TintScreen(int r, int g, int b, int s, int tenths);
	void FlashOnce(int r, int g, int b, int s, int tenths);
	void FlashBegin(int r, int g, int b, int s, int tenths);
	void FlashEnd();
	void ShakeOnce(int power, int speed, int tenths);
	void ShakeBegin(int power, int speed);
	void ShakeEnd();
	void Weather(int type, int strength);
	void PlayMovie(const std::string& filename,
				   int pos_x, int pos_y, int res_x, int res_y);
	void ShowBattleAnimation(int animation_id, int target_id, bool global);
	bool IsBattleAnimationWaiting() const;
	void Update();

private:
	std::vector<Picture*> pictures;

	RPG::SaveScreen& data;
	int flash_sat;		// RPGMaker bug: this isn't saved
	int flash_period;	// RPGMaker bug: this isn't saved
	int shake_direction;

	std::string movie_filename;
	int movie_pos_x;
	int movie_pos_y;
	int movie_res_x;
	int movie_res_y;

	enum Weather {
		Weather_None,
		Weather_Rain,
		Weather_Snow,
		Weather_Fog,
		Weather_Sandstorm
	};

protected:
	struct Snowflake {
		uint16 x;
		uint8 y;
		uint8 life;
	};

	std::vector<Snowflake> snowflakes;

	Plane* weather_plane;
	Surface* weather_surface;
	Bitmap* snow_bitmap;
	Bitmap* rain_bitmap;
	BattleAnimation* animation;

	void InitWeather();
	void StopWeather();
	void InitSnowRain();
	void UpdateSnowRain(int speed);
	void DrawRain();
	void DrawSnow();
	void DrawFog();
	void DrawSandstorm();
};
#endif // __game_screen__
