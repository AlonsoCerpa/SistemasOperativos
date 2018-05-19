#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>

struct my_msgbuf {
    long mtype;
    int signal_num;
};

int main(void)
{
    my_msgbuf buf;
    int msqid;
    key_t key;

    if ((key = ftok("receive_pipe.cpp", 'B')) == -1) { /* same key as kirk.c */
        perror("ftok");
        exit(1);
    }
    if ((msqid = msgget(key, 0644)) == -1) { /* connect to the queue */
        perror("msgget");
        exit(1);
    }

    while (true)
    {
        msgrcv(msqid, &buf, sizeof(int), 0, 0);
        std::cout << "Impar: " << buf.signal_num << "\n";
        //sleep(1);
    }

    return 0;
}
