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

// init
char *incoming_binary[8];
char *incoming_string[MAX_INPUT];
int mark_flag = 0 ;

int signal_counter = 0;
int counter_usr1= 0;
int counter_usr2 = 0; //keeps track of how many signals process recieves

// error checking must be valid ascii code
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

void printPID(){
    // print process own pid
    int pid = getpid();
    printf("single -- single\n");
    printf("Own PID: %d\n", pid);
}


void sigHandler(int signo){
    // signal handler
    if(signo == SIGUSR1){
        // recevie SIGUSR 1 and increment signal counter
        signal_counter++;
        if (signal_counter == 8){
            // receive 8 bit to initialize alarm
            ualarm(50000, 50000);
            signal_counter = 0;
        }
    }
    if (strlen(incoming_binary)== 8){
        // receive 8 bit info convert 8 bit info in to character
        char c = strtol(incoming_binary, 0, 2);
        int char_num = (int)(c);
        //error checking
        checkmess(char_num);
        printf("%c", c);
    
        fflush(stdout);
        strcat(incoming_string,&c);
        fflush(stdout);
        memset(incoming_binary, 0, 8);
  }

    if (signo == SIGALRM ){
        // SIGALRAM timer in 50000 micro seconds
        if (signal_counter == 1){
            //receive 1 signal ---> 0 
            strcat(incoming_binary, "0");
        }
        else if( signal_counter == 5){
             //receive 5 signals ---> 5
            strcat(incoming_binary, "1");
        }
        //reset every 50000 micro seconds
        signal_counter = 0;
    }

}



// input: target process pid and signal number
// send sigal to target pid
void sendSignal(int pid, int signo){
    int ret;
    if (signo == SIGUSR1){
        ret = kill(pid, SIGUSR1);
    }
    else if (signo == SIGUSR2){
        ret = kill(pid, SIGUSR2);
    }
}

/*The below function was taken from https://stackoverflow.com/questions/41384262/convert-string-to-binary-in-c on Jan 31, 2019.
The author of the code is gowrath.*/
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

// initialize alarm signal total time = 50000 micro seconds
void eightBit(int pid){
    usleep(24912);
    sendSignal(pid, SIGUSR1);
    usleep(25);
    sendSignal(pid, SIGUSR1);
    usleep(25);
    sendSignal(pid, SIGUSR1);
    usleep(25);
    sendSignal(pid, SIGUSR1);
    usleep(25);
    sendSignal(pid, SIGUSR1);
    usleep(25);
    sendSignal(pid, SIGUSR1);
    usleep(25);
    sendSignal(pid, SIGUSR1);
    usleep(25);
    sendSignal(pid, SIGUSR1);
    usleep(24913);
}

//SIGALRM SIGUSR1 signal handler
static void HandleAlrmSignal(void){
    struct sigaction sa;
    sigset_t set;
    // sigaction mask
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGALRM);
    sa.sa_handler = sigHandler;
    sigemptyset(&sa.sa_mask); //clear/initialize the sa mask, which signals to block, set to none
    sa.sa_mask = set;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGALRM, &sa, NULL);

}

// this is main function
int main(int argc, char *argv[]){
    fflush(stdout);
    //sighandler
    HandleAlrmSignal();
    printPID();
    
    int target_pid;
    scanf("%d", &target_pid);//get input pid
    printf("Target pid: %d\n", target_pid);
    getchar();
    
    
    while(1){
        char buf_in[MAX_INPUT];
        while(1){
            fgets(buf_in, MAX_INPUT, stdin);
            // get user message input

            if(errno == EINTR) {
                errno = 0;
                continue;
            }

            if (strcmp(buf_in, ".\n") == 0){      //exit program
                printf("counter_usr1 = %d\n", counter_usr1);
                printf("counter_usr2 = %d\n", counter_usr2);
                return 0;
            }

            printf("Your input: %s", buf_in);
            // init 
            eightBit(target_pid);
            usleep(50000);

            size_t length = strlen(buf_in);

            usleep(50000);
            // string to binary
            char* binary = stringToBinary(buf_in);
            // convert string to binary
            for (int i = 0; i < length*8; i++){
                char b = (char)binary[i];
                if(b == '0'){
                    // bit = 0 then send 1 SIGUSR1 to target pid
                    usleep(25000);
                    sendSignal(target_pid, SIGUSR1);
                    fflush(stdout);
                    usleep(25000); 
                }
                else if (b == '1'){
                    usleep(24900);
                     // bit = 1 then send 5 SIGUSR1 to target pid
                    sendSignal(target_pid, SIGUSR1);
                    usleep(50);
                    sendSignal(target_pid, SIGUSR1);
                    usleep(50);
                    sendSignal(target_pid, SIGUSR1);
                    usleep(50);
                    sendSignal(target_pid, SIGUSR1);
                    usleep(50);
                    sendSignal(target_pid, SIGUSR1);
                    usleep(24900);
                    fflush(stdout);
                }
            }
        }
    }       
    return 0;
}