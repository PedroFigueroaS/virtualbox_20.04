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
    int i, shm_fd;
    int *ptr;
    int message[2];
    double mean;

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

    while (1)
    {
        int n1, n2;

        n1 = ptr[0];
        n2 = ptr[1];

        mean = (n1 + n2) / 2.0;
        printf("mean value is: %f, sum is: %d\n", mean, n1 + n2);

        if ((n1 == n2) && (n1 == 0))
            break;

        sleep(5);
    }

    if (shm_unlink(shm_name) == 1)
    {
        printf("Error removing %s\n", shm_name);
        exit(1);
    }
    return 0;
}