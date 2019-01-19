






void printpid(){
  int pid;
  pid = getpid();
  printf("Own pid :  %d\n", pid );
}


int main(){
  int *target_pid;
  printpid();
  printf("%s\n", "please enter target pid :");
  scanf("%d", target_pid);
  printf("The target pid you enter is : %d\n", *target_pid );

  return 0;
}
