#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#define LEN 10

void sig_handler(int signo)
{
    if (signo == SIGINT){
        printf("received SIGINT\n");

        // Getting the pid of process "second"
        char line[LEN];
        FILE *cmd = popen("pidof -s second", "r");
        fgets(line, LEN, cmd);
        long pid = 0;
        pid = strtoul(line, NULL, 10);

        // Send signal to second
        kill(pid, SIGUSR1);

    }
}

void exit_handler(int signo)
{
    if (signo == SIGUSR2){
        printf("Signal received, exiting"); fflush(stdout);

        char line[LEN];
        FILE *cmd = popen("pidof -s second", "r");
        fgets(line, LEN, cmd);
        long pid = 0;
        pid = strtoul(line, NULL, 10);

        kill(pid, SIGUSR2);
        exit(EXIT_SUCCESS);
    }
}


int main(int argc, char const *argv[])
{
    char input_string[80], send_string[80];
    int write_param;
    sscanf(argv[1], "%d", &write_param);

    if (signal(SIGINT, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");
    
    if (signal(SIGUSR2, exit_handler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");
    
    while (1)
    {
        printf("Please, write two integer numbers, separated by commas (,), or q to quit\n");
        /* to be sure that the previous is executed immediately */
        fflush(stdout);
        /* read a full input line */
        fgets(input_string, 80, stdin);
        write(write_param, input_string, strlen(input_string) + 1);
        /* if the first input char is q, exit  */
        if (input_string[0] == 'q'){
            kill(getpid(), SIGUSR2);
        }
    }
    return 0;
}