#include <sys/types.h>
#include <sys/signal.h>
#include <signal.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <bits/siginfo.h>
#include <errno.h>

//#define MAX_INPUT 4096


char *incoming_binary[8];
char *incoming_string[1024];
int mark_flag = 0 ;

int signal_counter = 0;
int counter_usr1= 0;
int counter_usr2 = 0; //keeps track of how many signals process recieves

void checkmess(int num){
    int char_num = num;
    if(char_num<=127 && char_num>=1 && mark_flag == 0){
        printf("! ");
        mark_flag = 1;
    }else if (char_num>127 && char_num<1 && mark_flag == 0){
        printf("? ");
        mark_flag = 1;
    }
    if(char_num == 10){
        mark_flag = 0;
    }

}


static volatile int signalPid = -1;
void get_pid(int sig, siginfo_t *info, void *context){
    signalPid = info->si_pid;
}

void printPID(){
    int pid = getpid();
    printf("Own PID: %d\n", pid);
}


void sigHandler(int signo){
    if (signo == SIGUSR1){
        ++counter_usr1;
        strcat(incoming_binary, "0");
    }
    else if(signo == SIGUSR2){
        ++counter_usr2;
        strcat(incoming_binary, "1");
    }
    if (strlen(incoming_binary)>= 8){
        char c = strtol(incoming_binary, 0, 2);
        int char_num = (int)(c);
        // printf("%d",char_num);
        checkmess(char_num);
        fflush(stdout);

        printf("%c", c);
        fflush(stdout);
  
        strcat(incoming_string,&c);
        memset(incoming_binary, 0, 8);
  }
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

static void HandleHostSignal(void){
    struct sigaction sa;
    sigset_t set;
    sigemptyset(&sa.sa_mask);
    sigaddset(&set,SIGUSR1);
    sigaddset(&set,SIGUSR2);
    sa.sa_handler = sigHandler;
    sa.sa_mask = set;
     //clear/initialize the sa mask, which signals to block, set to none
    sa.sa_flags = SA_SIGINFO;
    //act.sa_flags |= SA_RESTART;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
}


int main(int argc, char *argv[]){
    fflush(stdout);
    HandleHostSignal();
    printPID();
  
    int target_pid;
    scanf("%d", &target_pid);
    printf("Target pid: %d\n", target_pid);
    getchar();
    
    
    while(1){
        char buf_in[MAX_INPUT];
        while(1){
            fgets(buf_in, MAX_INPUT, stdin);
            if(errno == EINTR) {
                // printf("in error\n");
                errno = 0;
                continue;
            }
            if (strcmp(buf_in, ".\n") == 0){      //exit program
                printf("counter_usr1 = %d\n", counter_usr1);
                printf("counter_usr2 = %d\n", counter_usr2);
                return 0;
            }
            printf("Output: %s\n", buf_in);
            
            size_t length = strlen(buf_in);
            char* binary = stringToBinary(buf_in);
            for (int i = 0; i < length*8; i++){
                char b = (char)binary[i];
                if(b == '0'){
                    sendSignal(target_pid, SIGUSR1);
                    printf("sent SIGUSR1 = 0\n");
                    usleep(200);
                    fflush(stdout);
                }else if (b == '1'){
                    sendSignal(target_pid, SIGUSR2);
                    printf("sent SIGUSR2 = 1\n");
                    usleep(200);
                    fflush(stdout);
                }
            }
        }
    }       
    return 0;
}