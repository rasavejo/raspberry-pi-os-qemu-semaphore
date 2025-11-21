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

void user_process_t1() {
    call_sys_write("User process\n\r");
    // int pid = call_sys_fork();
    unsigned long semaphore = call_sys_sem_new(1);
    while (1) {

        call_sys_write("Semaphore called \n");
        call_sys_write("Asked token \n");
        call_sys_sem_P(semaphore);
        call_sys_write("autoroutepardon! \n");
        call_sys_sem_V(semaphore);
        call_sys_write("Sem released \n");
    }
    call_sys_sem_delete(semaphore);
}
void user_process() {
    call_sys_write("User process\n\r");
    unsigned long semaphore = call_sys_sem_new(1);
    call_sys_write("semaphore created \n");
    int pid = call_sys_fork();
    if (pid < 0) {
        call_sys_write("Error during fork\n\r");
        call_sys_exit();
        return;
    }
    if (pid == 0) {
        loop("abcde");
    } else {
        loop("12345");
    }
}