#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int counter_sigusr1 = 0;
int counter_sigusr2 = 0;

void handler(int signal) {
  printf("Signal!!!\n");
  if (signal == SIGUSR2) {
    counter_sigusr2++;
    printf("counter2 %d\n", counter_sigusr2);
  } else if (signal == SIGUSR1) {
    
    counter_sigusr1++;
    printf("coutner1 %d\n", counter_sigusr1);
  }
}

int main() {
  printf("Own PID: %d\n", getpid());
  struct sigaction sa;
  sigset_t x;
  sa.sa_handler = handler;
  sigemptyset(&x);
  sigaddset(&x, SIGUSR1);
  sigaddset(&x, SIGUSR2);
  sa.sa_flags = 0;
  sa.sa_flags = SA_RESTART;


 

  sigaction(SIGUSR2, &sa, NULL);
  sigaction(SIGUSR1, &sa, NULL);
  while (1) {
    char buffer[4096];
    while (1) {
      scanf("%s", buffer);
      if(strcmp(buffer, "done") == 0){
        printf("Counter1: %d", counter_sigusr1);
        printf("Counter2: %d", counter_sigusr2);
        return 0;
      }
      /* if (errno == EINTR) { */
      /*   errno = 0; */
      /*   continue; */
      /* } */
      printf("Output: %s\n", buffer);
    }
  }
  return 0;
}