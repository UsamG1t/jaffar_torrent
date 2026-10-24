#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <sys/socket.h>
#include <sys/un.h>

enum
{
    BUFLEN = 100,
    SRV_BUF = 5
};

int
main(int argc, char **argv)
{
    struct sockaddr_un friend_addr, my_addr;
    int sockfd, client_sockfd;
    char buf[BUFLEN];
    int is_server;

    int turtle_x = 0, turtle_y = 0;
    int turtle_dx = 0, turtle_dy = 0;
    char turtle_talk[BUFLEN];
    
    int status = 0;
    pid_t pid;
    
    if (argc < 2 || (argc == 2 && !strcmp(argv[1], "client"))) {
        printf("Usage: %s server | [client <your_name>] \n\t\"_exit\" to poweroff server\n\t\"quit\" to just exit\n", argv[0]);
        return 0;
    }

    is_server = !strcmp(argv[1], "server");
    
    if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        printf("can't create socket\n");
        return 0;
    }

    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sun_family = AF_UNIX;
    strncpy(my_addr.sun_path, is_server ? "server" : argv[2], is_server ? 7 : strlen(argv[2]) + 1);

    /* связываем сокет */
    unlink(my_addr.sun_path); 
    if (bind(sockfd, (struct sockaddr*) &my_addr, sizeof(my_addr.sun_family) + \
            strlen(my_addr.sun_path) +1) < 0) { 

        printf("can't bind socket!\n");
        return 0;
    }

    if (!is_server) {
        memset(&friend_addr, 0, sizeof(friend_addr));
        friend_addr.sun_family = AF_UNIX;
        strncpy(friend_addr.sun_path, "server", 7);

        if (connect(sockfd, (struct sockaddr*) &friend_addr, sizeof(friend_addr.sun_family) + \
                strlen(friend_addr.sun_path) +1) < 0) {

            printf("can't connect to socket!\n");
            return 0;
        }

    
        while (printf(">>: "), fgets(buf, BUFLEN, stdin) != NULL) {

            if (!strncmp(buf, "_exit", 5) || !strncmp(buf, "quit", 4)) {
                send(sockfd, buf, strlen(buf) + 1, 0);
                break;
            }
            
            if (send(sockfd, buf, strlen(buf) + 1, 0) != strlen(buf) + 1) {
                printf("client: error writing socket!\n");
                break;
            }

            if (recv(sockfd, buf, BUFLEN, 0) < 0) {
                printf("client: error reading socket!\n");
                break;
            }

            printf("<< %s\n", buf);
        } // while

        shutdown(sockfd, 2);
        close(sockfd);
        unlink(my_addr.sun_path); 
        return 0;

    }


    if (listen(sockfd, SRV_BUF) < 0) {
        printf("server: error listening socket!");
        unlink(my_addr.sun_path); 
        return 0;
    }

    while(1) {
        memset(&friend_addr, 0, sizeof(friend_addr));
        socklen_t friend_len = sizeof(friend_addr);
        if ((client_sockfd = accept(sockfd, (struct sockaddr*) &friend_addr, &friend_len)) < 0) {
            printf("server: error accepting socket!");
            break;
        }

        if ((pid = fork()) < 0) {
            printf("server: error during fork!");
            break;
        } else if (!pid) {
            close(sockfd);

            while (1) {
                if (recv(client_sockfd, buf, BUFLEN, 0) < 0) { // ошибка при приеме данных
                    printf("server_fork: error reading socket!");
                    break;
                }

                fprintf(stderr, "server_fork: received from client: %s\n", buf);

                if (!strncmp(buf, "_exit", 5)) {
                    status = 1;
                    fprintf(stderr, "server_fork: stoping server\n");
                    break;
                } else if (!strncmp(buf, "quit", 4)) {
                    fprintf(stderr, "server_fork: quit cmd\n");
                    break;
                } else if (!strncmp(buf, "run", 3)) {
                    fprintf(stderr, "server_fork: run cmd\n");
                    if (sscanf(&buf[3], "%d%d", &turtle_dx, &turtle_dy) != 2) {
                        strcpy(buf, "not enough args: need two\n");
                    } else {
                        turtle_x += turtle_dx;                        
                        turtle_y += turtle_dy;
                        snprintf(buf, BUFLEN, "Turtle travel to <%d:%d>\n", turtle_x, turtle_y);                        
                    }
                } else if (!strncmp(buf, "say", 3)) {
                    fprintf(stderr, "server_fork: say cmd\n");
                    if (sscanf(&buf[3], "%s", turtle_talk) != 1) {
                        strcpy(buf, "Turtle says nothing\n");
                    } else {
                        snprintf(buf, BUFLEN, "Turtle said \"%s\"\n",turtle_talk);                        
                    }
                } else {
                    strcpy(buf, "Turtle does not know this command\n");
                }
                
                if (send(client_sockfd, buf, strlen(buf) + 1, 0) != strlen(buf)+1) {
                    printf("server_fork: error writing socket!\n");
                    break;
                }
            }

            shutdown(client_sockfd, 2);
            close(client_sockfd);
            return status;
        }

        waitpid(-1, &status, WNOHANG);
        if (WIFEXITED(status) && WEXITSTATUS(status)) {
            fprintf(stderr, "server: stoping\n");
            break;
        }
    } // while

    while (wait(NULL) != -1);

    shutdown(sockfd, 2);
    close(sockfd);
    unlink(my_addr.sun_path);
    return 0;
}
