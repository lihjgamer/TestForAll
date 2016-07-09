#pragma once
#include "ItemDef.h"
class CItem
{
	friend class CBag;
public:
	CItem(void);
	~CItem(void);

	static CItem* Constructor();
	static void   Destroy(CItem* pItem);

	void		  SetItemAttr(const uint32 id,const uint32 stack,const uint32 count);
	void		  SetItemAttr(item_attr&  attr);

	void		  GetItemAttr(uint32 &id,uint32 &stack,uint32 &count);
	void		  GetItemAttr(item_attr&  attr);

	uint32		  GetItemId() const;
	void		  SetItemId(const uint32 id);

	uint32	      GetItemStack() const;
	void		  SetItemStack(const uint32 stack);

	uint32        GetItemCount() const;
	void		  SetItemCount(const uint32 count);
	
	uint32		  GetItemSlot() const;
	void		  SetItemSlot(const uint32 slot);

	uint32        GetUnqueId()  const;
	void		  SetUnqueId(const uint32 unqueid);

private:
	item	m_item;
};
