//
//   Header: Map class provides all operations with seabattle map
// 
///////////////////////////////////////////////////////////////////////////


#pragma once

#include <vector>


// =========================================  MAP Class  ========================================= //

// Provides all operations with battlefield map, including
// "firing", operations with ships and various checking.
class Map
{
public:

	// -----ENUM -----

	enum class FireResult		{ Miss, MissDouble, MissDestroyed, Hit, HitDrown, HitEnd };
	enum class Directions		{ Undefined, Up, Down, Left, Right };
	enum class Alignment		{ Horizontal, Vertical };
	enum class Owner			{ Player, Enemy };
	enum class ShipState		{ Alive, Drown };

	// -----METHODS----- 

	Map(Owner owner);
	void						Draw() const;
	void						RedrawCell(int x, int y, ShipState state) const;
	FireResult					FireToCell(int x, int y);
	bool						AddShip(int x, int y, int length, Alignment align);
	bool						HasShipsToDeploy(int length) const;
	bool						HasShipsToDeploy() const;
	bool						CanFireHere(int x, int y) const;
	bool						CanFireHereDirectional(int x, int y, Directions direction_check) const;

	// -----CONST-----

	static const int			k_mapSize = 10;

	// ----------------------------------------------------

private:

	// -----ENUM-----

	enum class CellState		{ Empty, Destroyed, Fired, Ship };

	// -----STRUCT-----

	struct ShipBlock
	{
		int			x = -1;
		int			y = -1;
		bool		ok = true;
	};

	struct Ship
	{
		int			length = 0;
		ShipBlock	coords[4];
	};

	// -----METHODS-----

	bool						CanDeployShipHere(int x, int y) const;
	bool						IsValidCoord(int x, int y) const;

	// -----MEMBER VARS-----

	int							m_shipPositionsFree[5];
	Owner						m_owner;
	CellState					m_map[k_mapSize][k_mapSize];
	std::vector <Ship>			m_ships;
	
	// -----CONST-----

	static const int 			k_playerMapX = 10;
	static const int 			k_playerMapY = 2;
	static const int 			k_enemyMapX = 45;
	static const int 			k_enemyMapY = 2;
	static const char 			*k_mapSeparator;
		
};