#include "Items.h"
#include <string.h>

CItem::CItem(void)
{
	memset(&m_item,0,sizeof(m_item));
}

CItem::~CItem(void)
{
}

CItem* CItem::Constructor()
{
	return new CItem;
}

void CItem::Destroy(CItem* pItem)
{
	if (pItem != NULL)
	{
		delete pItem;
	}
}

void CItem::SetItemAttr(const uint32 id,const uint32 stack,const uint32 count)
{
	m_item.attr.id    = id;
	m_item.attr.stack = stack;
	m_item.attr.count = count;
}

void CItem::SetItemAttr(item_attr& attr)
{
	m_item.attr = attr;
}

void CItem::GetItemAttr(uint32 &id,uint32 &stack,uint32 &count)
{
	id    = m_item.attr.id;
	stack = m_item.attr.stack;
	count = m_item.attr.count;
}

void CItem::GetItemAttr(item_attr& attr)
{
	attr = m_item.attr;
}

uint32 CItem::GetItemSlot() const
{
	return m_item.slot;
}

void CItem::SetItemSlot(const uint32 slot)
{
	m_item.slot = slot;
}

uint32 CItem::GetUnqueId() const
{
	return m_item.unique_id;
}

void CItem::SetUnqueId(const uint32 unqueid)
{
	m_item.unique_id = unqueid;
}

void CItem::SetItemId(const uint32 id)
{
	m_item.attr.id = id;
}

uint32 CItem::GetItemId() const
{
	return m_item.attr.id;
}

void CItem::SetItemStack(const uint32 stack)
{
	m_item.attr.stack = stack;
}

uint32 CItem::GetItemStack() const
{
	return m_item.attr.stack;
}

uint32 CItem::GetItemCount() const
{
	return m_item.attr.count;
}

void CItem::SetItemCount(const uint32 count)
{
	m_item.attr.count = count;
}
