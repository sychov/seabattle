// Game flow class module. header

#include "map.h"
#include "console.h"


class Seabattle_Flow
{
private:

	// STRUCT

	struct Point_to_check 
	{
		Coords coords;
		Directions direction;
	};

	// ATTRIBUTES

	bool seeking = true;
	std::vector <Point_to_check> check_points;

	// METHODS

	void			_add_check_points(int x, int y);
	Point_to_check	_get_check_point();
	void			_proceed_check_point(Point_to_check point);

public:

	// ATTRIBUTES

	Map player_map, enemy_map;
	Seabattle_Console io_module;

	// METHODS
	
	Seabattle_Flow();
	void prepare_player_battlefield();
	void set_battlefield_authomatically(Map &map, bool hidden);
	bool player_turn();
	bool enemy_turn();
	void good_bye();
};