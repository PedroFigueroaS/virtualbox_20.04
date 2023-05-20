#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#define SEM_PATH_WRITER "/sem_AOS_writer"
#define SEM_PATH_READER "/sem_AOS_reader"

int main(int argc, char *argv[])
{
    const char *shm_name = "/AOS";
    int i, shm_fd;
    int *ptr;
    int message[2];
    double mean;
    sem_t *sem_id_writer;
    sem_t *sem_id_reader;

    shm_fd = shm_open(shm_name, O_RDONLY, 0666);
    if (shm_fd == 1)
    {
        printf("Shared memory segment failed\n");
        exit(1);
    }

    ptr = mmap(0, sizeof(message), PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED)
    {
        printf("Map failed\n");
        return 1;
    }

    sem_id_writer = sem_open(SEM_PATH_WRITER, 0);
    if(sem_id_writer== (void*)-1){
        perror("sem_open failure");
        exit(1);
    }

    sem_id_reader = sem_open(SEM_PATH_READER, 0);
    if(sem_id_reader== (void*)-1){
        perror("sem_open failure");
        exit(1);
    }

    while (1)
    {
        int n1, n2;

        printf("[CLIENT] Waiting for READER semaphore\n"),
        sem_wait(sem_id_reader);
        printf("[CLIENT] READER entered!\n");

        n1 = ptr[0];
        n2 = ptr[1];

        printf("[CLIENT] Leaving the WRITER semaphore\n");
        sleep(10);
        sem_post(sem_id_writer);
        printf("[CLIENT] Semaphore WRITER unlocked\n");

        mean = (n1 + n2) / 2.0;
        printf("mean value is: %f, sum is: %d\n", mean, n1 + n2);

        if ((n1 == n2) && (n1 == 0))
            break;
    }

    if (shm_unlink(shm_name) == 1)
    {
        printf("Error removing %s\n", shm_name);
        exit(1);
    }

    sem_close(sem_id_reader);
    sem_close(sem_id_writer);
    sem_unlink(SEM_PATH_READER);
    sem_unlink(SEM_PATH_WRITER);

    return 0;
}
