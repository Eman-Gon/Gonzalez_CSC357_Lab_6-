#define _GNU_SOURCE

#include "net.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PORT 32454  // Choose a different port number between 1024 and 65535

void handle_request(int nfd)
{
    FILE *network = fdopen(nfd, "r");
    char *line = NULL;
    size_t size;
    ssize_t num;

    if (network == NULL)
    {
        perror("fdopen");
        close(nfd);
        return;
    }

    while ((num = getline(&line, &size, network)) >= 0)
    {
        // Send the received data back to the client
        write(nfd, line, num);
    }

    free(line);
    fclose(network);
}

void run_service(int fd)
{
    while (1)
    {
        int nfd = accept_connection(fd);
        if (nfd != -1)
        {
            printf("Connection established\n");
            handle_request(nfd);
            printf("Connection closed\n");
        }
    }
}

int main(void)
{
    int fd = create_service(PORT);
    if (fd == -1)
    {
        perror(0);
        exit(1);
    }

    printf("Listening on port: %d\n", PORT);
    run_service(fd);
    close(fd);

    return 0;
}