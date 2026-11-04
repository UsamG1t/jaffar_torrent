// Простейший TCP-сервер: принимает строку от клиента и отвечает её длиной.
// Клиентов обслуживает по очереди (без fork).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 50500

int main(void)
{
    int listen_fd, client_fd;
    struct sockaddr_in addr, client_addr;
    socklen_t client_len;
    char buf[1024];
    ssize_t n;

    // 1. Создаём сокет: домен AF_INET, тип SOCK_STREAM (TCP)
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("socket");
        exit(1);
    }

    // Разрешаем быстрый перезапуск сервера (иначе bind может вернуть EADDRINUSE)
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 2. Привязываем сокет к порту; INADDR_ANY - принимать на любом IP-адресе хоста
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);              // порт - в сетевом порядке байт
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(1);
    }

    // 3. Переводим сокет в режим прослушивания (очередь до 5 клиентов)
    if (listen(listen_fd, 5) < 0) {
        perror("listen");
        exit(1);
    }
    printf("Сервер слушает порт %d\n", PORT);

    for (;;) {
        // 4. Ждём клиента; accept возвращает НОВЫЙ сокет для общения с ним
        client_len = sizeof(client_addr);
        client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));
        printf("Подключился клиент %s:%d\n", ip, ntohs(client_addr.sin_port));

        // 5. Принимаем сообщение
        n = recv(client_fd, buf, sizeof(buf) - 1, 0);
        if (n < 0) {
            perror("recv");
        } else if (n > 0) {
            buf[n] = '\0';
            printf("Получено: %s\n", buf);

            // 6. Отправляем ответ - длину полученной строки
            char reply[32];
            snprintf(reply, sizeof(reply), "%zd", n);
            send(client_fd, reply, strlen(reply), 0);
        }

        // 7. Закрываем сокет клиента; слушающий сокет остаётся открытым
        close(client_fd);
    }
}
