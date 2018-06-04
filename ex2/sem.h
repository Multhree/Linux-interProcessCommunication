#ifndef sem_h
#define sem_h

#include<stdio.h>
#include<sys/sem.h>

int getNewSem(key_t key)//获取新信号量
{
	return semget(key,1,0666|IPC_CREAT);
}

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *arry;
};

int setSemValue(int semid,int n)//设置信号量
{
	union semun semTemp;
        semTemp.val = n;
        if(semctl(semid,0,SETVAL,semTemp)==-1)
        {
		return 0;
	}
        return 1;
}

int delSem(int semid)//删除信号量
{
	union semun semTemp;
        semctl(semid,0,IPC_RMID,semTemp);
}
/*
struct sembuf  
{  
	unsigned short int sem_num;    //在信号量集中的序号 
  	short int sem_op;              // 对信号量的操作，>0, 0, <0 
  	short int sem_flg;             // 操作标识：0， IPC_WAIT, SEM_UNDO  
}; 
*/ 
int P(int semid)//P操作
{
	struct sembuf semTemp;
        semTemp.sem_num = 0;
	semTemp.sem_op = -1;
	semTemp.sem_flg = SEM_UNDO;
        if(semop(semid,&semTemp,1) == -1)
        {
            printf("P() failed\n");
            return 0;
        }
        return 1;
}

int V(int semid)//V操作
{
	struct sembuf semTemp;
        semTemp.sem_num = 0;
	semTemp.sem_op = 1;
	semTemp.sem_flg = SEM_UNDO;
        if(semop(semid,&semTemp,1) == -1)
        {
            printf("V() failed\n");
            return 0;
        }
        return 1;
}

#endif
