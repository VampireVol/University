#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>

using namespace std;

struct InThread
{
    int flag;
    sem_t *sem;
};

void *threadOne(void *iThr)
{
    cout << "Thread 1 start work" << endl;
    timespec timerBr;
    while((((InThread*) iThr)->flag) != 0)
    {
        clock_gettime(CLOCK_REALTIME, &timerBr);
        timerBr.tv_sec += 2;
        if (sem_timedwait(((InThread*) iThr)->sem, &timerBr) == 0)
        {
            for(int i = 0; i < 5; ++i)
            {
                cout << "1";
                fflush(stdout);
                sleep(1);
            }
            sem_post(((InThread*) iThr)->sem);
            sleep(2);
        }
        else
        {
            //perror("sem_timedwait");
            if ((((InThread*) iThr)->flag) == 0)
                break;
            sleep(1);
        }
    }
    cout << endl << "Thread 1 finished work" << endl;
    ((InThread*) iThr)->flag = 4;
    pthread_exit(iThr);
}

void *threadTwo(void *iThr)
{
    cout << "Thread 2 start work" << endl;
    timespec timerBr;
    while((((InThread*) iThr)->flag) != 0)
    {
        clock_gettime(CLOCK_REALTIME, &timerBr);
        timerBr.tv_sec += 2;
        if (sem_timedwait(((InThread*) iThr)->sem, &timerBr) == 0)
        {
            for(int i = 0; i < 5; ++i)
            {
                cout << "2";
                fflush(stdout);
                sleep(1);
            }
            sem_post(((InThread*) iThr)->sem);
            sleep(2);
        }
        else
        {
            //perror("sem_timedwait");
            if ((((InThread*) iThr)->flag) == 0)
                break;
            sleep(1);
        }
    }
    cout << endl << "Thread 2 finished work" << endl;
    ((InThread*) iThr)->flag = 5;
    pthread_exit(iThr);
}

int main(int argc, char *argv[])
{
    cout << "Program start work" << endl;
    pthread_t thread1, thread2;
    InThread inThr1, inThr2;
    void *pCode[2];
    sem_t sem;
    sem_init(&sem, 0, 1);
    inThr1.sem = inThr2.sem = &sem;
    inThr1.flag = inThr2.flag = 1;
    pthread_create(&thread1, NULL, &threadOne, &inThr1);
    pthread_create(&thread2, NULL, &threadTwo, &inThr2);
    getchar();
    inThr1.flag = inThr2.flag = 0;
    pthread_join(thread1, &pCode[0]);
    pthread_join(thread2, &pCode[1]);
    if (*(int*) pCode[0] == 4)
    {
        cout << "Thread 1 successfully completed work with code:" << *(int*) pCode[0] << endl;
    }
    if (*(int*) pCode[1] == 5)
    {
        cout << "Thread 2 successfully completed work with code:" << *(int*) pCode[1] << endl;
    }
    sem_destroy(&sem);
    cout << "Program finished work" << endl;
    return 0;
}
