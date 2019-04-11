#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

using namespace std;

int main(int argc, char *argv[])
{
    cout << "Program 1 start work" << endl;
    for (int i = 0; i < 5; ++i)
    {
        cout << argv[i] << " ";
        fflush(stdout);
        sleep(2);
    }
    cout << endl;
    cout << "Program 1 finished work" << endl;
    return 6;
}
