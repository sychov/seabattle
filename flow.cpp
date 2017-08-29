// Game flow class module.


#include "flow.h"


// Constructor: main game loop
Seabattle_Flow::Seabattle_Flow()
{
	player_map.draw_map(false);
	enemy_map.draw_map(true);
	prepare_player_battlefield();
	set_battlefield_authomatically(enemy_map, true);
	while (true)
	{
		if (player_turn())
		{
			io_module.show_message("Congratulations! You won!", io_module.MSG_System);
			break;
		}

		if (enemy_turn())
		{
			io_module.show_message("It's not your day, pal!", io_module.MSG_System);
			break;
		}
	};
	good_bye();
};


//Fill battlefield authomatically
void Seabattle_Flow::set_battlefield_authomatically(Map &map, bool hidden) {
	int x, y;
	bool vposition;
	
	for (int length = 4; length; --length) 
		for (int q = 0; q < 5 - length; ++q)
			do
			{
				x = rand() % MAP_SIZE;
				y = rand() % MAP_SIZE;
				vposition = (bool)(rand() % 2);
			} while (!map.add_ship(x, y, length, vposition, hidden));
}


// Fulfill player battlefield with ships.
void Seabattle_Flow::prepare_player_battlefield()
{
	int length = 0;
	Coords coords;
	bool confirm;
		
	io_module.show_message("Hello! This is Sea Battle game.", io_module.MSG_System);
	io_module.show_message("If you run it, it means, you want to play it!", io_module.MSG_System);
	io_module.show_message("For now, we will set up your battlefield ships.", io_module.MSG_System);
	io_module.show_message();
	io_module.pause();
	io_module.cls();

	io_module.show_message("Setting up ships:", io_module.MSG_System);
	io_module.show_message();
	confirm = io_module.ask_for_yesno("Place ships authomatically (Y/N): ");
	io_module.cls();
	if (confirm) 
	{
		set_battlefield_authomatically(player_map, false);
		return;
	}
		
	while (player_map.has_unused_ships())
	{
		//get length of a ship
		while (true)
		{
			io_module.cls();
			io_module.show_message("Setting up ships:", io_module.MSG_System);
			io_module.show_message();
			length = io_module.ask_for_int("Enter ship length: ");
			if ((length > 0) && (length <= 4))
				break;
			io_module.show_error("Incorrect ship length! Must be from 1 to 4.");
		}
		if (!player_map.has_unused_ships(length))
		{
			io_module.show_error("All ships with this length are already used.");
			continue;
		}

		//get coords where ship starts
		coords = io_module.ask_for_coords("Enter top left corner ship coordinates: ");
			
		//get position (vertical / horisontal)
		if (length > 1)
			confirm = io_module.ask_for_yesno("Place it vertically? (Y/N): ");
		else
			confirm = true;

		// check possibility
		if (!player_map.add_ship(coords.x, coords.y, length, confirm, false))
			io_module.show_error("Impossible to put a ship this way!");
	}
}


// Player turn. Return true, if player wins.
bool Seabattle_Flow::player_turn()
{
	Coords coords;
	Fire_result result;
	bool next_turn = true;
	bool winned = false;

	while (next_turn)
	{
		next_turn = false;

		io_module.cls();
		io_module.show_message("Your turn:", io_module.MSG_System);
		io_module.show_message();
		coords = io_module.ask_for_coords("Enter coordinates to fire: ");
		result = enemy_map.fire_to_cell(coords.x, coords.y, true);

		switch (result)
		{
		case FIRE_Miss:
			io_module.show_message("You missed!");
			break;

		case FIRE_Miss_destroyed:
			io_module.show_message("You hit to the dead part!");
			break;
			
		case FIRE_Miss_double:
			io_module.show_message("You fire to the air second time!");
			break;

		case FIRE_Hit:
			io_module.show_message("Gotcha!");
			next_turn = true;
			break;

		case FIRE_Hit_drown:
			io_module.show_message("You did it!");
			next_turn = true;
			break;

		case FIRE_Hit_end:
			io_module.show_message("Congratulations! It was the last one!");
			winned = true;
			break;
		}
		io_module.pause();
	}
	return winned;
}


// Enemy turn.
bool Seabattle_Flow::enemy_turn()
{
	Fire_result result;
	Point_to_check temp_point;
	bool next_turn = true;
	bool winned = false;
	int x, y;

	io_module.cls();
	io_module.show_message("Computer turn:", io_module.MSG_System);
	io_module.show_message();
	while (next_turn)
	{
		next_turn = false;
		
		// proceed "seeking" mode
		if (seeking)	
		{
			do
			{
				x = rand() % MAP_SIZE;
				y = rand() % MAP_SIZE;
			} 
			while (!player_map.can_fire_here(x, y));
		}
		// proceed "cartured" mode
		else			
		{
			temp_point = _get_check_point();
			x = temp_point.coords.x;
			y = temp_point.coords.y;
		}
			
		// fire !
		result = player_map.fire_to_cell(x, y, false);
		switch (result)
		{
		case FIRE_Hit:
			next_turn = true;
			if (seeking)
			{
				seeking = false;
				_add_check_points(x, y);
			}
			else
				_proceed_check_point(temp_point);
			io_module.show_computer_turn("Computer successgully hit!", x, y);
			break;

		case FIRE_Hit_drown:
			next_turn = true;
			seeking = true;
			io_module.show_computer_turn("Computer successgully hit and kill your ship!", x, y);
			break;

		case FIRE_Hit_end:
			winned = true;
			io_module.show_computer_turn("Computer won!", x, y);
			break;

		default:
			io_module.show_computer_turn("Computer miss!", x, y);
			break;
		}
	}
	io_module.pause();
	return winned;
}


// Must be call when computer player occasionally hit a player ship.
// Set up possible points and directions to further attacks.
void Seabattle_Flow::_add_check_points(int x, int y)
{
	Point_to_check temp_point;

	check_points.clear();
	for (int direction = 1; direction <= 4; direction++)
		if (player_map.can_fire_here_limited(x + DIRECTIONS_COORDS[direction][0], 
												y + DIRECTIONS_COORDS[direction][1],
												(Directions)direction))
		{
			temp_point.coords.x = x + DIRECTIONS_COORDS[direction][0];
			temp_point.coords.y = y + DIRECTIONS_COORDS[direction][1];
			temp_point.direction = (Directions)direction;
			check_points.push_back(temp_point);
		}
}


//Pop check-point to attack;
Seabattle_Flow::Point_to_check Seabattle_Flow::_get_check_point()
{
	if (!check_points.size())
		throw("Something strange happens... trying to finish off a ship with no checkpoints!");

	Point_to_check point = check_points.back();
	check_points.pop_back();
	return point;
}


//Add another check point, that continues way of "point";
void Seabattle_Flow::_proceed_check_point(Point_to_check point)
{
	point.coords.x += DIRECTIONS_COORDS[point.direction][0];
	point.coords.y += DIRECTIONS_COORDS[point.direction][1];
	if (player_map.can_fire_here_limited(point.coords.x, point.coords.y, point.direction))
		check_points.push_back(point);
}

	
// Finish the game.
void Seabattle_Flow::good_bye()
{
	io_module.show_message("Good Bye!\n\n\n", io_module.MSG_System);
}
	
