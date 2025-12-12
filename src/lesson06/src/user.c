#include "user.h"
#include "printf.h"
#include "user_sys.h"

void loop(char *str) {
    char buf[2] = {""};
    while (1) {
        for (int i = 0; i < 5; i++) {
            buf[0] = str[i];
            call_sys_write(buf);
            user_delay(1000000);
        }
    }
}

void v1_dynamic_test_2() {
    call_sys_write("We will test the v1 through dynamic testing : 2 semaphore, 1 and 2 tokens and 4 tasks\n");
    unsigned long sem1 = call_sys_sem_new(2);
    unsigned long sem2 = call_sys_sem_new(1);
    int pid = call_sys_fork();
    if(pid == 0){
        pid = call_sys_fork();
        if(pid == 0){
            pid = call_sys_fork();
        }
    }
    if (pid < 0) {
        call_sys_write("Error during fork\n\r");
        call_sys_exit();
        return;
    }
    for (int i = 0; i < 3; i++) {
        switch(pid){
            case 0 :
                user_delay(100000000);
                call_sys_write("[P0] -- Asking token SEM1\n");
                call_sys_sem_P(sem1);
                user_delay(200000000);
                call_sys_write("[P0] -- In critical section SEM1\n");
                user_delay(400000000);
                call_sys_write("[P0] - Asking token SEM2\n");
                call_sys_sem_P(sem2);
                call_sys_write("[P0] - In critical section SEM2\n");
                user_delay(400000000);
                call_sys_write("[P0] - Release the token SEM2\n");
                call_sys_sem_V(sem2);
                user_delay(200000000);
                call_sys_write("[P0] -- Release the token SEM1\n");
                call_sys_sem_V(sem1);
                user_delay(200000000);
                call_sys_write("[P0] Finished \n");
                user_delay(30000000);
                break;
            case 2 : 
                user_delay(20000000);
                call_sys_write("[P2] - Asking token SEM2\n");
                call_sys_sem_P(sem2);
                user_delay(20000000);
                call_sys_write("[P2] - In critical section SEM2\n");
                user_delay(400000000);
                call_sys_write("[P2] - Release the token SEM2\n");
                call_sys_sem_V(sem2);
                call_sys_write("[P2] - Finished \n");
                user_delay(20000000);
                break;
            case 3 : 
                user_delay(10000000);
                call_sys_write("[P3] -- Asking token SEM1\n");
                call_sys_sem_P(sem1);
                user_delay(10000000);
                call_sys_write("[P3] -- In critical section SEM1\n");
                user_delay(400000000);
                call_sys_write("[P3] -- Release the token SEM1 \n");
                call_sys_sem_V(sem1);
                user_delay(40000000);
                call_sys_write("[P3] -- Finished \n");
                break;
            case 4 : 
                user_delay(100000000);
                call_sys_write("[P4] -- Asking token SEM1\n");
                call_sys_sem_P(sem1);
                user_delay(200000000);
                call_sys_write("[P4] -- In critical section SEM1\n");
                user_delay(400000000);
                call_sys_write("[P4] - Asking token SEM2\n");
                call_sys_sem_P(sem2);
                call_sys_write("[P4] - In critical section SEM2\n");
                user_delay(400000000);
                call_sys_write("[P4] - Release the token SEM2\n");
                call_sys_sem_V(sem2);
                user_delay(200000000);
                call_sys_write("[P4] -- Release the token SEM1\n");
                call_sys_sem_V(sem1);
                user_delay(200000000);
                call_sys_write("[P4] Finished \n");
                user_delay(30000000);
                break;
        }; 
    }
}


void v1_dynamic_test_1() {
    call_sys_write("We will test the v1 through dynamic testing : 1 semaphore, 2 tokens and 5 tasks \n");
    unsigned long semaphore = call_sys_sem_new(2);
    int pid = call_sys_fork();
    if(pid == 0){
        pid = call_sys_fork();
        if(pid == 0){
            pid = call_sys_fork();
            if(pid == 0){
                pid = call_sys_fork();
            }
        }
    }
    if (pid < 0) {
        call_sys_write("Error during fork\n\r");
        call_sys_exit();
        return;
    }
    for (int i = 0; i < 6; i++) {
        switch(pid){
            case 0 :
                user_delay(100000000);
                call_sys_write("[P0] Asking token\n");
                call_sys_sem_P(semaphore);
                user_delay(200000000);
                call_sys_write("[P0] In critical section\n");
                user_delay(400000000);
                call_sys_write("[P0] Release the token \n");
                call_sys_sem_V(semaphore);
                call_sys_write("[P0] Finished \n");
                user_delay(30000000);
                break;
            case 2 : 
                user_delay(20000000);
                call_sys_write("[P2] Asking token\n");
                call_sys_sem_P(semaphore);
                user_delay(20000000);
                call_sys_write("[P2] In critical section\n");
                user_delay(400000000);
                call_sys_write("[P2] Release the token \n");
                call_sys_sem_V(semaphore);
                call_sys_write("[P2] Finished \n");
                user_delay(20000000);
                break;
            case 3 : 
                user_delay(10000000);
                call_sys_write("[P3] Asking token\n");
                call_sys_sem_P(semaphore);
                user_delay(10000000);
                call_sys_write("[P3] In critical section\n");
                user_delay(400000000);
                call_sys_write("[P3] Release the token \n");
                call_sys_sem_V(semaphore);
                user_delay(40000000);
                call_sys_write("[P3] Finished \n");
                break;
            case 4 : 
                user_delay(100000000); 
                call_sys_write("[P4] Asking token\n");
                call_sys_sem_P(semaphore);
                call_sys_write("[P4] In critical section\n");
                user_delay(400000000);
                call_sys_write("[P4] Release the token \n");
                call_sys_sem_V(semaphore);
                user_delay(300000000); 
                call_sys_write("[P4] Finished \n");
                user_delay(200000000);
                break;
            case 5 : 
                user_delay(300000000);  
                call_sys_write("[P5] Asking token\n");
                user_delay(200000000);
                call_sys_sem_P(semaphore);
                call_sys_write("[P5] In critical section\n");
                user_delay(400000000);
                call_sys_write("[P5] Release the token \n");
                call_sys_sem_V(semaphore);
                call_sys_write("[P5] Finished \n");
                user_delay(10000000);
                break;

        }; 
    }
}


void v1_dynamic_test_0() {
    call_sys_write("We will test the v1 through dynamic testing : 1 semaphore, 1 token and 2 tasks \n");
    unsigned long semaphore = call_sys_sem_new(1);
    int pid = call_sys_fork();
    switch(pid){
        case 0 :
            call_sys_write("[P0] Asking token\n");
            call_sys_sem_P(semaphore);
            call_sys_write("[P0] In critical section\n");
            call_sys_write("[P0] Release the token \n");
            call_sys_sem_V(semaphore);
            call_sys_write("[P0] Finished \n");
            break;
        default : 
            call_sys_write("[P2] wait a delay : \n");       
            call_sys_write("[P2] Asking token\n");
            call_sys_sem_P(semaphore);
            call_sys_write("[P2] In critical section\n");
            user_delay(400000000);
            call_sys_write("[P2] Release the token \n");
            call_sys_sem_V(semaphore);
            call_sys_write("[P2] Finished \n");
            break;
    }; 
    
}

void user_process() {
    call_sys_write("Inside user process \n");
    v1_dynamic_test_2();
    call_sys_write("end of user process \n");
    loop("");
}