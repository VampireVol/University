#include<iostream>
#include<cstdlib>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<sys/stat.h>

using namespace std;
struct InThread
{
    int flag;
    int fd;
};

void *threadOne(void *iThr)
{
    cout << "Thread 1 start work" << endl;
    int numOut;
    while((((InThread*) iThr)->flag) != 0)
    {
        numOut = rand() % 100 + 1;
        read(((InThread*) iThr)->fd, &numOut, 1);
        cout << "From buffer:" << numOut << endl << flush;
        sleep(1);
    }
    cout << endl << "Thread 1 finished work" << endl;
    ((InThread*) iThr)->flag = 4;
    pthread_exit(iThr);
}

int main(int argc, char *argv[])
{
    cout << "Program 1 start work" << endl;
    pthread_t thread1;
    InThread inThr1;
    void *pCode;
    mkfifo("fdLab8", O_RDWR);
    inThr1.fd = open("fdLab8", O_CREAT | O_RDWR | O_APPEND);
    inThr1.flag = 1;
    pthread_create(&thread1, NULL, &threadOne, &inThr1);
    getchar();
    inThr1.flag = 0;
    pthread_join(thread1, &pCode);
    if (*(int*) pCode == 4)
    {
        cout << "Thread 1 successfully completed work with code:" << *(int*) pCode << endl;
    }
    close(inThr1.fd);
    unlink("fdLab8");
    cout << "Program 1 finished work" << endl;
    return 0;
}
