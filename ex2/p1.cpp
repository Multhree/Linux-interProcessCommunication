#include<stdio.h>
#include"sem.h"
#include"box.h"


int main()
{
	
	while(1)
	{
		int op;
		printf("input op:0 send, 1 receive, 2 recall, 3 exit:");
		scanf("%d",&op);
		if(op == 0)
		{
			printf("input msg:");
			int msg;
			scanf("%d",&msg);
			send(boxB,msg);
		}
		else if(op == 1)
		{
			printf("msg:%d\n",receive(boxA));
		}
		else if(op == 2)
		{
			recall(boxB);
		}
		else if(op == 3)
		{
			deleteBox(boxA);
			break;
		}
		else
		{
			printf("invalid op! please input again!\n");
		}
	}
	return 0;
}
