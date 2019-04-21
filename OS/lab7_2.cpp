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
    sem_t *semw;
    sem_t *semr;
    void *mem;
};

void *threadOne(void *iThr)
{
    cout << "Thread 1 start work" << endl;
    int numOut;
    while((((InThread*) iThr)->flag) != 0)
    {
        sem_wait(((InThread*) iThr)->semw);
        numOut = *(int* )((InThread*) iThr)->mem;
        cout << numOut << " " << flush;
        sem_post(((InThread*) iThr)->semr);
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
    void *mem;
    sem_t *semw;
    sem_t *semr;
    int fd;
    fd = shm_open("descriptor", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, sizeof(int));
    mem = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    semw = sem_open("semw", O_CREAT, 0644, 0);
    semr = sem_open("semr", O_CREAT, 0644, 0);
    inThr1.semw = semw;
    inThr1.semr = semr;
    inThr1.flag = 1;
    inThr1.fd = fd;
    inThr1.mem = mem;
    pthread_create(&thread1, NULL, &threadOne, &inThr1);
    getchar();
    inThr1.flag = 0;
    pthread_join(thread1, &pCode);
    if (*(int*) pCode == 4)
    {
        cout << "Thread 1 successfully completed work with code:" << *(int*) pCode << endl;
    }
    munmap(mem, sizeof(int));
    close(fd);
    shm_unlink("descriptor");
    sem_close(semw);
    sem_close(semr);
    sem_unlink("semw");
    sem_unlink("semr"); 
    cout << "Program 1 finished work" << endl;
    return 0;
}
