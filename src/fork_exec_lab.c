#include "fork_exec_lab.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int g_counter = 100;


pid_t spawn_child(void) {
    return fork();
}

int run_child_process(int *heap_counter, int stack_counter) {
    int child_sum;

    /* TODO(student): Update all three counters in the child by +7. */
    g_counter += 7;
    *heap_counter += 7;
    stack_counter += 7;
    /* TODO(student): Compute child_sum as the sum of the updated counters. */
    child_sum = g_counter + *heap_counter + stack_counter;

    /* TODO(student): Print exactly: child: g=<g> h=<h> s=<s> sum=<sum> */
    printf("child: g=%d h=%d s=%d sum=%d\n", g_counter, *heap_counter, stack_counter, child_sum);
    /* TODO(student): Return child_sum % 256. */
    return child_sum % 256;

    (void)heap_counter;
    (void)stack_counter;
    (void)child_sum;
    fprintf(stderr, "TODO: child address-space logic not implemented\n");
    return 1;
}

int wait_for_child(pid_t child_pid, int *heap_counter, int stack_counter) {
    int status = 0;
    int child_code;

    if (waitpid(child_pid, &status, 0) == -1) {
        perror("waitpid");
        return 1;
    }

    if (!WIFEXITED(status)) {
        fprintf(stderr, "parent: child terminated abnormally\n");
        return 1;
    }

    child_code = WEXITSTATUS(status);

    /* TODO(student): Print exactly: parent: child-exit=<code> */
    printf("parent: child-exit=%d\n", child_code);

    /* TODO(student): Print exactly: parent: g=<g> h=<h> s=<s> */
    printf("parent: g=%d h=%d s=%d\n", g_counter, *heap_counter, stack_counter);

    /* TODO(student): If parent values are 100, 200, 300 print:
       parent: address-space=isolated
       Otherwise print:
       parent: address-space=unexpected */
    if(g_counter == 100 && *heap_counter == 200 && stack_counter == 300){
        printf("parent: address-space=isolated\n");
    }else{
        printf("parent: address-space=unexpected\n");
    }

    /* TODO(student): Return 0 only if child_code is 109 and parent values are unchanged. */
    if (child_code == 109 && g_counter == 100 && *heap_counter == 200 && stack_counter == 300) {
        return 0;
    }

    /*
    (void)status;
    (void)child_code;
    (void)heap_counter;
    (void)stack_counter;
    //fprintf(stderr, "TODO: parent reporting not implemented\n");
    */

    return 1;
}

int main(void) {
    int *heap = malloc(sizeof(int));
    if (heap == NULL) {
        perror("malloc");
        return 1;
    }

    *heap = 200;
    int stack = 300;
    printf("parent: start g=%d h=%d s=%d\n", g_counter, *heap, stack);
    fflush(stdout);

    pid_t pid = spawn_child();

    if (pid == -1) {
        perror("fork");
        free(heap);
        return 1;
    }

    if (pid == 0) {
        int child_code = run_child_process(heap, stack);
        fflush(stdout);
        free(heap);
        _exit(child_code);
    }

    int result = wait_for_child(pid, heap, stack);

    free(heap);
    return result;
}