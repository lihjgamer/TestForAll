#include "Bag.h"
#include "Items.h"

#include <algorithm>
//#include <functional>
uint32 CBag::m_unique_ids = 1;
CBag::CBag(void)
{
}

CBag::~CBag(void)
{
}

bool CBag::Init(const int nBagCellNum)
{
	m_nBagCellsNum = nBagCellNum;
	m_pBagCells = new bool[m_nBagCellsNum]();
	//memset(m_pBagCells,0,sizeof(m_pBagCells));
	
	if (m_pBagCells == NULL)
	{
		return false;
	}

	for (uint32 i = 0; i < m_nBagCellsNum; i ++)
	{
		m_listFreeIndex.push_back(i);
	}
	m_listUseIndex.clear();

	m_vectItemList.clear();

	return true;
}

bool CBag::Uninit()
{
	if (m_pBagCells != NULL)
	{
		delete []m_pBagCells;
		m_pBagCells = NULL;
		m_nBagCellsNum = 0;
	}
	m_listFreeIndex.clear();
	m_listUseIndex.clear();

	DestroyItems();
	return true;
}

int CBag::GetFreeIndexNum() const
{
	return (int)m_listFreeIndex.size();
}

int CBag::GetUseIndexNum() const
{
	return (int)m_listUseIndex.size();
}

int CBag::GetFreeIndex() const
{
	int nIndex = -1;
	if (m_listFreeIndex.empty())
	{
		return nIndex;
	}
	nIndex = m_listFreeIndex.front();
	return nIndex;
}

void CBag::DestroyItems()
{
	for(VectItemList::iterator iter = m_vectItemList.begin();
		iter != m_vectItemList.end();
		++ iter)
	{
		CItem::Destroy(*iter);
	}
	m_vectItemList.clear();
}

bool CBag::AddItem(const uint32 id,const uint32 stack,const uint32 count)
{
	CItem* pItem = CItem::Constructor();
	if (pItem == NULL)
	{
		return false;
	}

	pItem->SetItemAttr(id,stack,count);
	pItem->SetUnqueId(m_unique_ids++);

	int nSlot = TryToMerge(pItem);
	if (nSlot > 0)
	{
		CItem::Destroy(pItem);
		pItem = NULL;
	}
	else
	{
		uint32 nFreeSlot= GetFreeIndex();
		if (nFreeSlot != -1)
		{
			pItem->SetItemSlot(nFreeSlot);
			m_vectItemList.push_back(pItem);

			EraseAndPushBySlot(m_listFreeIndex,m_listUseIndex,nFreeSlot,enumEraseAndPushType_True);
		}
	}

	return true;
}

bool CBag::DelItem(const uint32 id,const uint32 stack,const uint32 count)
{
	int del_num = count;
	for(VectItemList::iterator iter = m_vectItemList.begin();
		iter != m_vectItemList.end();
		)
	{
		CItem* pItem = (*iter);
		if (pItem->m_item.attr.id == id
			&&pItem->m_item.attr.stack == stack)
		{
			if (pItem->m_item.attr.count > (uint32)del_num)
			{
				pItem->m_item.attr.count -= del_num;
				del_num = 0;
			}
			else
			{
				int nSlot = pItem->m_item.slot;
				del_num -= pItem->m_item.attr.count;
				
				EraseAndPushBySlot(m_listUseIndex,m_listFreeIndex,nSlot,enumEraseAndPushType_False);
				CItem::Destroy(pItem);
				iter = m_vectItemList.erase(iter);
				continue;
			}
		}
		if (del_num <= 0)
		{
			break;
		}
		++ iter;
	}
	return true;
}

int CBag::TryToMerge(CItem* pItem)
{
	int bSlot = -1;
	if (pItem->GetItemStack() > 1)
	{
		for (VectItemList::iterator iter = m_vectItemList.begin();
			iter != m_vectItemList.end();
			++ iter)
		{
			item_attr attr;
			(*iter)->GetItemAttr(attr);
			if (attr == pItem->m_item.attr
				&& attr.count < attr.stack)
			{
				uint32 mergecount = attr.stack - attr.count;
				if (mergecount >= pItem->m_item.attr.count)
				{
					attr.count += pItem->m_item.attr.count;
					(*iter)->SetItemCount(attr.count);
					pItem->m_item.attr.count = 0;
				}
				else
				{
					pItem->m_item.attr.count -= mergecount;
					(*iter)->SetItemCount(attr.stack);
				}

				if (pItem->m_item.attr.count <= 0)
				{
					bSlot = (*iter)->GetItemSlot();
					break;
				}
			}
		}
	}
	if (pItem->m_item.attr.count <= 0)
	{
		return bSlot;
	}
	else
	{
		return -1;
	}
}

bool CBag::SwapSlotToSlot(const uint32 nSlot1,const uint32 nSlot2)
{
	if (nSlot1 >= 0 && nSlot1 < m_nBagCellsNum
		&& nSlot2 >= 0 && nSlot2 < m_nBagCellsNum
		&& nSlot1 != nSlot2
		&& (m_pBagCells[nSlot1]||m_pBagCells[nSlot2]))
	{
		CItem* pSlot1Item = GetItemBySlot(nSlot1);
		CItem* pSlot2Item = GetItemBySlot(nSlot2);
		
		if (pSlot1Item != NULL && pSlot2Item != NULL)
		{
			pSlot1Item->SetItemSlot(nSlot2);
			pSlot2Item->SetItemSlot(nSlot1);
		}
		else if (pSlot1Item == NULL && pSlot2Item != NULL)
		{
			pSlot2Item->SetItemSlot(nSlot1);
			EraseAndPushBySlot(m_listFreeIndex,m_listUseIndex,nSlot1,enumEraseAndPushType_True);
			EraseAndPushBySlot(m_listUseIndex,m_listFreeIndex,nSlot2,enumEraseAndPushType_False);
		}
		else if (pSlot1Item != NULL && pSlot2Item == NULL)
		{
			pSlot1Item->SetItemSlot(nSlot2);
			EraseAndPushBySlot(m_listFreeIndex,m_listUseIndex,nSlot2,enumEraseAndPushType_True);
			EraseAndPushBySlot(m_listUseIndex,m_listFreeIndex,nSlot1,enumEraseAndPushType_False);
		}
		return true;
	}

	return false;
}

bool CBag::SwapItemToItem(CItem* pItem1,CItem* pItem2)
{
	if (pItem1 != pItem2 && pItem1 != NULL && pItem2 != NULL)
	{
		int nSlot1 = pItem1->GetItemSlot();
		int nSlot2 = pItem2->GetItemSlot();

		pItem1->SetItemSlot(nSlot2);
		pItem2->SetItemSlot(nSlot1);

		return true;
	}
	return false;
}

bool CBag::SwapItemToSlot(CItem* pSrc,const uint32 nSlot)
{
	if (pSrc != NULL && pSrc->GetItemSlot() != nSlot && nSlot >= 0 && nSlot < m_nBagCellsNum)
	{
		CItem* pDst = GetItemBySlot(nSlot);
		if (pDst != NULL)
		{
			SwapItemToItem(pSrc,pDst);
		}
		else
		{
			SwapSlotToSlot(pSrc->GetItemSlot(),nSlot);
		}
		return true;
	}

	return false;
}

CItem* CBag::GetItemBySlot(const uint32 nSlot)
{
	for (VectItemList::iterator iter = m_vectItemList.begin();
		iter != m_vectItemList.end();
		++ iter)
	{
		if ((*iter)->GetItemSlot() == nSlot)
		{
			return (*iter);
		}
	}

	return NULL;
}

void CBag::PrintGagInfo()
{
	const char* szContext[] = 
	{
		"true",
		"false",
	};

	int nCol = 10;
	printf("bag cell info : \n");
	for (uint32 i = 0; i < m_nBagCellsNum; i ++)
	{
		printf("[%02d_%05s] ",i+1,m_pBagCells[i] ? szContext[0] : szContext[1]);
		if ((i + 1)%10 == 0)
		{
			printf("\n");
		}
	}

	int nCurCol = 0;
	printf("\nitem cell info : \n");
	for (VectItemList::iterator iter = m_vectItemList.begin();
		 iter != m_vectItemList.end();
		 ++ iter)
	{
		CItem* pItem = (*iter);
		printf("%02d_%02d ",pItem->GetItemSlot(),pItem->GetItemId());
		if ((nCurCol + 1)%10 == 0)
		{
			printf("\n");
		}
		nCurCol ++;
	}

// 	printf("\nuse index info :\n");
// 	for (BagCellList::iterator iter = m_listFreeIndex.begin())
// 	{
// 	}

	printf("\n");
}

void CBag::EraseAndPushBySlot(BagCellList& eraselist,BagCellList& pushlist,const uint32 slot,const EraseAndPushType type )
{
	for (BagCellList::iterator iter = eraselist.begin();
		iter != eraselist.end();
		++ iter)
	{
		if (uint32(*iter) == slot)
		{
			eraselist.erase(iter);
			pushlist.push_back(slot);
			//std::sort(pushlist.begin(),pushlist.end());
			pushlist.sort();
			type == enumEraseAndPushType_False ? m_pBagCells[slot] = false : m_pBagCells[slot] = true;
			break;
		}
	}
}

bool CBag::SortBag(const uint32 sort_type)
{
	switch(sort_type)
	{
	case 0:
		{
			std::sort(m_vectItemList.begin(),m_vectItemList.end(),&CBag::SortById);
		}
		break;
	case 1:
		{
			std::sort(m_vectItemList.begin(),m_vectItemList.end(),&CBag::SortByStack);
		}
		break;
	case 2:
		{
			std::sort(m_vectItemList.begin(),m_vectItemList.end(),&CBag::SortByCount);
		}
		break;
	default:
		break;
	}
	
	uint32 slot_begin = 0;
	uint32 slot_old   = 0;
	for (VectItemList::iterator iter = m_vectItemList.begin();
		iter != m_vectItemList.end();
		++ iter)
	{
		CItem* pItem = (*iter);
		
		slot_old     = pItem->GetItemSlot();
		if (slot_old == slot_begin)
		{
			slot_begin ++;
			continue;
		}
		CItem* pSwapItem = GetItemBySlot(slot_begin);
		if (pSwapItem != NULL && pSwapItem != pItem)
		{
			pSwapItem->SetItemSlot(slot_old);
			pItem->SetItemSlot(slot_begin);
			slot_begin ++;
			continue;
		}

		pItem->SetItemSlot(slot_begin);

		EraseAndPushBySlot(m_listUseIndex,m_listFreeIndex,slot_old,enumEraseAndPushType_False);
		EraseAndPushBySlot(m_listFreeIndex,m_listUseIndex,slot_begin,enumEraseAndPushType_True);
		
		slot_begin ++;

	}
	return true;
}

bool CBag::SortById(const CItem* pItem1,const CItem* pItem2)
{
	// °´IDÅÅÐò
	if( pItem1->m_item.attr.id != pItem2->m_item.attr.id)
	{
		return pItem1->m_item.attr.id > pItem2->m_item.attr.id;
	}
	else
	{
		return pItem1->m_item.attr.count > pItem2->m_item.attr.count;
	}
}

bool CBag::SortByStack(const CItem* pItem1,const CItem* pItem2)
{
	if( pItem1->m_item.attr.id != pItem2->m_item.attr.id)
	{
		return pItem1->m_item.attr.id > pItem2->m_item.attr.id;
	}
	else
	{
		return pItem1->m_item.attr.stack > pItem2->m_item.attr.stack;
	}
}

bool CBag::SortByCount(const CItem* pItem1,const CItem* pItem2)
{
	if( pItem1->m_item.attr.id != pItem2->m_item.attr.id)
	{
		return pItem1->m_item.attr.id > pItem2->m_item.attr.id;
	}
	else
	{
		return pItem1->m_item.attr.count > pItem2->m_item.attr.count;
	}
}

