#ifndef _MAP_DEF_H_
#define _MAP_DEF_H_

#include "lihj_type.h"

typedef enum enumMapCellType
{
	map_attr_none = 0,
	map_attr_safe = 1,
	map_attr_obstacle = 2,
};

typedef struct structMapCell
{
	lihj_uint8 	uint8_type;		// 类型
	lihj_int32  int32_weights;	// 权值
}MapCell,*PMapCell;

typedef struct GameMap
{
	lihj_uint16 uint16_width;
	lihj_uint16 uint16_highth;
	PMapCell	mapcell;
}GameMap;

struct path_node 
{
	lihj_uint16 parent_node;
	lihj_uint16 cur_node;
	lihj_uint16 insert_counter;
	lihj_uint16 update_counter;
	lihj_uint16 f;
	lihj_uint16 g;
	lihj_uint16 h;

	path_node()
	{
		parent_node = cur_node = -1;
		f = g = h = 0;

		insert_counter = update_counter = 0;
	}

	path_node(lihj_uint16 parent,lihj_uint16 cur)
	{
		parent_node = parent;
		cur_node    = cur;

		f = g = h = 0;

		insert_counter = update_counter = 0;
	}

	bool operator == (const path_node& node)
	{
		return this->cur_node == node.cur_node;
	}
};


struct Vector2D
{
	lihj_int32 x;
	lihj_int32 y;

	Vector2D()
	{
		x = y =0;
	}

	Vector2D(lihj_int32 x, lihj_int32 y)
	{
		this->x = x;
		this->y = y;
	}

	bool operator ==(const Vector2D& vector2d)
	{
		return this->x == vector2d.x
			&& this->y == vector2d.y;
	}

	Vector2D operator + (const Vector2D& vector2d) const
	{
		return Vector2D(this->x + vector2d.x , this->y + vector2d.y);
	}

	Vector2D operator - (const Vector2D& vector2d) const
	{
		return Vector2D(this->x - vector2d.x , this->y - vector2d.y);
	}

	Vector2D operator * (const lihj_int32 val) const
	{
		return Vector2D(this->x * val , this->y * val);
	}

	Vector2D operator / (const lihj_int32 val) const
	{
		return Vector2D(this->x / val , this->y / val);
	}

	static bool     In_XY_Direction(const Vector2D& vector2d)
	{
		return abs(vector2d.x) == abs(vector2d.y);
	}

	static bool     In_X_Y_Direction(const Vector2D& vector2d)
	{
		return (vector2d.x == 0 || vector2d.y == 0);
	}
};

#endif