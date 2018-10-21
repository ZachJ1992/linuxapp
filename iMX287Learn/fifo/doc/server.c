#include "client.h"

int main()
{
    int server_fifo_fd;
    int client_fifo_fd;

    int res;
    char client_fifo_name[NAME_SIZE];

    message msg;

    char *p;

    if (mkfifo(SERVER_FIFO_NAME, 0777) == -1)
    {
        fprintf(stderr, "Sorry, create server fifo failure!\r\n");
        exit(EXIT_FAILURE);
    }

    server_fifo_fd = open(SERVER_FIFO_NAME, O_RDONLY);
    if (server_fifo_fd == -1)
    {
        fprintf(stderr, "Sorry, server fifo open failure!\r\n");
        exit(EXIT_FAILURE);
    }

    sleep(5);

   while (res = read(server_fifo_fd, &msg, sizeof(msg)) > 0)
   {
        p = msg.data;
        while (*p)
        {
        *p = toupper(*p);
        ++p;
        }

        sprintf(client_fifo_name, CLIENT_FIFO_NAME, msg.client_pid);
        client_fifo_fd = open(client_fifo_name, O_WRONLY);
        if (client_fifo_fd == -1)
        {
        fprintf(stderr, "Sorry, client fifo open failure!\r\n");
        exit(EXIT_FAILURE);
        }

        write(client_fifo_fd, &msg, sizeof(msg));
        close(client_fifo_fd);
    }

    close(server_fifo_fd);
    unlink(SERVER_FIFO_NAME);
    exit(EXIT_SUCCESS);
}

