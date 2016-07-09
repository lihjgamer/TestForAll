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
		��������
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
		���Ժϲ���Ʒ��������Ʒ
		Result : 
		>= 0	: ��ʾ�ϲ��ɹ� ,������ �ϲ�����slot
		-1		: ��ʾδ�ϲ�����Ҫ��ӵ� �µ� slot ��
	*/
	int    TryToMerge(CItem* pItem);

	CItem* GetItemBySlot(const uint32 nSlot);
	
	
	/*
		�������:
		��slot �� eraselist ���Ƴ� ��������� pushlist ��
		type : ������m_pBagCell ��� Ϊ true/false
		enumEraseAndPushType_False : ��ʾ ���� ��Slot �� : m_pBagCells[slot] = false;
		enumEraseAndPushType_True  : ��ʾ ʹ�� ��Slot �� : m_pBagCells[slot] = true;
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
