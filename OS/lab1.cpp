#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

using namespace std;

void *threadOne(void *flag)
{
    cout << "Thread 1 start work" << endl;
    while((*(int*) flag) != 0)
    {
        cout << "1";
        fflush(stdout);
        sleep(1);
    }
    cout << endl << "Thread 1 finished work" << endl;
    *(int*) flag = 4;
    pthread_exit(flag);
}
void *threadTwo(void *flag)
{
    cout << "Thread 2 start work" << endl;
    while((*(int*) flag) != 0)
    {
        cout << "2";
        fflush(stdout);
        sleep(1);
    }
    cout << endl << "Thread 2 finished work" << endl;
    *(int*) flag = 5;
    pthread_exit(flag);
}
int main(int argc, char *argv[])
{
    cout << "Program start work" << endl;
    pthread_t thread1, thread2;
    int flagOne, flagTwo;
    void *pCode[2];
    flagOne = flagTwo = 1;
    pthread_create(&thread1, NULL, &threadOne, &flagOne);
    pthread_create(&thread2, NULL, &threadTwo, &flagTwo);
    getchar();
    flagOne = flagTwo = 0;
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
    cout << "Program finished work" << endl;
    return 0;
}
