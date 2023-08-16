#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(){
    // Fork a child process
    int pid = fork();
    if(pid == 0){
        for(int i=0; i<20; i++){
            sleep(1);
        }
        exit();
    }

    sleep(100);
    int scheduledCount = procScheduledCount(pid);
    if(scheduledCount != -1){
        printf(1, "Check1: Scheduled count for PID: %d => %d\n", pid, scheduledCount);
    } else {
        printf(1, "Check1: Scheduled count for PID: %d check is invalid\n", pid);
    }

    // Reap child
    wait();

    scheduledCount = procScheduledCount(pid);
    if(scheduledCount == -1){
        printf(1, "Check2: Scheduled count for PID: %d check is invalid\n", pid);
    } else {
        printf(1, "Check2: Scheduled count for PID: %d => %d\n", pid, scheduledCount);
    }

    // Fork another child process : Its PCB should be the same as the PCB of the previous child
    int pid1 = fork();
    if(pid1 == 0){
        exit();
    }

    sleep(100);
    scheduledCount = procScheduledCount(pid1);
    if(scheduledCount != -1){
        printf(1, "Check3: Scheduled count for PID: %d => %d\n", pid1, scheduledCount);
    } else {
        printf(1, "Check3: Scheduled count for PID: %d check is invalid\n", pid1);
    }

    // Reap child
    wait();

    exit();
}
