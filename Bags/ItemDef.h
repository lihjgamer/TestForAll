#pragma once

#ifndef uint32
#define uint32 unsigned int 
#endif

struct item_attr 
{
	uint32 id;
	uint32 stack;
	uint32 count;

	bool operator ==(const item_attr& attr)
	{
		return (id == attr.id && stack == attr.stack);
	}
};

struct item 
{
	uint32 slot;
	uint32 unique_id;
	item_attr    attr;
};



