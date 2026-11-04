// Простейший TCP-клиент: отправляет строку серверу и печатает ответ.
// Использование: ./client [сообщение]
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 50500

int main(int argc, char *argv[])
{
    const char *msg = (argc > 1) ? argv[1] : "Я люблю ОСи)";
    int sock;
    struct sockaddr_in server;
    char buf[1024];
    ssize_t n;

    // 1. Создаём сокет: домен AF_INET, тип SOCK_STREAM (TCP)
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    // 2. Заполняем адрес сервера (bind клиенту не нужен: порт выберет система)
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) != 1) {
        fprintf(stderr, "Неверный IP-адрес\n");
        exit(1);
    }

    // 3. Устанавливаем соединение с сервером
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect");
        exit(1);
    }

    // 4. Отправляем сообщение
    if (send(sock, msg, strlen(msg), 0) < 0) {
        perror("send");
        exit(1);
    }

    // 5. Ждём ответ
    n = recv(sock, buf, sizeof(buf) - 1, 0);
    if (n < 0) {
        perror("recv");
        exit(1);
    }
    buf[n] = '\0';
    printf("Отправлено: \"%s\"\nОтвет сервера (длина): %s\n", msg, buf);

    // 6. Закрываем сокет
    close(sock);
    return 0;
}
