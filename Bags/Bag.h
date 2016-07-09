#pragma once

#include <list>
#include <vector>
#include "ItemDef.h"
class CItem;

class CBag
{
public:
	CBag(void);
	~CBag(void);

public:
	bool	Init(const int nBagCellNum);
	bool    Uninit();

	int	    GetFreeIndexNum() const;
	int		GetUseIndexNum() const;

	int		GetFreeIndex() const;

	bool	AddItem(const uint32 id,const uint32 stack,const uint32 count);
	bool    DelItem(const uint32 id,const uint32 stack,const uint32 count);

	bool	SwapSlotToSlot(const uint32 nSlot1,const uint32 nSlot2);
	bool	SwapItemToItem(CItem* pItem1,CItem* pItem2);
	bool	SwapItemToSlot(CItem* pSrc,const uint32 nSlot);

	/*
		背包整理
		sort_type : 
		0 : sort_by_id
		1 : sort_by_stack
		2 : sort_by_count
	*/
	bool    SortBag(const uint32 sort_type);

	void	PrintGagInfo();
private:
	typedef std::list<int> BagCellList;
	typedef std::vector<CItem*> VectItemList;
	enum EraseAndPushType
	{
		enumEraseAndPushType_False,
		enumEraseAndPushType_True,
	};


	void    DestroyItems();

	/*
		尝试合并物品到已有物品
		Result : 
		>= 0	: 表示合并成功 ,并返回 合并到的slot
		-1		: 表示未合并，需要添加到 新的 slot 上
	*/
	int    TryToMerge(CItem* pItem);

	CItem* GetItemBySlot(const uint32 nSlot);
	
	
	/*
		函数简介:
		将slot 从 eraselist 中移除 ，并添加至 pushlist 中
		type : 用来将m_pBagCell 标记 为 true/false
		enumEraseAndPushType_False : 表示 废弃 该Slot 即 : m_pBagCells[slot] = false;
		enumEraseAndPushType_True  : 表示 使用 该Slot 即 : m_pBagCells[slot] = true;
	*/
	void   EraseAndPushBySlot(BagCellList& eraselist,BagCellList& pushlist,const uint32 slot,const EraseAndPushType type = enumEraseAndPushType_False);


	static  bool SortById(const CItem* pItem1,const CItem* pItem2);
	static  bool SortByStack(const CItem* pItem1,const CItem* pItem2);
	static  bool SortByCount(const CItem* pItem1,const CItem* pItem2);
private:


	static uint32 m_unique_ids;
private:
	bool*			m_pBagCells;
	uint32			m_nBagCellsNum;
	VectItemList	m_vectItemList;
	

	BagCellList			   m_listFreeIndex;
	BagCellList			   m_listUseIndex;
};
