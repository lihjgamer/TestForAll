#pragma once
#include "lihj_type.h"
#include "map_def.h"
//struct  GameMap;
class CAStar
{
public:
	void Init(const GameMap* game_map,
		const lihj_uint16 begin_cell_x,
		const lihj_uint16 begin_cell_y,
		const lihj_uint16 end_cell_x,
		const lihj_uint16 end_cell_y);
	void Init(const lihj_uint16 begin_map_cell_index,
		const lihj_uint16 end_map_cell_index);

	bool BeginStar(const GameMap* game_map);

	void GetPath(std::list<lihj_uint16>& list_path_node);

	void EndStar();

public:

	bool InOpenList(const lihj_uint16 cell_index,path_node& node);
	bool InCloseList(const lihj_uint16 cell_index,path_node& node);

	void PrintCloseList() const;


public:
	CAStar(void);
	~CAStar(void);
private:

	typedef std::vector<path_node> VectCells;


	void GetSmallestValueF(const GameMap* game_map,const lihj_uint16& parent_node,path_node& node) const;
	void GetSmallestValueF(path_node& node) const;

	void EraseAndPush(VectCells& erase_vector,VectCells& push_vecor,const path_node& node);
	bool PushNearCellToOpenList(const GameMap* game_map,const path_node& node);

	void Vector2DToCellIndex(const GameMap* game_map,const Vector2D& vector2d,lihj_uint16& cell_index) const;
	void CellIndexToVector2D(const GameMap* game_map,const lihj_uint16& cell_index,Vector2D& vector2d) const;
	
	bool IsValidVector2D(const GameMap* game_map,const Vector2D& cell_pos);
	bool IsValidCellIndex(const GameMap* game_map,const lihj_uint16& cell_index);

	bool IsInList(const VectCells& vect_cells,const lihj_uint16 cell_index,path_node& node);
	void UpdateNode(const path_node& node);

	virtual bool CanGoThrough(const GameMap* game_map,const lihj_uint16 cell_index);


private:

	lihj_uint16 m_uint16_begin_cell_index;
	lihj_uint16 m_uint16_end_cell_index;


	VectCells	m_vect_cells_openlist;
	VectCells	m_vect_cells_closelist;

	static lihj_uint16 uint_counter;
};
