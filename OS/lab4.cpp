#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

using namespace std;
const int maxCount = 100;
class TBuffer
{
    public:
    TBuffer();
    ~TBuffer();
    void Write(int m);
    int Read();
    private:
    int in, out, curCount;
    int buf[maxCount];
    pthread_cond_t cond;
    pthread_mutex_t mutex;
};
TBuffer::TBuffer() : in(0), out(0), curCount(0) 
{
    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);
}
TBuffer::~TBuffer()
{
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
}
void TBuffer::Write(int m)
{
    timespec timerBr;
    pthread_mutex_lock(&mutex);
    while(curCount == maxCount)
    {
        clock_gettime(CLOCK_REALTIME, &timerBr);
        timerBr.tv_sec += 5;
        if (pthread_cond_timedwait(&cond, &mutex, &timerBr) == ETIMEDOUT)
            return;
    }
    ++curCount;
    buf[in++] = m;
    in %= maxCount;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}
int TBuffer::Read()
{
    timespec timerBr;
    pthread_mutex_lock(&mutex);
    while(curCount == 0)
    {
        clock_gettime(CLOCK_REALTIME, &timerBr);
        timerBr.tv_sec += 5;
        if (pthread_cond_timedwait(&cond, &mutex, &timerBr) == ETIMEDOUT)
            return -1;
    }
    --curCount;
    int m = buf[out++];
    out %= maxCount;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    return m;
}
struct InThread
{
    int flag;
    TBuffer *buf;
};

void *threadOne(void *iThr)
{
    int number = 1;
    cout << "Thread 1 start work" << endl;
    while((((InThread*) iThr)->flag) != 0)
    {
        cout << "To buffer: " << number << endl;
        ((InThread*) iThr)->buf->Write(number);
        ++number;
        sleep(3);
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
        number = ((InThread*) iThr)->buf->Read();
        cout << "From buffer: " << number << endl;
        fflush(stdout);
        sleep(1);
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
    TBuffer buf;
    inThr1.flag = inThr2.flag = 1;
    inThr1.buf = inThr2.buf = &buf;
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
    cout << "Program finished work" << endl;
    return 0;
}
