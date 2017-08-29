// MAP class definition

#include "map.h"
#include "screen.h"

using namespace seabattle_screen;


// Constructor. Initialize as empty map & ships data.
Map::Map()
{
	// All cells on map becomes "hidden empty"
	for (int x = 0; x < MAP_SIZE; x++)
		for (int y = 0; y < MAP_SIZE; y++)
			map[x][y] = CELL_Empty;
		
	// Fulfill ships vacancies
	ship_positions_free[0] = 0;
	ship_positions_free[1] = 4;
	ship_positions_free[2] = 3;
	ship_positions_free[3] = 2;
	ship_positions_free[4] = 1;
}


// Draw map.
void Map::draw_map(bool hidden)
{
	int start_x = hidden ? ENEMY_MAP_X : PLAYER_MAP_X;
	int start_y = hidden ? ENEMY_MAP_Y : PLAYER_MAP_Y;

	outtext(start_x, start_y, "  a b c d e f g h i j", LightGrey);
	for (int y = 0; y < MAP_SIZE; y++)
	{
		outtext(start_x, start_y + 2 + y*2, 48 + y, LightGrey);
		outtext(start_x + 1, start_y + 1 + y*2, (char*)MAP_SEPARATOR, LightBlue);
		for (int x = 0; x < MAP_SIZE; x++)
		{
			outtext(start_x + x*2 + 1, start_y + 2 + y*2, '|', LightBlue);
			renew_map_cell(x, y, hidden);
		}
		outtext(start_x + MAP_SIZE * 2 + 1, start_y + 2 + y * 2, '|', LightBlue);
	}
	outtext(start_x + 1, start_y + MAP_SIZE*2 + 1, (char*)MAP_SEPARATOR, LightBlue);
}


// Draw single cell of map.
void Map::renew_map_cell(int x, int y, bool hidden, bool draw_drown)
{
	char temp;
	Colors fg = Black, bg = Black;
	Map_cell value = map[x][y];

	if (hidden)
	{
		switch (value)
		{
		case CELL_Destroyed:
			temp = 'X';
			fg = LightRed;
			if (draw_drown)
			{
				fg = LightRed;
				bg = Magenta;
			}
			break;
		case CELL_Fired:
			temp = 250;
			fg = LightCyan;
			break;
		default:
			temp = 176;
			fg = Blue;
			break;
		};
		outtext(ENEMY_MAP_X + x * 2 + 2, ENEMY_MAP_Y + 2 + y * 2, temp, fg, bg);
	} 
	else
	{
		switch (value)
		{
		case CELL_Destroyed:
			temp = 'X';
			fg = LightRed;
			break;
		case CELL_Ship:
			temp = '*';
			bg = Yellow;
			fg = LightYellow;
			break;
		case CELL_Fired:
			temp = 250;
			fg = LightMagenta;
			break;
		default:
			temp = ' ';
			fg = Black;
			break;
		};
		outtext(PLAYER_MAP_X  + x * 2 + 2, PLAYER_MAP_Y + 2 + y * 2, temp, fg, bg);
	}
}


// Adding ship to battlemap
bool Map::add_ship(int x, int y, int length, bool vertical, bool silent)
{
	// Check, if it possible to add another ship to battle map
	if ((x < 0) || (x >= MAP_SIZE) || (y < 0) || (y >= MAP_SIZE))
		return false;
	if (vertical)
	{
		for (int dy = 0; dy < length; dy++)
			if ((y + dy >= MAP_SIZE) || (!_can_put_ship_here(x, y + dy)))
				return false;
	}
	else
	{
		for (int dx = 0; dx < length; dx++)
			if ((x + dx >= MAP_SIZE) || (!_can_put_ship_here(x + dx, y)))
				return false;
	}

	// Adding ship
	Ship new_ship;
	new_ship.length = length;
	for (int q = 0; q < length; q++)
		if (vertical)
		{
			new_ship.coords[q].x = x;
			new_ship.coords[q].y = y + q;
			map[x][y + q] = CELL_Ship;
			if (!silent)
				renew_map_cell(x, y + q, false);
		}
		else
		{
			new_ship.coords[q].x = x + q;
			new_ship.coords[q].y = y;
			map[x + q][y] = CELL_Ship;
			if (!silent)
				renew_map_cell(x + q, y, false);
		}
	ships.push_back(new_ship);
	ship_positions_free[length]--;

	return true;
}


// Check, if this cell is OK to get new ship part into
bool Map::_can_put_ship_here(int x, int y)
{
	for (int dx = -1; dx <= 1; dx++)
		for (int dy = -1; dy <= 1; dy++)
		{
			if ((x + dx < 0) || (x + dx >= MAP_SIZE) || (y + dy < 0) || (y + dy >= MAP_SIZE))
				continue;
			if (map[x + dx][y + dy] == CELL_Ship)
				return false;
		}
	return true;
}


// Check, if this cell is good to random fire into it
bool Map::can_fire_here(int x, int y)
{
	// there is no sence to fire to already fired cell
	if (map[x][y] == CELL_Fired)
		return false;

	// there is no sence to random fire to cell near destroyed ship
	for (int dx = -1; dx <= 1; dx++)
		for (int dy = -1; dy <= 1; dy++)
		{
			if ((x + dx < 0) || (x + dx >= MAP_SIZE) || (y + dy < 0) || (y + dy >= MAP_SIZE))
				continue;
			if (map[x + dx][y + dy] == CELL_Destroyed)
				return false;
		}
	return true;
}


// // Check, if this cell is good to fire for finish off.
bool Map::can_fire_here_limited(int x, int y, Directions direction_check)
{
	// here we have to check bounds before proceeding
	if ((x < 0) || (x >= MAP_SIZE) || (y < 0) || (y >= MAP_SIZE))
		return false;

	// there is no sence to fire to already fired cell
	if (map[x][y] == CELL_Fired)
		return false;

	int low_dx = -1, high_dx = 1, low_dy = -1, high_dy = 1;
	switch (direction_check)
	{
	case DIR_UP:
		high_dy = -1;
		break;
	case DIR_DOWN:
		low_dy = 1;
		break;
	case DIR_LEFT:
		high_dx = -1;
		break;
	case DIR_RIGHT:
		low_dx = 1;
		break;
	}

	// there is no sence to random fire to cell near destroyed ship
	for (int dx = low_dx; dx <= high_dx; dx++)
		for (int dy = low_dy; dy <= high_dy; dy++)
		{
			if ((x + dx < 0) || (x + dx >= MAP_SIZE) || (y + dy < 0) || (y + dy >= MAP_SIZE))
				continue;
			if (map[x + dx][y + dy] == CELL_Destroyed)
				return false;
		}
	return true;
}


// Check, if there is a possibiliry to add another ship.
// If length defined, return "true" if there are free positions of ships with defined length.
// Else (if undefined or = -1 , return "true" if there is at least one position at all.
bool Map::has_unused_ships(int length)
{
	// Whole vacancies
	if (length == -1)
		return ships.size() < 10;
	else
		return ship_positions_free[length] != 0;
}


// Calculate and fix in data firing result. 
Fire_result Map::fire_to_cell(int x, int y, bool hidden)
{
	switch (map[x][y])
	{
	case CELL_Destroyed:
		return FIRE_Miss_destroyed;

	case CELL_Fired:
		return FIRE_Miss_double;

	case CELL_Empty:
		map[x][y] = CELL_Fired;
		renew_map_cell(x, y, hidden);
		return FIRE_Miss;

	case CELL_Ship:
		map[x][y] = CELL_Destroyed;
		renew_map_cell(x, y, hidden);

		// search for the ship by coords
		bool break_flag = false;
		auto it = ships.begin();
		while (!break_flag && (it != ships.end()))
		{
			for (int q = 0; q < it->length; ++q)
				if ((it->coords[q].x == x) && (it->coords[q].y == y))
				{
					break_flag = true;
					it->coords[q].ok = false;
					break;
				}
			if (break_flag)
				break;
			++it;
		}
		if (!break_flag)
			throw("There is no ship accorded with this cell?!?");

		// Check, if this ship is drowned
		bool drown_flag = true;
		for (int q = 0; q < it->length; ++q)
			if (it->coords[q].ok) 
			{
				drown_flag = false;
				break;
			}
		if (!drown_flag)
			return FIRE_Hit;
		
		// Redraw drowned ship
		for (int q = 0; q < it->length; ++q)
			renew_map_cell(it->coords[q].x, it->coords[q].y, hidden, true);

		// Remove drowned ship from list and check, if any ships remain
		ships.erase(it);
		if (ships.size())
			return FIRE_Hit_drown;
		else
			return FIRE_Hit_end;
	}
}

