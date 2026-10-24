#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define BUFLEN 40

int
main(int argc, char **argv)
{
    struct sockaddr_un friend_addr, my_addr;
    int sockfd;
    int is_server;
    char buf[BUFLEN];
    int friend_len;
    int quit_flag = 0;

    if (argc != 2) {
        printf("Usage: %s client|server.\n", argv[0]);
        return 0;
    }

    is_server = !strcmp(argv[1], "server"); /*  определяем, кто мы: клиент или сервер */ 
    
    /* создаем сокет, в sockfd помещается
    дескриптор создаваемого сокета*/
    if ((sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0) {
        printf("can't create socket\n");
        return 0;
    }

    /* создаём структуру адреса для связывания сокета*/
    memset(&my_addr, 0, sizeof(my_addr)); /* предварительно очищаем структуру */
    my_addr.sun_family = AF_UNIX; /* локальный домен */
    strcpy(my_addr.sun_path, is_server ? "server" : "client");


    /* связываем сокет */
    unlink(my_addr.sun_path); 
    if (bind(sockfd, (struct sockaddr *) &my_addr, 
        sizeof(my_addr.sun_family) + strlen(my_addr.sun_path) +1) < 0) { 
    /* к размеру структуры прибавляем реальную длину строки с
    адресом, включая завершающий ‘\0’ */
        printf("can't bind socket!");
        return 0;
    }
    /* прежде всего удаляем файл, иначе, если такой файл уже
    существует, то считается, что такое имя уже
    занято и связывание завершается неуспешно */


    if (!is_server) { /* это – клиент */

        /* аналогично заполняем адресную структуру
        с именем файла сервера */
        memset(&friend_addr, 0, sizeof(friend_addr));
        friend_addr.sun_family = AF_UNIX;
        strcpy(friend_addr.sun_path, "server");
        
        printf("type the string: ");
        /* читаем строки с командами от
        пользователя */
        
        while (scanf("%s", buf) == 1) {

            /* не пора ли выходить? */
            quit_flag = (!strcmp(buf, "quit"));
            
            /* считали строку и передаем ее
            серверу, размер строки указываем с
            учетом завершающего ‘\0’ */
            if (sendto(sockfd, buf, strlen(buf) + 1, 0,
                    (struct sockaddr *)&friend_addr, 
                    sizeof(friend_addr.sun_family) + strlen("server")) != strlen(buf) + 1) {
                /* Строка не отправилась */
                printf("client: error writing socket!\n");
                unlink(my_addr.sun_path); /*удаляем файл при выходе */
                return 0;
            }

            /*получаем ответ и выводим его на
            печать; NULL указвает на то, что нам
            не нужна адресная структура
            отправителя сообщения */
            if (recvfrom(sockfd, buf, BUFLEN, 0, NULL, 0) < 0) { // ошибка при приеме данных
                printf("client: error reading socket!\n");
                unlink(my_addr.sun_path); /*удаляем файл при выходе */
                return 0;
            }

            printf("client: server answered: %s\n", buf);

            if (quit_flag) break;

            printf("type the string: ");
        } // while
        
        close(sockfd);
        unlink(my_addr.sun_path); /*удаляем файл при выходе */
        return 0;
    } // if (!is_server)
    
    /* это – сервер */
    while (1) {
    
        /* получаем строку от клиента и выводим на
        печать; в последних двух параметрах
        получаем адресную структуру отправителя и
        ее размер */
        friend_len = sizeof(friend_addr);
        if (recvfrom(sockfd, buf, BUFLEN, 0, 
                (struct sockaddr *) &friend_addr, &friend_len) < 0) { // ошибка при приеме данных
            printf("server: error reading socket!");
            unlink(my_addr.sun_path); /*удаляем файл при выходе */
            return 0;
        }

        printf("server: received from client: %s\n", buf);

        /* не пора ли выходить? */
        quit_flag = (!strcmp(buf, "quit"));
        
        if (quit_flag) {
            strcpy(buf, "quit_flag now!");
        } else {  /* в зависимости от запроса готовим ответ */
            if (!strcmp(buf, "ping")) {
                strcpy(buf, "pong!");
            } else {
                strcpy(buf, "wrong string!");
            }
        }
        
        /* посылаем ответ */
        if (sendto(sockfd, buf, strlen(buf) + 1, 0, 
                (struct sockaddr *) &friend_addr, friend_len) != strlen(buf)+1) { // ошибка при передаче данных
            printf("server: error writing socket!\n");
            unlink(my_addr.sun_path); /*удаляем
            файл при выходе */
            return 0;
        }
        if (quit_flag) break;
    } // while
    
    close(sockfd);
    unlink(my_addr.sun_path); /*удаляем файл, который был создан в файловой системе при связывании сокета*/
    return 0;
}
