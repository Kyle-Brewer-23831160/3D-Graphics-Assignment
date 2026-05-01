#include "TileMap.h"

TileMap::TileMap()
{
	TileMap0.Initialise(TileMap0Layout);
	TileMap1.Initialise(TileMap1Layout);
	TileMap2.Initialise(TileMap2Layout);
	TileMap3.Initialise(TileMap3Layout);
	TileMap4.Initialise(TileMap4Layout);
	TileMap5.Initialise(TileMap5Layout);
	TileMap6.Initialise(TileMap6Layout);
	TileMap7.Initialise(TileMap7Layout);
	TileMap8.Initialise(TileMap8Layout);
	TileMap9.Initialise(TileMap9Layout);

	TileMaps.push_back(TileMap0);
	TileMaps.push_back(TileMap1);
	TileMaps.push_back(TileMap2);
	TileMaps.push_back(TileMap3);
	TileMaps.push_back(TileMap4);
	TileMaps.push_back(TileMap5);
	TileMaps.push_back(TileMap6);
	TileMaps.push_back(TileMap7);
	TileMaps.push_back(TileMap8);
	TileMaps.push_back(TileMap9);
}
