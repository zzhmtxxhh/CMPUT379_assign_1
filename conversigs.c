#include <sys/types.h>
#include <sys/signal.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

#define _GNU_SOURCE
char *incoming_binary[8];
char *incoming_string[1024];
// incoming_binary[0] = '\0';
int flag = 0;
int signal_counter = 0;
int counter_sig1= 0;
int counter_sig2 = 0; //keeps track of how many signals process recieves

volatile sig_atomic_t stop;

static volatile int signalPid = -1;
void get_pid(int sig, siginfo_t *info, void *context){
    signalPid = info->si_pid;
}

void printPID(){
    int pid = getpid();
    printf("Own PID: %d\n", pid);
}



void handler(int signal) {
  // printf("Signal!!!\n");
  if (signal == SIGUSR1) {
    // printf("0\n");
    counter_sig1++;
    signal_counter++;
    strcat(incoming_binary,"0");
    // printf("coutner_sig_1: %d\n", counter_sig1);
  } else if (signal == SIGUSR2) {
    // printf("1\n");
    counter_sig2++;
    signal_counter++;
    strcat(incoming_binary,"1");
    // printf("coutner_sig_2: %d\n", counter_sig2);
  }
  if (strlen(incoming_binary)>= 8){
    // printf("%s\n", incoming_binary);
    // printf("%s\n", "strtol");
    char c = strtol(incoming_binary, 0, 2);
    printf("%c", c);
    
    fflush(stdout);
    strcat(incoming_string,&c);
    memset(incoming_binary, 0, 8);
  }

  //
  // int stirng_len = strlen(incoming_string);
  // for (int k = 0 ; i <= string_len; i++){
  //     printf("%s\n",incoming_string[k]);
  // }
}

// void sigHandler(int signo){
//     if (signo == SIGUSR1){
//         printf("recieved SIGUSR1");
//         ++counter_usr1;
//     }
//     else if(signo == SIGUSR2){
//         printf("recieved SIGUSR2");
//         ++counter_usr2;
//     }
// }

static void HandleHostSignal(void){
    // struct sigaction act;
    // sigemptyset(&act.sa_mask); //clear/initialize the sa mask, which signals to block, set to none.
    // //act.sa_flags = SA_SIGINFO;
    // act.sa_flags = 0;
    // act.sa_handler = sigHandler;
    // act.sa_sigaction = get_pid;
    // //act.sa_flags |= SA_RESTART;
    // sigaction(SIGUSR1, &act, NULL);
    // sigaction(SIGUSR2, &act, NULL);

  struct sigaction sa;
  sa.sa_handler = handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_flags = SA_RESTART;

  sigaction(SIGUSR1, &sa, NULL);
  sigaction(SIGUSR2, &sa, NULL);
  // printf("len of incoming string %d\n", strlen(incoming_string));
}

void sendSignal(int pid, int signo){
    int ret;
    if (signo == SIGUSR1){
        ret = kill(pid, SIGUSR1);
    }
    else if (signo == SIGUSR2){
        ret = kill(pid, SIGUSR2);
    }
    printf("ret: %d\n", ret);
}

char* stringToBinary(char* s) {
    if(s == NULL) return 0; /* no input string */
    size_t len = strlen(s);
    char *binary = malloc(len*8 + 1); // each char is one byte (8 bits) and + 1 at the end for null terminator
    binary[0] = '\0';
    for(size_t i = 0; i < len; ++i) {
        char ch = s[i];
        for(int j = 7; j >= 0; --j){
            if(ch & (1 << j)) {
                strcat(binary,"1");
            } else {
                strcat(binary,"0");
            }
        }
    }
    return binary;
}

void linefeed(int pid){
  int target_pid = pid;
  sendSignal(target_pid, SIGUSR1);
  sendSignal(target_pid, SIGUSR1);
  sendSignal(target_pid, SIGUSR1);
  sendSignal(target_pid, SIGUSR1);
  sendSignal(target_pid, SIGUSR2);
  sendSignal(target_pid, SIGUSR2);
  sendSignal(target_pid, SIGUSR1);
  sendSignal(target_pid, SIGUSR2);
}



int main(int argc, char *argv[]){
    fflush(stdout);
    printPID();
    HandleHostSignal();

    int target_pid;
    scanf("%d", &target_pid);
    printf("Target pid: %d\n", target_pid);

    while(1){

        char buf_in[MAX_INPUT];
        while(1){

            //sleep(1);
            scanf("%s", buf_in);
            while((getchar())!= '\n');
            //if(errno == EINTR) printf("scanf interrupted");
            if (strcmp(buf_in, "done") == 0){      //exit program
                printf("counter_usr1 = %d\n", counter_sig1);
                printf("counter_usr2 = %d\n", counter_sig2);
                return 0;
            }
            printf("Output: %s\n", buf_in);

            size_t length = strlen(buf_in);
            //printf("%zu\n", length);
            //printf("%s\n", stringToBinary(buf_in));

            char* binary = stringToBinary(buf_in);

            for (int i = 0; i < length*8; i++){
                char b = (char)binary[i];
                if(b == '0'){
                    usleep(20);
                    sendSignal(target_pid, SIGUSR1);
                    printf("sent SIGUSR1\n");
                }else if (b == '1'){
                    usleep(20);
                    sendSignal(target_pid, SIGUSR2);
                    printf("sent SIGUSR2\n");
                }
            }
            // printf("%s\n","linefeed" );
            // linefeed(target_pid);
        }
    }
    return 0;
}
