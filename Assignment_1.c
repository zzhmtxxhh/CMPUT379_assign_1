
#include <unistd.h>
#include <signal.h>



void printpid(){
  int pid;
  pid = getpid();
  printf("Own pid :  %d\n", pid );
  fflush(NULL);
}

int enter_target_pid(){
  int target_int_pid;
  char target_pid[100];
  printf("%s\n", "please enter target pid :");
  fflush(NULL);
  fgets(target_pid,100,stdin);
  sscanf(target_pid, "%d", &target_int_pid);
  fflush(NULL);
  printf("The target pid you enter is : %d\n", target_int_pid);
  fflush(NULL);
  return target_int_pid;
}

void sig_handler(int signo){
 if (signo == SIGUSR1){
     printf("Signal caught\n");
     fflush(NULL);
 }
}

void send_signal (int pid){
    printf("waiting to send");
    int ret;
    ret = kill(pid, SIGUSR1);
    printf("ret : %d",ret);
    fflush(NULL);

}


void operation(int target_pid){
  char msn[100];
  int pid = target_pid;
  printf("opeartion pid :%d\n",pid );

  while(1){

  printf("Enter the message: ");
  fflush(NULL);
  fgets(msn,100,stdin);
  printf("Your message is : %s",msn);
  fflush(NULL);

}

}


int main(){

  int target_pid;
  printpid();
  target_pid = enter_target_pid();
  printf("afdadf da faf : %d\n", target_pid);
  fflush(NULL);
  send_signal(target_pid);


  if (signal(SIGUSR1,sig_handler) == SIG_ERR){
    printf("\n cannot catch the signal\n");
    fflush(NULL);
  }
  while(1){
    printf("looping here");
    fflush(NULL);
  sleep(1);
  }
  // send_signal(target_pid);


  // operation(target_pid);
  return 0;
}
