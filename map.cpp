//
//   Map class provides all operations with seabattle map
// 
////////////////////////////////////////////////////////////////////


#include <cassert>

#include "map.h"
#include "screen.h"

using namespace seabattle_screen;


// Constructor. Initialize as empty map & ships data.
Map::Map(Owner owner)
{
	m_owner = owner;

	// All cells on map becomes "hidden empty"
	for (int x = 0; x < k_mapSize; x++)
	{
		for (int y = 0; y < k_mapSize; y++)
		{
			m_map[x][y] = CellState::Empty;
		}
	}
		
	// Fulfill ships vacancies
	m_shipPositionsFree[0] = 0;
	m_shipPositionsFree[1] = 4;
	m_shipPositionsFree[2] = 3;
	m_shipPositionsFree[3] = 2;
	m_shipPositionsFree[4] = 1;
}


// Draw battlefield map.
void Map::Draw() const
{
	int start_x = (m_owner == Owner::Enemy) ? k_enemyMapX : k_playerMapX;
	int start_y = (m_owner == Owner::Enemy) ? k_enemyMapY : k_playerMapY;

	PrintString(start_x, start_y, "  a b c d e f g h i j", Colors::LightGrey, Colors::Black);
	for (int y = 0; y < k_mapSize; y++)
	{
		PrintChar(start_x, start_y + 2 + y * 2, 48 + y, Colors::LightGrey, Colors::Black);
		PrintString(start_x + 1, start_y + 1 + y * 2, (char*)k_mapSeparator, Colors::LightBlue, Colors::Black);
		for (int x = 0; x < k_mapSize; x++)
		{
			PrintChar(start_x + x * 2 + 1, start_y + 2 + y * 2, '|', Colors::LightBlue, Colors::Black);
			RedrawCell(x, y, ShipState::Alive);
		}
		PrintChar(start_x + k_mapSize * 2 + 1, start_y + 2 + y * 2, '|', Colors::LightBlue, Colors::Black);
	}
	PrintString(start_x + 1, start_y + k_mapSize * 2 + 1, (char*)k_mapSeparator, Colors::LightBlue, Colors::Black);
}


// Draw single cell of battlefield map.
void Map::RedrawCell(int x, int y, ShipState state) const
{
	char temp;
	Colors fg = Colors::Black, bg = Colors::Black;
	CellState value = m_map[x][y];

	if (m_owner == Owner::Enemy)
	{
		switch (value)
		{
			case CellState::Destroyed:
			{
				temp = 'X';
				fg = Colors::LightRed;
				if (state == ShipState::Drown)
				{
					fg = Colors::LightRed;
					bg = Colors::Magenta;
				}
			}
			break;
			case CellState::Fired:
			{
				temp = 250;
				fg = Colors::LightCyan;
			}
			break;
			default:
			{
				temp = 176;
				fg = Colors::Blue;
			}
			break;
		};
		PrintChar(k_enemyMapX + x * 2 + 2, k_enemyMapY + 2 + y * 2, temp, fg, bg);
	} 
	else
	{
		switch (value)
		{
			case CellState::Destroyed:
			{
				temp = 'X';
				fg = Colors::LightRed;
			}
			break;
			case CellState::Ship:
			{
				temp = '*';
				bg = Colors::Yellow;
				fg = Colors::LightYellow;
			}
			break;
			case CellState::Fired:
			{
				temp = 250;
				fg = Colors::LightMagenta;
			}
			break;
			default:
			{
				temp = ' ';
				fg = Colors::Black;
			}
			break;
		};
		PrintChar(k_playerMapX  + x * 2 + 2, k_playerMapY + 2 + y * 2, temp, fg, bg);
	}
}


// Add a new ship to the map.
bool Map::AddShip(int x, int y, int length, Alignment align)
{
	// Check, if it possible to add another ship to battle map
	const bool isValidX = (x >= 0) && (x < k_mapSize);
	const bool isValidY = (y >= 0) && (y < k_mapSize);
	if (!isValidX || !isValidY)
	{
		return false;
	}
	if (align == Alignment::Vertical)
	{
		for (int dy = 0; dy < length; dy++)
		{
			if ((y + dy >= k_mapSize) || (!CanDeployShipHere(x, y + dy)))
			{
				return false;
			}
		}
	}
	else
	{
		for (int dx = 0; dx < length; dx++)
		{
			if ((x + dx >= k_mapSize) || (!CanDeployShipHere(x + dx, y)))
			{
				return false;
			}
		}
	}

	// Adding ship
	Ship new_ship;
	new_ship.length = length;
	for (int q = 0; q < length; q++)
		if (align == Alignment::Vertical)
		{
			new_ship.coords[q].x = x;
			new_ship.coords[q].y = y + q;
			m_map[x][y + q] = CellState::Ship;
			if (m_owner == Owner::Player)
			{
				RedrawCell(x, y + q, ShipState::Alive);
			}
		}
		else
		{
			new_ship.coords[q].x = x + q;
			new_ship.coords[q].y = y;
			m_map[x + q][y] = CellState::Ship;
			if (m_owner == Owner::Player)
			{
				RedrawCell(x + q, y, ShipState::Alive);
			}
		}
	m_ships.push_back(new_ship);
	m_shipPositionsFree[length]--;
	return true;
}


// Check, if this map cell is acceptible to deploy new ship part here. True, if it's possible.
bool Map::CanDeployShipHere(int x, int y) const
{
	int tempX, tempY;
	for (int dx = -1; dx <= 1; dx++)
	{
		for (int dy = -1; dy <= 1; dy++)
		{
			tempX = x + dx;
			tempY = y + dy;
			if (!IsValidCoord(tempX, tempY))
			{
				continue;
			}
			if (m_map[tempX][tempY] == CellState::Ship)
			{
				return false;
			}
		}
	}
	return true;
}


// Check, if this map cell is acceptible to random fire into it. True, if it's possible.
bool Map::CanFireHere(int x, int y) const
{
	// there is no sence to fire to already fired cell
	if (m_map[x][y] == CellState::Fired)
	{
		return false;
	}

	// there is no sence to random fire to cell near destroyed ship
	int tempX, tempY;
	for (int dx = -1; dx <= 1; dx++)
		for (int dy = -1; dy <= 1; dy++)
		{
			tempX = x + dx;
			tempY = y + dy;
			if (!IsValidCoord(tempX, tempY))
			{
				continue;
			}
			if (m_map[tempX][tempY] == CellState::Destroyed)
			{
				return false;
			}
		}
	return true;
}


// // Check, if this map cell is acceptible to fire into for finishing injured ship off.
bool Map::CanFireHereDirectional(int x, int y, Directions direction_check) const
{
	// here we have to check bounds before proceeding
	if (!IsValidCoord(x, y))
	{
		return false;
	}

	// there is no sence to fire to already fired cell
	if (m_map[x][y] == CellState::Fired)
	{
		return false;
	}

	int low_dx = -1, high_dx = 1, low_dy = -1, high_dy = 1;
	switch (direction_check)
	{
		case Directions::Up:
		{
			high_dy = -1;
		}
		break;
		case Directions::Down:
		{
			low_dy = 1;
		}
		break;
		case Directions::Left:
		{
			high_dx = -1;
		}
		break;
		case Directions::Right:
		{
			low_dx = 1;
		}
		break;
	}

	// there is no sence to random fire to cell near destroyed ship
	int tempX, tempY;
	for (int dx = low_dx; dx <= high_dx; dx++)
	{ 
		for (int dy = low_dy; dy <= high_dy; dy++)
		{
			tempX = x + dx;
			tempY = y + dy;
			if (!IsValidCoord(tempX, tempY))
			{
				continue;
			}
			if (m_map[tempX][tempY] == CellState::Destroyed)
			{
				return false;
			}
		}
	}
	return true;
}


// Check, if there is a possibiliry to add another ship with chosen length.
bool Map::HasShipsToDeploy(int length) const
{
	return m_shipPositionsFree[length] != 0;
}


// Check, if there is a possibiliry to add another ship of any length.
bool Map::HasShipsToDeploy() const
{
	return m_ships.size() < 10;
}


// Calculate firing result and make appropriate changes on map. 
Map::FireResult Map::FireToCell(int x, int y)
{
	switch (m_map[x][y])
	{
		case CellState::Destroyed:
		{
			return FireResult::MissDestroyed;
		}
		case CellState::Fired:
		{
			return FireResult::MissDouble;
		}
		case CellState::Empty:
		{
			m_map[x][y] = CellState::Fired;
			RedrawCell(x, y, ShipState::Alive);
			return FireResult::Miss;
		}
		case CellState::Ship:
		{

			m_map[x][y] = CellState::Destroyed;
			RedrawCell(x, y, ShipState::Alive);

			// search for the ship by coords
			bool break_flag = false;
			auto it = m_ships.begin();
			while (!break_flag && (it != m_ships.end()))
			{
				for (int q = 0; q < it->length; q++)
				{
					if ((it->coords[q].x == x) && (it->coords[q].y == y))
					{
						break_flag = true;
						it->coords[q].ok = false;
						break;
					}
				}
				if (break_flag)
				{
					break;
				}
				it++;
			}

			// There is no ship accorded with this cell?!?
			assert(break_flag);
			

			// Check, if this ship is drowned
			bool drown_flag = true;
			for (int q = 0; q < it->length; q++)
			{
				if (it->coords[q].ok)
				{
					drown_flag = false;
					break;
				}
			}
			if (!drown_flag)
			{
				return FireResult::Hit;
			}

			// Redraw drowned ship
			for (int q = 0; q < it->length; q++)
			{
				RedrawCell(it->coords[q].x, it->coords[q].y, ShipState::Drown);
			}

			// Remove drowned ship from list and check, if any ships remain
			m_ships.erase(it);
			if (m_ships.size())
			{
				return FireResult::HitDrown;
			}
			else
			{
				return FireResult::HitEnd;
			}
		}
		default:
		{
			
		}
	}
}


bool Map::IsValidCoord(int x, int y) const
{
	bool isValidX = (x >= 0) && (x < k_mapSize);
	bool isValidY = (y >= 0) && (y < k_mapSize);
	if (isValidX && isValidY)
	{
		return true;
	}
	else
	{
		return false;
	}
}


const char *Map::k_mapSeparator = "+-+-+-+-+-+-+-+-+-+-+";

