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

int flag = 0;
char *incoming_binary[8];
char *incoming_string[1024];

int signal_counter = 0;
int counter_usr1= 0;
int counter_usr2 = 0; //keeps track of how many signals process recieves

//volatile sig_atomic_t stop;

static volatile int signalPid = -1;
void get_pid(int sig, siginfo_t *info, void *context){
    signalPid = info->si_pid;
}

void printPID(){
    int pid = getpid();
    printf("Own PID: %d\n", pid);
}


// void sigHandler(int signo){
//     if (signo == SIGUSR1){
//         //printf("recieved SIGUSR1 = 0\n");
//         //printf("PID of signal = %d\n", signalPid);
//         ++counter_usr1;
//         strcat(incoming_binary, "0");
//     }
//     else if(signo == SIGUSR2){
//         //printf("recieved SIGUSR2 = 1\n");
//         //printf("PID of signal = %d\n", signalPid);
//         ++counter_usr2;
//         strcat(incoming_binary, "1");
//     }
//     if (strlen(incoming_binary)>= 8){
//     // printf("%s\n", incoming_binary);
//     // printf("%s\n", "strtol");
//         char c = strtol(incoming_binary, 0, 2);
//         printf("%c", c);
    
//         fflush(stdout);
//         strcat(incoming_string,&c);
//         memset(incoming_binary, 0, 8);
//   }
// }

void sigHandler(int signo){
    if(signo == SIGUSR1){
        signal_counter++;
        printf("signal_counter %d\n", signal_counter);
        if (signal_counter == 8){
            printf("%s\n", "enter 7");
            ualarm(500000, 500000);
            signal_counter = 0;

        }
    }
    if (strlen(incoming_binary)== 8){
        printf("incoming binary: &s\n", incoming_binary);
        char c = strtol(incoming_binary, 0, 2);
        printf("################     %c      ###############\n", c);
    
        fflush(stdout);
        strcat(incoming_string,&c);
        fflush(stdout);
        memset(incoming_binary, 0, 8);
  }

    if (signo == SIGALRM ){
        flag = 1;

        if (signal_counter == 1){
            printf("Received %s\n", "0");
            strcat(incoming_binary, "0");
        }
        else if( signal_counter == 5){
            printf("received %s\n", "1");
            strcat(incoming_binary, "1");
        }
        signal_counter = 0;
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
    //printf("ret: %d\n", ret);
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

    sa.sa_handler = sigHandler;
    sigemptyset(&sa.sa_mask); //clear/initialize the sa mask, which signals to block, set to none

    sa.sa_flags = SA_SIGINFO;
    //act.sa_flags |= SA_RESTART;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

}

void eightBit(int pid){
    usleep(249125);
    sendSignal(pid, SIGUSR1);
    usleep(250);
    sendSignal(pid, SIGUSR1);
    usleep(250);
    sendSignal(pid, SIGUSR1);
    usleep(250);
    sendSignal(pid, SIGUSR1);
    usleep(250);
    sendSignal(pid, SIGUSR1);
    usleep(250);
    sendSignal(pid, SIGUSR1);
    usleep(250);
    sendSignal(pid, SIGUSR1);
    usleep(250);
    sendSignal(pid, SIGUSR1);
    usleep(249125);
}

static void HandleAlrmSignal(void){
    struct sigaction sa;
    sigset_t set;
    sigaddset(&set, SIGUSR1);
    sa.sa_handler = sigHandler;
    sigemptyset(&sa.sa_mask); //clear/initialize the sa mask, which signals to block, set to none
    sa.sa_mask = set;
    sa.sa_flags = SA_SIGINFO;
    //act.sa_flags |= SA_RESTART;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGALRM, &sa, NULL);
    // sigaction(SIGUSR2, &sa, NULL);

}


int main(int argc, char *argv[]){
    fflush(stdout);
    HandleAlrmSignal();
    

    printPID();
    
  
    int target_pid;
    scanf("%d", &target_pid);
    printf("Target pid: %d\n", target_pid);
    getchar();
    
    
    while(1){
        //printf("1st while loop\n");
        char buf_in[MAX_INPUT];
        while(1){
            fgets(buf_in, MAX_INPUT, stdin);

        
            
            if(errno == EINTR) {
                //printf("In errno\n");
                errno = 0;
                continue;
            }
            if (strcmp(buf_in, ".\n") == 0){      //exit program
                printf("counter_usr1 = %d\n", counter_usr1);
                printf("counter_usr2 = %d\n", counter_usr2);
                return 0;
            }


            printf("Output: %s\n", buf_in);
            printf("%s\n", "reset alarm");
            eightBit(target_pid);
            usleep(500000);

            size_t length = strlen(buf_in);

            printf("error %s\n", "what happen");
            usleep(500000);
            char* binary = stringToBinary(buf_in);

            for (int i = 0; i < length*8; i++){
                char b = (char)binary[i];
                if(b == '0'){
                    usleep(250000);
                    sendSignal(target_pid, SIGUSR1);
                    printf("sent SIGUSR1\n");
                    fflush(stdout);

                    
                    usleep(250000);

                    
                }else if (b == '1'){
                    usleep(249000);
                    sendSignal(target_pid, SIGUSR1);
                    usleep(500);
                    sendSignal(target_pid, SIGUSR1);
                    usleep(500);
                    sendSignal(target_pid, SIGUSR1);
                    usleep(500);
                    sendSignal(target_pid, SIGUSR1);
                    usleep(500);
                    sendSignal(target_pid, SIGUSR1);
                    usleep(249000);

                    printf("sent 5 SIGUSR1\n");
                    
                    fflush(stdout);
                }
            }
        }
    }       
    return 0;
}