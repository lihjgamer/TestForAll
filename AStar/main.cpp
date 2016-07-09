#include <stdio.h>
#include <stdlib.h>

#include "vld.h"
#include "map_def.h"
#include "GameMap.h"
#include "AStar.h"
#include "logger.h"
using namespace lihj;
int main(int argc,char** argv)
{
	CGameMap gamemap;
	gamemap.Init();

	GameMap* game_map = gamemap.GetGameMap();
	
	CAStar star;
	star.Init(0,244);
	std::list<lihj_uint16> pathlist;
	bool bok = star.BeginStar(game_map);
	if (bok)
	{
		//star.PrintCloseList();
		star.GetPath(pathlist);
	}

	//std::list<lihj_uint16>::iterator iter = pathlist.begin();

	for (lihj_uint16 y = 0; y < game_map->uint16_highth; y ++ )
	{
		for (lihj_uint16 x = 0; x < game_map->uint16_width; x ++)
		{
			lihj_uint16 cell_index = x + y * game_map->uint16_width;
			path_node node;
			bool inopenlist  = star.InOpenList(cell_index,node);
			bool incloselist = inopenlist || star.InCloseList(cell_index,node);
			if (inopenlist || incloselist)
			{
				bool bfind = false;
				for (std::list<lihj_uint16>::const_iterator iter = pathlist.begin();
					iter != pathlist.end();
					++ iter)
				{
					if ((*iter) == cell_index)
					{
						bfind = true;
						break;
					}
				}

				if (bfind)
				{
					LogInfo("[%04u %04u]",cell_index,node.insert_counter);
				}
				else if(inopenlist)
				{
					LogDebug("[%04u %04u]",cell_index,node.insert_counter);
				}
				else
				{
					LogFatal("[%04u %04u]",cell_index,node.insert_counter);
				}
			}
			else
			{
				if (game_map->mapcell[cell_index].uint8_type == 2)
				{
					LogDebug1("[%04u %04u]",cell_index ,0);
				}
				else
				{
					LogError("[%04u %04u]",cell_index ,0);
				}
			}
		}
		LogDebug("\n");
	}	

	star.EndStar();
	gamemap.Uninit();

	system("pause");
	return 0;
}