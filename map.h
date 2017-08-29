// MAP class declaration

#include <vector>


enum Fire_result { FIRE_Miss, FIRE_Miss_double, FIRE_Miss_destroyed, FIRE_Hit, FIRE_Hit_drown, FIRE_Hit_end };
enum Directions {DIR_UNDEFINED,	DIR_UP,	DIR_DOWN, DIR_LEFT, DIR_RIGHT};

const int DIRECTIONS_COORDS[5][2] = { { 0, 0 }, { 0, -1 }, { 0, 1 }, { -1, 0 }, { 1, 0 } };
const int MAP_SIZE = 10;


class Map
{
private:

	// ENUM

	enum Map_cell { CELL_Empty, CELL_Destroyed, CELL_Fired, CELL_Ship };

	// CONST

	static const int PLAYER_MAP_X = 10;
	static const int PLAYER_MAP_Y = 2;
	static const int ENEMY_MAP_X = 45;
	static const int ENEMY_MAP_Y = 2;

	const char *MAP_SEPARATOR = "+-+-+-+-+-+-+-+-+-+-+";

	// STRUCT

	struct Ship_Block
	{
		int x = -1;
		int y = -1;
		bool ok = true;
	};

	struct Ship
	{
		Ship_Block coords[4];
		int length = 0;
	};

	// ATTRIBUTES

	Map_cell			map[MAP_SIZE][MAP_SIZE];
	std::vector <Ship>	ships;
	int					ship_positions_free[5];
	
	// METHODS

	bool _can_put_ship_here(int x, int y);

public:
	
	// METHODS

	Map();
	void			draw_map(bool hidden);
	void			renew_map_cell(int x, int y, bool hidden, bool draw_drown = false);
	bool			add_ship(int x, int y, int length, bool vertical = false, bool silent = true);
	bool			has_unused_ships(int length = -1);
	Fire_result		fire_to_cell(int x, int y, bool hidden);
	bool			can_fire_here(int x, int y);
	bool			can_fire_here_limited(int x, int y, Directions direction_check);
};