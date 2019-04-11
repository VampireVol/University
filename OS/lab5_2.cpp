#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/wait.h>

using namespace std;

int main(int argc, char *argv[])
{
    cout << "Program 2 start work" << endl;
    int stat;
    pid_t pid = fork();
    if (pid == 0)
    {
        cout << "Child process. My pid is: " << getppid() << endl;
        execl("lab5", "1", "2", "3", "4", "5", NULL);
        perror("execl");
        cout << "Child process work end." << endl;
    }
    else
    {
        cout << "Parent process. My pid is: " << getppid() << endl;
        sleep(2);
        waitpid(pid, &stat, 0);
        cout << "Child process end with code: " << WEXITSTATUS(stat) << endl;
        cout << "Parent process work end." << endl;
    }
    cout << "Program 2 finished work" << endl;
    return 0;
}
