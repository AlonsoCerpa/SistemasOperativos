#include <random>
#include <iostream>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO_NAME "/tmp/myfifo"


void signal_handler(int my_signal);

int cont = 0;

int main()
{
    mknod(FIFO_NAME, S_IFIFO | 0666, 0);  //crea un archivo FIFO
    signal(SIGTERM, signal_handler);

    while (true)
    {

    }

    return 0;
}

void signal_handler(int my_signal)
{
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 1000);

    int signal_num = dist(rng);
    std::cout << cont << ": Senhal recibida: " << signal_num << "\n";
    ++cont;

//ESRIBIENDO EN EL ARCHIVO FIFO
    int fd;

    printf("Esperando a que lean el archivo...\n");
    fd = open(FIFO_NAME, O_WRONLY);   //Establece conexion entre un file y un file descriptor
    
    write(fd, &signal_num, sizeof(int));

    printf("Se ha escrito en el archivo: %d\n\n", signal_num);
    close(fd);
}
