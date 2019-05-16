#include<iostream>
#include<cstdlib>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<mqueue.h>
#include<string.h>

using namespace std;
mqd_t id;
struct InThread
{
    int exit;
    int flag;
   
};

void *threadOne(void *iThr)
{
    cout << "Thread 1 start work" << endl;
    char *mes = "Message";
    int num = 1;
    char buf[8192];
    while((((InThread*) iThr)->flag) != 0)
    {
        memset(buf, 0, sizeof(buf));
        strcpy(buf, mes);
        strcat(buf, to_string(num).c_str());
        cout << "return value: " << mq_send(id, buf, strlen(buf), 0) << flush << endl;
        ++num;
        sleep(1);
    }
    cout << endl << "Thread 1 finished work" << endl;
    ((InThread*) iThr)->exit = 4;
    pthread_exit(iThr);
}

int main(int argc, char *argv[])
{
    cout << "Program 1 start work" << endl;
    pthread_t thread1;
    InThread inThr1;
    void *pCode;
    const char *NAME = "/Queue";
    mq_attr attr, newattr;
    id = mq_open(NAME, O_CREAT | O_WRONLY | O_NONBLOCK, 0644, 0);
    cout << "Id from mq_open: " << id << endl;

    pthread_create(&thread1, NULL, &threadOne, &inThr1);
    inThr1.flag = 1;
    getchar();
    inThr1.flag = 0;
    pthread_join(thread1, &pCode);
    if (*(int*) pCode == 4)
    {
       cout << "Thread 1 successfully completed work with code:" << *(int*) pCode << endl;
    }
    mq_close(id);
    mq_unlink(NAME); 
    cout << "Program 1 finished work" << endl;
    return 0;
}
