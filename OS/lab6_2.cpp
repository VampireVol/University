#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<fcntl.h>

using namespace std;
struct InThread
{
    int flag;
    int fstr;
    sem_t *sem;
};

void *threadOne(void *iThr)
{
    cout << "Thread 2 start work" << endl;
    char num = '2';
    while((((InThread*) iThr)->flag) != 0)
    {
        if (sem_wait(((InThread*) iThr)->sem) == 0)
        {
            for(int i = 0; i < 5; ++i)
            {
                write(((InThread*) iThr)->fstr, &num,1);
                cout << "2";
                fflush(stdout);
                sleep(1);
            }
            sem_post(((InThread*) iThr)->sem);
            sleep(2);
        }
        else
        {
            if ((((InThread*) iThr)->flag) == 0)
                break;
            sleep(1);
        }
    }
    cout << endl << "Thread 2 finished work" << endl;
    ((InThread*) iThr)->flag = 4;
    pthread_exit(iThr);
}

int main(int argc, char *argv[])
{
    cout << "Program 2 start work" << endl;
    pthread_t thread1;
    InThread inThr1;
    void *pCode;
    sem_t *sem;
    sem = sem_open("sem", O_CREAT, 0644, 1);
    inThr1.sem = sem;
    inThr1.flag = 1;
    inThr1.fstr = open("lab6Out.txt", O_CREAT | O_RDWR | O_APPEND);
    pthread_create(&thread1, NULL, &threadOne, &inThr1);
    getchar();
    inThr1.flag = 0;
    pthread_join(thread1, &pCode);
    if (*(int*) pCode == 4)
    {
        cout << "Thread 2 successfully completed work with code:" << *(int*) pCode << endl;
    }
    close(inThr1.fstr);
    sem_close(sem);
    sem_unlink("sem"); 
    cout << "Program 2 finished work" << endl;
    return 7;
}
