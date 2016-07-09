#include "GameMapFactory.h"
#include "map_def.h"
#include <stdio.h>

CGameMapFactory::CGameMapFactory(void)
{
}

CGameMapFactory::~CGameMapFactory(void)
{
}

GameMap* CGameMapFactory::CreateGameMap(lihj_uint16 width,lihj_uint16 highth)
{
	GameMap* pgm = new GameMap;
	pgm->uint16_width = width;
	pgm->uint16_highth = highth;
	pgm->mapcell = new MapCell[ width * highth];
	return pgm;
}

void CGameMapFactory::DestoryGameMap(GameMap* game_map)
{
	delete[] game_map->mapcell;
	delete game_map;
	game_map = NULL;
}

void CGameMapFactory::SetMapCellAttr(GameMap* game_map,const lihj_uint16 map_cell_index,const lihj_uint8 cell_type,const lihj_int32 cell_weights)
{
	if (game_map != NULL && map_cell_index >= 0 && map_cell_index < game_map->uint16_width * game_map->uint16_highth)
	{
		game_map->mapcell[map_cell_index].uint8_type     = cell_type;
		game_map->mapcell[map_cell_index].int32_weights = cell_weights;
	}
}

void CGameMapFactory::GetMapCellAttr(GameMap* game_map,const lihj_uint16 map_cell_index,lihj_uint8 &cell_type,lihj_int32 &cell_weights)
{
	if (game_map != NULL && map_cell_index >= 0 && map_cell_index < game_map->uint16_width * game_map->uint16_highth)
	{
		cell_type    = game_map->mapcell[map_cell_index].uint8_type; 
		cell_weights = game_map->mapcell[map_cell_index].int32_weights;
	}
}
