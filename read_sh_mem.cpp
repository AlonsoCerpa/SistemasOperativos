#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <iostream>

#define SHMSZ 27

int main()
{
    int shmid;
    key_t key;
    int *shm;

    key = 5678;

    if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((shm = (int *)shmat(shmid, NULL, 0)) == (int *) -1) {
        perror("shmat");
        exit(1);
    }

    while (true)
    {
        std::cout << "Par: " << *shm << "\n";
        //sleep(1);
    }
    return 0;
}

//Compilar: g++ read_sh_mem.cpp -o read_sh_m.exe