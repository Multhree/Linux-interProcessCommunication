#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEY 1234
#define MESSAGEMAXSIZE 1024
#define RECEIVETYPE 0

struct msg
{
	long int messageType;
	char message[MESSAGEMAXSIZE];
};

int main()
{	
	struct msg data;
	int msgid1 = msgget((key_t)KEY, 0666|IPC_CREAT);//创建消息队列1
	if(msgid1 < 0)
	{
		printf("msg1get failed.\n");
		exit(EXIT_FAILURE);
	}

	
	int msgid2= msgget((key_t)(KEY+1), 0666|IPC_CREAT);//创建消息队列2
	if(msgid2 < 0)
	{
		printf("msg2get failed.\n");
		exit(EXIT_FAILURE);
	}
	
	int op;
	while(1)
	{
		printf("input op:0 send, 1 receive, 2 exit:");
		scanf("%d", &op);	
		
		if(op == 0)
		{
			printf("input msg:");
			scanf("%s",data.message);
			msgsnd(msgid1, &data,MESSAGEMAXSIZE,0);//向消息队列1发送消息
		}	
		else if(op == 1)
		{			
			msgrcv(msgid2, &data,MESSAGEMAXSIZE,RECEIVETYPE,0);
			printf("msg: %s\n",data.message);//从消息队列2接收消息
		}
		else if(op == 2)
		{
			break;
		}
		else
		{
			printf("invalid op! please input again!\n");
		}	
	}
	msgctl(msgid1, IPC_RMID, 0);
	msgctl(msgid2, IPC_RMID, 0);
	
	return 0;
}
