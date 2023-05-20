#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int n1, n2;
int count = 0;

void sig_handler(int signo)
{
    if (signo == SIGUSR1){
        int sum = n1 + n2 + count;
        printf("Sum: %d \n", sum);

        count++;
    }
}

void exit_handler(int signo)
{
    if (signo == SIGUSR2){
        printf("Signal received, exiting"); fflush(stdout);
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char const *argv[])
{
    char str1[80], str2[80];
    char format_string[80] = "%d,%d";
    double mean;
    int read_param;
    sscanf(argv[1], "%d", &read_param);

    if (signal(SIGUSR1, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");

    if (signal(SIGUSR2, exit_handler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");

    while (1)
    {
        read(read_param, str1, 80);

        /* read numbers from input line */
        sscanf(str1, format_string, &n1, &n2);
        mean = (n1 + n2) / 2.0;
        printf("mean value is: %f, sum is: %d\n", mean, n1 + n2);
    }
    return 0;
}
