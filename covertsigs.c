#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>


volatile sig_atomic_t stop;

static volatile int signalPid = -1;
void get_pid(int sig, siginfo_t *info, void *context){
    signalPid = info->si_pid;
}

void sig_handler(int signo){
 if (signo == SIGUSR1){
     printf("Signal caught\n");
     fflush(NULL);
     //stop = 1;
 }
}

void printPID(){
    int pid = getpid();
    printf("Own PID: %d\n", pid);
}

void send_signal (int pid){
    int ret;
    ret = kill(pid, SIGUSR1);
    printf("ret : %d",ret);

}

static void HandleHostSignal(void)
{
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sa.sa_handler = sig_handler;
    sa.sa_sigaction = get_pid;
    sigaction(SIGUSR1, &sa, NULL);
}

int main(){
    printPID();
    int target_pid;

    
    HandleHostSignal();
    scanf("%d", &target_pid);
    printf("%d\n", target_pid);
    send_signal(target_pid);
    
    //raise(SIGUSR1);
    //send_signal(target_pid);
    // if (signal(SIGUSR1, sig_handler) == SIG_ERR){
    //     printf("Can't catch SIGUSR1\n");
    // }

    int check = 0;
    while(1){
        
        sleep(1);
        while(signalPid != -1 && check != 1){
            printf("PID of signal sender = %d\n", signalPid);
            check = 1;
            break;
        }


    }                        //wait for a signal
    //printf("PID of signal sender = %d\n", signalPid);
    // sigemptyset(&sa.sa_mask);
    // sa.sa_flags = SA_RESTART;


    // signal(SIGUSR1, sig_handler);
    // //kill(target_pid, SIGUSR1);




    return 0;
}