#pragma once
#include "lihj_type.h"
struct GameMap;
class CGameMapFactory
{
public:
	static GameMap* CreateGameMap(lihj_uint16 width,lihj_uint16 highth);
	static void     DestoryGameMap(GameMap* game_map);

	static void     SetMapCellAttr(GameMap* game_map,const lihj_uint16 map_cell_index,const lihj_uint8 cell_type,const lihj_int32 cell_weights);
	static void     GetMapCellAttr(GameMap* game_map,const lihj_uint16 map_cell_index,lihj_uint8 &cell_type,lihj_int32 &cell_weights);
public:
	CGameMapFactory(void);
	~CGameMapFactory(void);
};
