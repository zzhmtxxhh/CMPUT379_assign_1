
#include <stdio.h>
#include <unistd.h>



void printpid(){
  int pid;
  pid = getpid();
  printf("Own pid :  %d\n", pid );
}

int enter_target_pid(){
  int target_int_pid;
  char target_pid[100];
  printf("%s\n", "please enter target pid :");
  fgets(target_pid,100,stdin);
  sscanf(target_pid, "%d", &target_int_pid);
  printf("The target pid you enter is : %d\n", target_int_pid);
  return target_int_pid;
}


void operation(int target_pid){
  char msn[100];
  int pid = target_pid;
  printf("opeartion pid :%d\n",pid );

  while(1){
  printf("Enter the message: ");
  fgets(msn,100,stdin);
  printf("Your message is : %s",msn);
}






}

int main(){

  int target_pid;
  printpid();
  target_pid = enter_target_pid();
  operation(target_pid);
  return 0;
}
