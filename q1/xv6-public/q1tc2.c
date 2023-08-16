#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int pow(int a, int b){
    int result = 1;
    for(int i=0; i<b; i++){
        result *= a;
    }
    return result;
}

int main(int argc, char *argv[]){
    int NCHILD = 2, ITERSEED = 10;
    if(argc > 1){
        NCHILD = atoi(argv[1]);
    }
    if(argc > 2){
        ITERSEED = atoi(argv[2]);
    }

    int child_pids[NCHILD];
    for(int i = 1; i <= NCHILD; i++){
        int pid = fork();
        if(pid == 0){
            // Make blocking syscalls to get scheduled multiple times
            for(int iter = 0; iter <= pow(ITERSEED, i); iter++)
                sleep(1); // TODO: add a remark.

            exit();
        } else {
            child_pids[i - 1] = pid;
        }
    }

    // Sleep for a while to let the children run and exit
    sleep(pow(ITERSEED, NCHILD) + 100);

    for(int i=0; i<NCHILD; i++){
        int scheduledCount = procScheduledCount(child_pids[i]);
        if(scheduledCount != -1){
            printf(1, "Scheduled count for PID: %d => %d\n", child_pids[i], scheduledCount);
        } else {
            printf(1, "Check3: Scheduled count for PID: %d check is invalid\n", child_pids[i]);
            break;
        }
    }

    // Reap children
    for(int i=0; i<NCHILD; i++)
        wait();

    exit();
}