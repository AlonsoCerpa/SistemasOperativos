#include <signal.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <mutex>
#include <queue>

int cont = 0;
std::mutex mt_queue;
std::mutex mt_print;
int signals_queue[100];
int idx_queue = 0;

void add_signal_to_queue(int thread_id)
{
    mt_queue.lock();
    signals_queue[idx_queue] = thread_id;
    ++idx_queue;
    mt_queue.unlock();
}

void send_signals(pid_t pid, int sig)
{
    int thread_id;
    int idx_q;

    while (true)
    {
        mt_queue.lock();    
        if (idx_queue > 0)
        {
            --idx_queue;
            thread_id = signals_queue[idx_queue];

            kill(pid, SIGTERM);

            mt_print.lock();
            std::cout << cont << ": Se envio la senhal del thread " << thread_id << "\n";
            mt_print.unlock();

            ++cont;

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        mt_queue.unlock();
    }
}

int main(int argc, char *argv[])
{
    int pid;

    std::cout << "Ingrese PID de 'receive_s.exe': ";
    std::cin >> pid;

    int num_threads = 7;
    std::thread threads[num_threads];

    std::thread send_s_thread(send_signals, pid, SIGTERM);

    while (true)
    {
        for (int i = 0; i < num_threads; ++i)
        {
            threads[i] = std::thread(add_signal_to_queue, i);
        }
        for (int i = 0; i < num_threads; ++i)
        {
            threads[i].join();
        }
    }
    
    return 0;
}
