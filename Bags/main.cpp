#include <stdio.h>
#include <stdlib.h>


#include "Bag.h"

#include "vld.h"
int main(int argc,char** argv)
{
	
	CBag bag;
	bag.Init(50);

	bag.AddItem(1,1,1);
	bag.AddItem(4,1,1);
	bag.AddItem(2,1,1);
	bag.AddItem(2,1,1);
	bag.AddItem(2,1,1);
	bag.AddItem(3,1,1);
	bag.AddItem(5,10,3);
	bag.AddItem(5,10,3);
	bag.AddItem(5,10,3);
	bag.AddItem(5,10,3);

	bag.AddItem(1,1,1);
	bag.AddItem(4,1,1);
	bag.AddItem(2,1,1);
	bag.AddItem(2,1,1);
	bag.AddItem(2,1,1);
	bag.AddItem(3,1,1);
	bag.AddItem(5,10,3);
	bag.AddItem(5,10,3);
	bag.AddItem(5,10,3);
	bag.AddItem(5,10,3);
	bag.AddItem(6,3,3);
	bag.AddItem(6,3,2);
	bag.AddItem(6,3,2);
	bag.AddItem(5,10,3);
	bag.AddItem(5,10,3);
	bag.AddItem(5,10,3);
	bag.AddItem(7,1,1);


	bag.PrintGagInfo();
	printf("-------------------------------\n");
	bag.SortBag(1);
	bag.PrintGagInfo();

	printf("-------------------------------\n");

	bag.SwapSlotToSlot(0,20);
	bag.PrintGagInfo();
	printf("-------------------------------\n");

	bag.SwapSlotToSlot(1,30);
	bag.PrintGagInfo();
	printf("-------------------------------\n");

	bag.SortBag(0);
	bag.PrintGagInfo();
	printf("-------------------------------\n");

	bag.DelItem(2,1,1);
	bag.PrintGagInfo();
	printf("-------------------------------\n");

	bag.SortBag(0);
	bag.PrintGagInfo();
	printf("-------------------------------\n");

	
	bag.Uninit();

	system("pause");
	return 0;
}