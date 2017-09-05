//
//   Game flow class module.
// 
//////////////////////////////////////////


#include <cassert>

#include "game.h"


// Constructor: main game loop.
Game::Game() :
m_enemyMap(Map::Owner::Enemy),
m_playerMap(Map::Owner::Player)
{
	m_playerMap.Draw();
	m_enemyMap.Draw();
	PreparePlayerMap();
	SetMapAutomate(m_enemyMap, Map::Owner::Enemy);
	m_isSeeking = true;

	while (true)
	{
		if (PlayerTurn())
		{
			m_console.ShowMessage("Congratulations! You won!", Console::MessageType::System);
			break;
		}

		if (EnemyTurn())
		{
			m_console.ShowMessage("It's not your day, pal!", Console::MessageType::System);
			break;
		}
	};
	GameOver();
};


//Fill some battlefield with ships authomatically.
void Game::SetMapAutomate(Map &map, Map::Owner owner) {
	int x, y;
	Map::Alignment align;
	
	for (int length = 4; length; --length)
	{
		for (int q = 0; q < 5 - length; ++q)
		{
			do
			{
				x = rand() % Map::k_mapSize;
				y = rand() % Map::k_mapSize;
				align = (Map::Alignment)(rand() % 2);
			} while (!map.AddShip(x, y, length, align));
		}
	}
}


// Fulfill player battlefield with ships.
void Game::PreparePlayerMap()
{
	int length = 0;
	Console::Coords coords;
		
	m_console.ShowMessage("Hello! This is Sea Battle game.", Console::MessageType::System);
	m_console.ShowMessage("If you run it, it means, you want to play it!", Console::MessageType::System);
	m_console.ShowMessage("For now, we will set up your battlefield ships.", Console::MessageType::System);
	m_console.ShowEmptyRow();
	m_console.Pause();
	m_console.ClearConsole();

	m_console.ShowMessage("Setting up ships:", Console::MessageType::System);
	m_console.ShowEmptyRow();

	bool confirm;
	confirm = m_console.AskForYesNo("Place ships authomatically (Y/N): ");
	m_console.ClearConsole();
	if (confirm) 
	{
		SetMapAutomate(m_playerMap, Map::Owner::Player);
		return;
	}
		
	while (m_playerMap.HasShipsToDeploy())
	{
		//get length of a ship
		while (true)
		{
			m_console.ClearConsole();
			m_console.ShowMessage("Setting up ships:", Console::MessageType::System);
			m_console.ShowEmptyRow();
			length = m_console.AskForInt("Enter ship length: ");
			if ((length > 0) && (length <= 4))
			{
				break;
			}
			m_console.ShowError("Incorrect ship length! Must be from 1 to 4.");
		}
		if (!m_playerMap.HasShipsToDeploy(length))
		{
			m_console.ShowError("All ships with this length are already used.");
			continue;
		}

		//get coords where ship starts
		coords = m_console.AskForCoords("Enter top left corner ship coordinates: ");
			
		//get alignment: confirmation means vertical, else horisontal
		if (length > 1)
		{
			confirm = m_console.AskForYesNo("Place it vertically? (Y/N): ");
		}
		else
		{
			confirm = true;
		}

		// check possibility
		if (!m_playerMap.AddShip(coords.x, coords.y, length, (Map::Alignment)confirm))
		{
			m_console.ShowError("Impossible to put a ship this way!");
		}
			
	}
}


// Proceed player turn. Return true, if player wins.
bool Game::PlayerTurn()
{
	Console::Coords coords;
	Map::FireResult result;
	bool next_turn = true;
	bool winned = false;

	while (next_turn)
	{
		next_turn = false;
		m_console.ClearConsole();
		m_console.ShowMessage("Your turn:", Console::MessageType::System);
		m_console.ShowEmptyRow();
		coords = m_console.AskForCoords("Enter coordinates to fire: ");
		result = m_enemyMap.FireToCell(coords.x, coords.y);

		switch (result)
		{
			case Map::FireResult::Miss:
			{
				m_console.ShowMessage("You missed!");
			}
			break;
			case Map::FireResult::MissDestroyed:
			{
				m_console.ShowMessage("You hit to the dead part!");
			}
			break;
			case Map::FireResult::MissDouble:
			{
				m_console.ShowMessage("You fire to the air second time!");
			}
			break;
			case Map::FireResult::Hit:
			{
				m_console.ShowMessage("Gotcha!");
				next_turn = true;
			}
			break;
			case Map::FireResult::HitDrown:
			{
				m_console.ShowMessage("You did it!");
				next_turn = true;
			}
			break;
			case Map::FireResult::HitEnd:
			{
				m_console.ShowMessage("Congratulations! It was the last one!");
				winned = true;
			}
			break;
			default:
			{
			}
			break;
		}
		m_console.Pause();
	}
	return winned;
}


// Proceed enemy turn. Return true, if computer wins.
bool Game::EnemyTurn()
{
	Map::FireResult result;
	Point_to_check temp_point;
	bool next_turn = true;
	bool winned = false;
	int x, y;

	m_console.ClearConsole();
	m_console.ShowMessage("Computer turn:", Console::MessageType::System);
	m_console.ShowEmptyRow();
	while (next_turn)
	{
		next_turn = false;
		
		// proceed "seeking" mode
		if (m_isSeeking)
		{
			do
			{
				x = rand() % Map::k_mapSize;
				y = rand() % Map::k_mapSize;
			} 
			while (!m_playerMap.CanFireHere(x, y));
		}
		// proceed "cartured" mode
		else			
		{
			temp_point = GetPointsToCheck();
			x = temp_point.coords.x;
			y = temp_point.coords.y;
		}
			
		// fire !
		result = m_playerMap.FireToCell(x, y);
		switch (result)
		{
			case Map::FireResult::Hit:
			{
				next_turn = true;
				if (m_isSeeking)
				{
					m_isSeeking = false;
					AddPointsToCheck(x, y);
				}
				else
					ProceedPointsToCheck(temp_point);
				m_console.ShowComputerTurn("Computer successgully hit!", x, y);
			}
			break;
			case Map::FireResult::HitDrown:
			{
				next_turn = true;
				m_isSeeking = true;
				m_console.ShowComputerTurn("Computer successgully hit and kill your ship!", x, y);
			}
			break;
			case Map::FireResult::HitEnd:
			{
				winned = true;
				m_console.ShowComputerTurn("Computer won!", x, y);
			}
			break;
			default:
			{
				m_console.ShowComputerTurn("Computer miss!", x, y);
			}
			break;
		}
	}
	m_console.Pause();
	return winned;
}


// Must be call when computer player occasionally hit a player ship.
// Set up possible points and directions to further attacks.
void Game::AddPointsToCheck(int x, int y)
{
	Point_to_check temp_point;

	m_pointsToCheck.clear();
	for (int direction = 1; direction <= 4; direction++)
	{
		if (m_playerMap.CanFireHereDirectional(
			x + k_directions_coords[direction][0],
			y + k_directions_coords[direction][1],
			static_cast <Map::Directions> (direction)))
		{
			temp_point.coords.x = x + k_directions_coords[direction][0];
			temp_point.coords.y = y + k_directions_coords[direction][1];
			temp_point.direction = static_cast <Map::Directions> (direction);
			m_pointsToCheck.push_back(temp_point);
		}
	}
}


//Pop "check"-point to attack.
Game::Point_to_check Game::GetPointsToCheck()
{
	// Something strange happens here, if we trying to finish off a ship with no checkpoints!
	assert(m_pointsToCheck.size());

	Point_to_check point = m_pointsToCheck.back();
	m_pointsToCheck.pop_back();
	return point;
}


//Add another point to check, that continues way of "point";
void Game::ProceedPointsToCheck(Point_to_check point)
{
	point.coords.x += k_directions_coords[(int)point.direction][0];
	point.coords.y += k_directions_coords[(int)point.direction][1];
	if (m_playerMap.CanFireHereDirectional(point.coords.x, point.coords.y, point.direction))
	{
		m_pointsToCheck.push_back(point);
	}
}

	
// Game-over proceedings.
void Game::GameOver()
{
	m_console.ShowMessage("Good Bye!\n\n\n", Console::MessageType::System);
}
	

const int Game::k_directions_coords[5][2] = {{0, 0}, {0, -1}, {0, 1}, {-1, 0}, {1, 0}};

