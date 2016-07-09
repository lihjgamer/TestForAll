#include "AStar.h"
#include "map_def.h"
#include "logger.h"
#include <math.h>
using namespace lihj;

lihj_uint16 CAStar::uint_counter = 0;
CAStar::CAStar(void)
{
}

CAStar::~CAStar(void)
{
}

void CAStar::Init(const GameMap* game_map, const lihj_uint16 begin_cell_x, const lihj_uint16 begin_cell_y, const lihj_uint16 end_cell_x, const lihj_uint16 end_cell_y)
{
	m_uint16_begin_cell_index = game_map->uint16_width * begin_cell_y + begin_cell_x;
	m_uint16_end_cell_index   = game_map->uint16_width * end_cell_y   + end_cell_x;
}

void CAStar::Init(const lihj_uint16 begin_map_cell_index, const lihj_uint16 end_map_cell_index)
{
	m_uint16_begin_cell_index = begin_map_cell_index;
	m_uint16_end_cell_index   = end_map_cell_index;
}

bool CAStar::BeginStar(const GameMap* game_map)
{
	bool bok = false;
	path_node default_node ;	// 用于比较使用
	path_node begin_node(m_uint16_begin_cell_index,m_uint16_begin_cell_index);	// 用于比较使用
	path_node end_node(m_uint16_end_cell_index,m_uint16_end_cell_index);	// 用于比较使用
	lihj_uint16 parent_node;

	m_vect_cells_openlist.push_back(begin_node);
	uint_counter ++;
	parent_node = -1; // 开始第一次检索
	while (!m_vect_cells_openlist.empty())
	{
		path_node node;
		//GetSmallestValueF(game_map,parent_node,node);

		GetSmallestValueF(node);
		if (node == end_node)
		{
			bok = true;
			break;
		}

		EraseAndPush(m_vect_cells_openlist,m_vect_cells_closelist,node);

		PushNearCellToOpenList(game_map,node);
		parent_node = node.cur_node;
	}






	return bok;
}

void CAStar::EraseAndPush(VectCells& erase_vector,VectCells& push_vecor,const path_node& node)
{
	for (VectCells::iterator iter = erase_vector.begin();
		iter != erase_vector.end();
		++ iter)
	{
		if ((*iter) == node)
		{
			erase_vector.erase(iter);
			push_vecor.push_back(node);
			break;
		}
	}
}

void CAStar::GetSmallestValueF(const GameMap* game_map,const lihj_uint16& parent_node,path_node& node) const
{
	static const Vector2D vect2d[] = {
		Vector2D(1 , 0),	// 右方
		Vector2D(1 , 1),	// 右下角
		Vector2D(0 , 1),	// 下方
		Vector2D(-1, 1),	// 左下角
		Vector2D(-1, 0),	// 左方
		Vector2D(-1,-1),	// 左上角
		Vector2D(0 ,-1),	// 上方
		Vector2D(1 ,-1),	// 右上角
	};
	path_node default_node;

	Vector2D  tmp_parent_vector2d;
	Vector2D  tmp_vector2d;
	Vector2D  tmp_vector2d_tmp;

	if (parent_node == lihj_uint16(-1))
	{
		node = m_vect_cells_openlist.at(0);
	}
	else
	{
		CellIndexToVector2D(game_map,parent_node,tmp_parent_vector2d);

		for (lihj_uint16 i = 0; i < 8; i ++)
		{
			tmp_vector2d = tmp_parent_vector2d + vect2d[i];

			for (VectCells::const_iterator iter = m_vect_cells_openlist.begin();
				iter != m_vect_cells_openlist.end();
				++ iter)
			{
				CellIndexToVector2D(game_map,(*iter).cur_node,tmp_vector2d_tmp);
				if (tmp_vector2d_tmp == tmp_vector2d && (node == default_node || node.f > (*iter).f))
				{
					node = (*iter);
				}
			}
		}
	}
	//LogInfo("Smallest Node [%d]\n",node.cur_node);
}

void CAStar::GetSmallestValueF(path_node& node) const
{
	path_node default_node;

	for (VectCells::const_iterator iter = m_vect_cells_openlist.begin();
		iter != m_vect_cells_openlist.end();
		++ iter)
	{
		if (node == default_node || node.f > (*iter).f)
		{
			node = (*iter);
		}
	}

	//LogInfo("Smallest Node [%d]\n",node.cur_node);

}

bool CAStar::PushNearCellToOpenList(const GameMap* game_map,const path_node& node)
{
	 static const Vector2D vector2d[] = {
		Vector2D(1 , 0),	// 右方
		Vector2D(1 , 1),	// 右下角
		Vector2D(0 , 1),	// 下方
		Vector2D(-1, 1),	// 左下角
		Vector2D(-1, 0),	// 左方
		Vector2D(-1,-1),	// 左上角
		Vector2D(0 ,-1),	// 上方
		Vector2D(1 ,-1),	// 右上角
	};

	Vector2D cur_pos;
	Vector2D dst_pos;
	CellIndexToVector2D(game_map,node.cur_node,cur_pos);
	CellIndexToVector2D(game_map,m_uint16_end_cell_index,dst_pos);

	Vector2D			tmp_pos;
	lihj_uint16			tmp_index;
	path_node			tmp_node;

	for (lihj_uint16 i = 0; i < 8; i ++)
	{
		tmp_pos = cur_pos + vector2d[i];
		Vector2DToCellIndex(game_map,tmp_pos,tmp_index);
		/*
			非法格子索引
			非法格子坐标（判断两个是因为 放置 将边角处的 格子加入到开放列表中）
			已经在关闭列表里
			不能通行
		*/
		if (!IsValidCellIndex(game_map,tmp_index)
			||!IsValidVector2D(game_map,tmp_pos)
			|| IsInList(m_vect_cells_closelist,tmp_index,tmp_node)
			|| !CanGoThrough(game_map,tmp_index))
		{
			continue;
		}
		
		bool bIn = IsInList(m_vect_cells_openlist,tmp_index,tmp_node);
		if (!bIn)
		{
			tmp_pos = dst_pos - tmp_pos;
			tmp_node.parent_node = node.cur_node;
			tmp_node.cur_node    = tmp_index;
			tmp_node.insert_counter     = uint_counter ++;
			tmp_node.g			 = Vector2D::In_X_Y_Direction(vector2d[i]) ? 10 : 14;
			tmp_node.h			 = (abs(tmp_pos.x) + abs(tmp_pos.y)) ;//+ (lihj_uint16)game_map->mapcell[tmp_index].int32_weights/** 10*/;
			tmp_node.f			 = tmp_node.g + tmp_node.h;
			m_vect_cells_openlist.push_back(tmp_node);
		}
		else
		{
			lihj_uint16 g = Vector2D::In_X_Y_Direction(vector2d[i]) ? 10 : 14;
			// 只有在权值变小的情况下，才更新
			if(node.g + g <= tmp_node.g)
			{
				tmp_node.g = node.g + g;
				tmp_node.f = tmp_node.g + tmp_node.h;
				tmp_node.parent_node = node.cur_node;
				tmp_node.update_counter ++;
				UpdateNode(tmp_node);
			}
		}

		if (tmp_index == m_uint16_end_cell_index)
		{
			return true;
		}
	}
	return false;
}

void CAStar::Vector2DToCellIndex(const GameMap* game_map,const Vector2D& vector2d,lihj_uint16& cell_index) const
{
	cell_index = game_map->uint16_width * vector2d.y + vector2d.x;
}

void CAStar::CellIndexToVector2D(const GameMap* game_map,const lihj_uint16& cell_index,Vector2D& vector2d) const
{
	try
	{
		vector2d.x = cell_index % game_map->uint16_width;
		vector2d.y = cell_index / game_map->uint16_width;
	}
	catch (...)
	{

	}
}

bool CAStar::IsValidVector2D(const GameMap* game_map,const Vector2D& cell_pos)
{
	return cell_pos.x >= 0 && cell_pos.x < game_map->uint16_width 
		&& cell_pos.y >= 0 && cell_pos.y < game_map->uint16_highth;
}

bool CAStar::IsValidCellIndex(const GameMap* game_map,const lihj_uint16& cell_index)
{
	return cell_index >= 0 && cell_index < game_map->uint16_highth * game_map->uint16_width;
}

bool CAStar::IsInList(const VectCells& vect_cells,const lihj_uint16 cell_index,path_node& node)
{
	for (VectCells::const_iterator iter = vect_cells.begin();
		iter != vect_cells.end();
		++ iter)
	{
		if ((*iter).cur_node == cell_index)
		{
			node = (*iter);
			return true;
		}
	}
	return false;
}

void CAStar::UpdateNode(const path_node& node)
{
	for (VectCells::iterator iter = m_vect_cells_openlist.begin();
		iter != m_vect_cells_openlist.end();
		++ iter)
	{
		if (node.cur_node == (*iter).cur_node)
		{
			(*iter) = node;
			return ;
		}
	}
}

void CAStar::GetPath(std::list<lihj_uint16>& list_path_node)
{
	bool bfinish = false;
	lihj_uint16 cur_path_node = m_uint16_end_cell_index;
	list_path_node.push_front(cur_path_node);
	path_node tmp_node;
	while(!bfinish && 
		(IsInList(m_vect_cells_openlist,cur_path_node,tmp_node) || IsInList(m_vect_cells_closelist,cur_path_node,tmp_node) ))
	{
		cur_path_node = tmp_node.parent_node;
		if (cur_path_node != m_uint16_begin_cell_index)
		{
			list_path_node.push_front(tmp_node.parent_node);
		}
		else
		{
			bfinish = true;
		}
	}
}

void CAStar::EndStar()
{
	m_uint16_begin_cell_index = m_uint16_end_cell_index = -1;
	m_vect_cells_openlist.clear();
	m_vect_cells_closelist.clear();
}

void CAStar::PrintCloseList() const
{
	for (VectCells::const_iterator iter = m_vect_cells_closelist.begin();
		iter != m_vect_cells_closelist.end();
		++ iter)
	{
		LogError("%d ",(*iter).cur_node);
	}
	LogError("\n");
}

bool CAStar::InOpenList(const lihj_uint16 cell_index,path_node& node)
{
	return IsInList(m_vect_cells_openlist,cell_index,node);
}

bool CAStar::InCloseList(const lihj_uint16 cell_index,path_node& node)
{
	return IsInList(m_vect_cells_closelist,cell_index,node);
}

bool CAStar::CanGoThrough(const GameMap* game_map,const lihj_uint16 cell_index)
{
	if (IsValidCellIndex(game_map,cell_index))
	{
		return (game_map->mapcell[cell_index].uint8_type & map_attr_obstacle) == 0;
	}
	return false;
}
