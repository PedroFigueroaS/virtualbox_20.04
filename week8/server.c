#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    const char *shm_name = "/AOS";
    int shm_fd;
    int *ptr;

    int message[2];
    shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == 1)
    {
        printf("Shared memory segment failed\n");
        exit(1);
    }

    ftruncate(shm_fd, sizeof(message));
    ptr = mmap(0, sizeof(message), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED)
    {
        printf("Map failed\n");
        return 1;
    }

    message[0] = -1;
    message[1] = -1;
    ptr[0] = message[0];
    ptr[1] = message[1];

    while (1)
    {
        printf("Please, write two integer numbers, first-enter-second, or send two 0 to quit\n");
        scanf("%d", &message[0]);
        scanf("%d", &message[1]);

        ptr[0] = message[0];
        ptr[1] = message[1];

        if ((message[0] == message[1]) && (message[0] == 0))
            break;
    }
    munmap(ptr, sizeof(message));
    return 0;
}