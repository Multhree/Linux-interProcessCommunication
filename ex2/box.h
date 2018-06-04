#ifndef box_h
#define box_h

#include"sem.h"
#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>

#define boxId  1234
#define boxLen 5

struct box
{
//信箱头
	int bid;//信箱标识符
	int bsize;//信格总数
	/*同步信号量*/
	int mailnum;//与信箱中信件数量相关的信号量
	int freenum;//与信箱中空信格数量相关的信号量
	/*互斥信号量*/
	int rmutex;//接收信件时的互斥信号量
	int wmutex;//存入信件时的互斥信号量
	int out;//当前可读取信件的信格地址
	int in;//当前可存入信件的信格地址
//信箱体
	int  *buf;
	void *shm;
};

struct box* getNewBox(int len,int n)//获取新信箱,len为信格总数，n为唯一邮箱变量
{
	int shmid;
        struct box* msgbox = (struct box*)malloc(sizeof(struct box));
        shmid = shmget((key_t)(boxId+n),sizeof(int)*len,0666|IPC_CREAT);
	msgbox->shm = shmat(shmid,0,0);
        msgbox->buf = (int *)(msgbox->shm);
	//初始化信格总数
        msgbox->bsize = len;
        //初始化邮箱标号，为共享信号的标号
        msgbox->bid = shmid;
        //初始化信号量
        msgbox->mailnum = getNewSem((key_t)(shmid+1));
        setSemValue(msgbox->mailnum,0);

        msgbox->freenum = getNewSem((key_t)(shmid+2));
        setSemValue(msgbox->freenum,len);

        msgbox->rmutex = getNewSem((key_t)(shmid+3));
        setSemValue(msgbox->rmutex,1);

        msgbox->wmutex = getNewSem((key_t)(shmid+4));
        setSemValue(msgbox->wmutex,1);

        msgbox->out = 0;
        msgbox->in = 0;


	return msgbox;
}

void send(struct box* dest,int msg)//发送
{
	P(dest->freenum);
	P(dest->wmutex);
	dest->buf[dest->in] = msg;
	dest->in = (dest->in+1)%dest->bsize;
	V(dest->wmutex);
	V(dest->mailnum);
};


int receive(struct box* addr)//接收
{
	int msg;
	P(addr->mailnum);
	P(addr->rmutex);
	msg = addr->buf[addr->out];
	addr->out = (addr->out+1)%addr->bsize;
	V(addr->rmutex);
	V(addr->freenum);
	
	return msg;
};

void recall(struct box* addr)//撤销
{
	P(addr->mailnum);
	P(addr->wmutex);
	P(addr->rmutex);
	addr->in = (addr->in-1)%addr->bsize;
	V(addr->rmutex);
	V(addr->wmutex);
	V(addr->freenum);
};
void deleteBox(struct box* msgbox)//删除信箱
{
	delSem(msgbox->freenum);
	delSem(msgbox->mailnum);
	delSem(msgbox->rmutex);
	delSem(msgbox->wmutex);
	shmdt(msgbox->shm);
	shmctl(msgbox->bid,IPC_RMID,0);
};

struct box* boxA = getNewBox(boxLen,1);
struct box* boxB = getNewBox(boxLen,2);
#endif
