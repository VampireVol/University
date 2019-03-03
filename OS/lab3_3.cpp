#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<unistd.h>
#include<fcntl.h>

using namespace std;

struct InThread
{
    int flag;
    int *fileDes;
};

void *threadOne(void *iThr)
{
    int number = 1;
    cout << "Thread 1 start work" << endl;
    while((((InThread*) iThr)->flag) != 0)
    {
        cout << "To buffer: " << number << endl;
        write(((InThread*) iThr)->fileDes[1], &number, 1);
        ++number;
        sleep(2);
    }
    cout << endl << "Thread 1 finished work" << endl;
    ((InThread*) iThr)->flag = 4;
    pthread_exit(iThr);
}
void *threadTwo(void *iThr)
{
    int number = 0;
    cout << "Thread 2 start work" << endl;
    while((((InThread*) iThr)->flag) != 0)
    {
        int rc = read(((InThread*) iThr)->fileDes[0], &number, 1);
    	if (rc > 0)
    	{
    		cout << "From buffer: " << number << endl;
    	}
    	else
    	{
    		perror("read");
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
    int fileDes[2];
    pipe(fileDes);
    fcntl(fileDes[0], F_SETFL, O_NONBLOCK);
    fcntl(fileDes[1], F_SETFL, O_NONBLOCK);
    inThr1.fileDes = inThr2.fileDes = fileDes;
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
    close(fileDes[0]);
    close(fileDes[1]);
    cout << "Program finished work" << endl;
    return 0;
}
