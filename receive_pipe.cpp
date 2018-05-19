#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/msg.h>

#include <thread>
#include <queue>
#include <mutex>

#define FIFO_NAME "/tmp/myfifo"
#define SHMSZ 27

std::mutex mt_queue;
std::queue<int> signal_nums_q;

struct my_msgbuf {
    long mtype;
    int signal_num;
};

void read_fifo()
{
    //FIFO
    mknod(FIFO_NAME, S_IFIFO | 0666, 0);

    int fd;
    int signal_num;
    while (true)
    {
        printf("Esperando a que escriban en el archivo...\n");
        fd = open(FIFO_NAME, O_RDONLY);
        read(fd, &signal_num, sizeof(int));

        mt_queue.lock();
        signal_nums_q.push(signal_num);
        mt_queue.unlock();

        printf("Se leyo del archivo FIFO: %d\n\n", signal_num);
        close(fd);
        //sleep(1);
    }
}

void send_msg()
{
    int signal_num;

    //SHARED MEMORY
    int shmid;
    key_t key1;
    int *shm;

    key1 = 5678;
    if ((shmid = shmget(key1, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((shm = (int *)shmat(shmid, NULL, 0)) == (int *) -1) {
        perror("shmat");
        exit(1);
    }


    //MESSAGE QUEUE
    int msqid;
    key_t key2;
    if ((key2 = ftok("receive_pipe.cpp", 'B')) == -1) {
        perror("ftok");
        exit(1);
    }
    if ((msqid = msgget(key2, 0644 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }

    struct my_msgbuf buf;
    buf.mtype = 1; /* we don't really care in this case */


    while (true)
    {
        if (signal_nums_q.size() > 0)
        {
            mt_queue.lock();
            signal_num = signal_nums_q.front();
            signal_nums_q.pop();
            mt_queue.unlock();

            if (signal_num % 2 == 0)
            {
                //SHARED MEMORY
                *shm = signal_num;
            }
            else
            {
                //MESSAGE QUEUE
                buf.signal_num = signal_num;
                msgsnd(msqid, &buf, sizeof(int), 0);

                //msgctl(msqid, IPC_RMID, NULL);
            }
            //sleep(5);
        }
    }
}

int main()
{
    std::thread t1(read_fifo);
    std::thread t2(send_msg);

    t1.join();
    t2.join();

    return 0;
}

//Compilar: g++ receive_pipe.cpp -o receive_p.exe -pthread