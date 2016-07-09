#include "GameMap.h"
#include "GameMapFactory.h"
#include <stdio.h>

CGameMap::CGameMap(void)
{
	m_pGameMap = NULL;
}

CGameMap::~CGameMap(void)
{
}

bool CGameMap::Init(lihj_uint16 init_width,lihj_uint16 init_height)
{

	m_pGameMap = CGameMapFactory::CreateGameMap(init_width,init_height);
	if (m_pGameMap == NULL)
	{
		return false;
	}
	
	for (lihj_uint16 i = 0; i < init_height; i ++)
	{
		for (lihj_uint16 j = 0; j < init_width; j ++)
		{
			lihj_uint16 cell_index = i * init_width + j;
			if ((j == init_width / 2 && (i != 0 && i != init_height -1))
				|| ( i == init_height / 2 && (j != 0 && j != init_width -1)))
			{
				CGameMapFactory::SetMapCellAttr(m_pGameMap,cell_index,map_attr_obstacle,1000);
			}
			else
			{
				CGameMapFactory::SetMapCellAttr(m_pGameMap,cell_index,map_attr_safe,10);
			}
		}
	}

	return true;
}

bool CGameMap::Uninit()
{
	CGameMapFactory::DestoryGameMap(m_pGameMap);
	return true;
}

GameMap* CGameMap::GetGameMap()
{
	return m_pGameMap;
}
