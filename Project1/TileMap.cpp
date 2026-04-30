#include "TileMap.h"

TileMap::TileMap()
{
	TileMap1.Initialise(TileMap1Layout);
	TileMap2.Initialise(TileMap2Layout);
	TileMap3.Initialise(TileMap3Layout);
	TileMap4.Initialise(TileMap4Layout);

	TileMaps.push_back(TileMap1);
	TileMaps.push_back(TileMap2);
	TileMaps.push_back(TileMap3);
	TileMaps.push_back(TileMap4);
}
