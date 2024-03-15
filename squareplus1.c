#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
    int pipe1[2], pipe2[2], pipe3[2];
    int value;

    if (pipe(pipe1) < 0 || pipe(pipe2) < 0 || pipe(pipe3) < 0) {
        perror("failed");
        exit(1);
    }

    pid_t child1 = fork();
    if (child1 < 0) {
        perror("failed");
        exit(1);
    } else if (child1 == 0) {
        close(pipe1[1]); close(pipe2[0]); close(pipe3[0]); close(pipe3[1]);

        while (read(pipe1[0], &value, sizeof(value)) > 0) {
            value = value * value;
            write(pipe2[1], &value, sizeof(value));
        }

        close(pipe1[0]); close(pipe2[1]);
        exit(0);
    }

    pid_t child2 = fork();
    if (child2 < 0) {
        perror("failed");
        exit(1);
    } else if (child2 == 0) {
        close(pipe1[0]); close(pipe1[1]); close(pipe2[1]); close(pipe3[0]);

        while (read(pipe2[0], &value, sizeof(value)) > 0) {
            value = value + 1;
            write(pipe3[1], &value, sizeof(value));
        }

        close(pipe2[0]); close(pipe3[1]);
        exit(0);
    }

    close(pipe1[0]); close(pipe2[0]); close(pipe2[1]); close(pipe3[1]);

    while (scanf("%d", &value) != EOF) {
        write(pipe1[1], &value, sizeof(value));
        read(pipe3[0], &value, sizeof(value));
        printf("Result: %d\n", value);
    }

    close(pipe1[1]); close(pipe3[0]);

    int status;
    waitpid(child1, &status, 0);
    waitpid(child2, &status, 0);

    return 0;
}