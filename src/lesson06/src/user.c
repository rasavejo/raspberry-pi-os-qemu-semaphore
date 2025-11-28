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
// test blocage d'une sémaphore
void test_semaphore_1() {
    call_sys_write("User process\n\r");
    // int pid = call_sys_fork();
    unsigned long semaphore = call_sys_sem_new(1);
    call_sys_write("Semaphore called \n");
    call_sys_write("Asked token \n");
    call_sys_sem_P(semaphore);
    call_sys_write("I am in a critical section\n");
    call_sys_sem_V(semaphore);
    call_sys_write("Sem released \n");
    call_sys_sem_delete(semaphore);
}

// test blocage d'une sémaphore
void test_semaphore_2(){
    unsigned long semaphore = call_sys_sem_new(2);
    int pid = call_sys_fork();
    if (pid < 0) {
        call_sys_write("Error during fork\n\r");
        call_sys_exit();
        return;
    }
    if (pid == 0) {
        call_sys_write("P0 want to take the semaphore\n\r");
        call_sys_sem_P(semaphore);
        call_sys_write("P0 has the token ! yeay !\n\r");
        user_delay(100000000);
        call_sys_write("P0 now wants to leave critical section\n\r");
        call_sys_sem_V(semaphore);
        call_sys_write("P0 has released the token \n\r");
        yield();
    } else {
        call_sys_write("P1 want to take the semaphore\n\r");
        call_sys_sem_P(semaphore);
        call_sys_write("P1 has the token ! yeay !\n\r");
        yield();
        call_sys_write("P1 now wants to leave critical section\n\r");
        call_sys_sem_V(semaphore);
        call_sys_write("P1 has released the token\n\r");
        yield();
    }
}

void test_semaphore_4(){
    unsigned long sem1 = call_sys_sem_new(2);
    unsigned long sem2 = call_sys_sem_new(1);
    // création de la 2eme tache user :
    int pid = call_sys_fork();
    if (pid < 0) {
        call_sys_write("Error during fork\n\r");
        call_sys_exit();
        return;
    }
    if (pid == 0) {
        // création de la 3eme tache user :
        pid = call_sys_fork();
        if (pid < 0) {
            call_sys_write("Error during second fork\n\r");
            call_sys_exit();
        return;
        }
        if( pid == 0){
            // création de la 4eme tache user :
            pid = call_sys_fork();
            if (pid < 0) {
                call_sys_write("Error during third fork\n\r");
                call_sys_exit();
            return;
            }  
        }
    }
    switch (pid){ // PID 1 is skipped ! 
        case 0 : 
            for(int i = 0; i < 5; i++){ yield(); }
            call_sys_write("[PROCESS 0] ask for token SEM2\n\r");
            call_sys_sem_P(sem2);
            call_sys_write("[PROCESS 0] took a token SEM2\n\r");
            call_sys_sem_V(sem2);
            call_sys_write("[PROCESS 0] released token SEM2\n\r");
            break;
        case 2 :
            for(int i = 0; i < 15; i++){ yield(); }
            call_sys_write("[PROCESS 2] ask for token SEM1\n\r");
            call_sys_sem_P(sem1);
            call_sys_write("[PROCESS 2] took a token SEM1\n\r");
            call_sys_sem_V(sem1);
            call_sys_write("[PROCESS 2] released token SEM1\n\r");
            break;
        case 3 :
            for(int i = 0; i < 10; i++){ yield(); }
            call_sys_write("[PROCESS 3] ask for token SEM1\n\r");
            call_sys_sem_P(sem1);
            call_sys_write("[PROCESS 3] took token SEM1\n\r");
            for(int i = 0; i < 25; i++){ yield(); }
            call_sys_sem_V(sem1);
            call_sys_write("[PROCESS 3] released token SEM1\n\r");
            break;
        case 4 :
            call_sys_write("[PROCESS 4] ask for token SEM1\n\r");
            call_sys_sem_P(sem1);
            call_sys_write("[PROCESS 4] took token SEM1\n\r");
            call_sys_write("[PROCESS 4] ask for token SEM2\n\r");
            call_sys_sem_P(sem2);
            call_sys_write("[PROCESS 4] took token SEM2\n\r");
            for(int i = 0; i < 50; i++){ yield();}
            call_sys_sem_V(sem2);
            call_sys_write("[PROCESS 4] released token SEM2\n\r");
            call_sys_sem_V(sem1);
            call_sys_write("[PROCESS 4] released token SEM1\n\r");
            break;
    } 

    while(1)
        yield();

}

void test_semaphore_3(){
    unsigned long semaphore = call_sys_sem_new(2);
    // création de la 2eme tache user :
    int pid = call_sys_fork();
    if (pid < 0) {
        call_sys_write("Error during fork\n\r");
        call_sys_exit();
        return;
    }
    if (pid == 0) {
        // création de la 3eme tache user :
        pid = call_sys_fork();
        if (pid < 0) {
            call_sys_write("Error during second fork\n\r");
            call_sys_exit();
        return;
        }
    }
    switch (pid){ // PID 1 is skipped ! 
        case 0 : 
            call_sys_write("[PROCESS 0] ask for token\n\r");
            call_sys_sem_P(semaphore);
            call_sys_write("[PROCESS 0] took a token\n\r");
            call_sys_sem_V(semaphore);
            call_sys_write("[PROCESS 0] released the token\n\r");
            break;
        case 2 :
            call_sys_write("[PROCESS 2] ask for token\n\r");
            call_sys_sem_P(semaphore);
            call_sys_write("[PROCESS 2] took a token\n\r");
            for(int i = 0; i < 5; i++){
                yield();
            }
            call_sys_sem_V(semaphore);
            call_sys_write("[PROCESS 2] released the token\n\r");
            break;
        case 3 :
            call_sys_write("[PROCESS 3] ask for token\n\r");
            call_sys_sem_P(semaphore);
            call_sys_write("[PROCESS 3] took a token\n\r");
            for(int i = 0; i < 10; i++){
                yield();
            }
            call_sys_sem_V(semaphore);
            call_sys_write("[PROCESS 3] released the token\n\r");
            break;
    } 

    while(1)
        yield();
}

void user_process() {
    call_sys_write("User process\n\r");
    test_semaphore_4();
    loop("");
}