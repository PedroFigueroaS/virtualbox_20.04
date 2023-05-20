// Write a program composed by two processes (use exec and debug them separately):
// - the two processes have one konsole each
// - P1 inputs two numbers A and B 
// - P1 sends via pipe numbers A and B to P2
// - every time you press ctrl-C P1 sends P2 a signal asking P2 to compute (A+B)++ (sum A and B and increment with respect to last time)
// - P2 outputs the result on its console
// - P1 keeps on waiting for input
// - some input stops both P1 and P2 (through signal) and make them terminate.


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


int spawn(char *args_list[])
{
  pid_t child_pid = fork();

  if (child_pid < 0)
  {
    perror("Error while creating fork");
    return 1;
  }

  if (child_pid == 0)
  {
    execvp(args_list[0], args_list);
    perror("exec failed");
    return 1;
  }

  else
  {
    return child_pid;
  }
}

int main()
{
  int fd[2];
  int fd_params[2];
  char read_param[10];
  char write_param[10];
  int pids[2];

  if (pipe(fd) == -1)
  {
    perror("Error while creating pipe");
    return 1;
  }

  int r_param = fd[0];
  sprintf(read_param, "%d", r_param);
  int w_param = fd[1];
  sprintf(write_param, "%d", w_param);

  char *args_list[][256] = {{"/usr/bin/konsole", "-e", "./first", write_param, NULL}, {"/usr/bin/konsole", "-e", "./second", read_param, NULL}};
  const int LENGTH = sizeof args_list / sizeof *args_list;

  for (size_t i = 0; i < LENGTH; i++)
  {
    pids[i] = spawn(args_list[i]);

  }

  // Waits for all children. Waitpid waits for the concrete child you indicate.
  int status;
  wait(&status);

  return 0;
}