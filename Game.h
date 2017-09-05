//
//   Header: Game flow class module.
// 
//////////////////////////////////////////


#pragma once

#include "map.h"
#include "console.h"


// ========================================  GAME Class  ========================================= //

// Main loop class for "Sea Battle" game.
class Game
{
public:

	//  -----METHODS-----
	Game();
	void							PreparePlayerMap();
	void							SetMapAutomate(Map &map, Map::Owner owner);
	bool							PlayerTurn();
	bool							EnemyTurn();
	void							GameOver();

	// ----------------------------------------------------

private:

	// -----STRUCTS----- 

	struct Point_to_check 
	{
		Console::Coords		coords;
		Map::Directions		direction;
	};

	//  -----METHODS----- 

	void							AddPointsToCheck(int x, int y);
	void							ProceedPointsToCheck(Point_to_check point);
	Point_to_check					GetPointsToCheck();

	// -----MEMBER VARS----- 

	bool							m_isSeeking;
	Map								m_playerMap, m_enemyMap;
	Console							m_console;
	std::vector <Point_to_check>	m_pointsToCheck;

	// -----CONST-----

	static const int				k_directions_coords[5][2];

};
