#pragma once
#include "AStar.h"

struct GameMap;
class CGameMap
{
public:
	bool Init(lihj_uint16 init_width = 10,lihj_uint16 init_height = 50);
	bool Uninit();

	GameMap* GetGameMap();

public:
	CGameMap(void);
	~CGameMap(void);

private:
	GameMap* m_pGameMap;

	//CAStar   m_AStar;
};
