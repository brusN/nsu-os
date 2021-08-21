#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define FORK_ERR -1
#define EXEC_ERR -1
#define WAIT_ERR -1
/*
 * Напишите программу, которая создает подпроцесс. Этот подпроцесс должен исполнить cat(1) длинного файла.
 * Родитель должен вызвать printf(3) и распечатать какой-либо текст.
 * После выполнения первой части задания модифицируйте программу так, чтобы последняя строка, распечатанная родителем,
 * выводилась после завершения порожденного процесса. Используйте wait(2), waitid(2) или waitpid(3).
 */

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Incorrect input arguments!");
        exit(EXIT_FAILURE);
    }

    char commandName[] = "cat";
    char fullCommandPath[] = "/bin/cat";
    char *fileName = argv[1];

    pid_t pid;
    pid = fork();

    if (pid == FORK_ERR) {
        perror("Error while fork()");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        if (execl(fullCommandPath, commandName, fileName, NULL) == EXEC_ERR) {
            perror("Error while execl()");
            exit(EXIT_FAILURE);
        }
    }
    else {
        int curStatus = 0;
        pid_t waitStatus = wait(&curStatus);
        if (waitStatus == WAIT_ERR) {
            perror("Error while wait()");
            exit(EXIT_FAILURE);
        }

        if (WIFSIGNALED(curStatus)) {
            int signalInfo = WTERMSIG(curStatus);
            fprintf(stderr, "Child process terminated with a signal: %d\n", signalInfo);
            if (WCOREDUMP(curStatus)) {
               fprintf(stderr, "Core file created");
            }
        } else if (WIFEXITED(curStatus)) {
            int exitStatus = WEXITSTATUS(curStatus);
            printf("\n[From parent process] Child PID is %d\n", pid);
            printf("[From parent process] Child process exited with waitStatus: %d\n", exitStatus);
        }
    }

    return EXIT_SUCCESS;
}
