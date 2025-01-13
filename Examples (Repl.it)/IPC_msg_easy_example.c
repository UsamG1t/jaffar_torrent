#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct Msgbuf {
    long type;
    char text[40];
} Msgbuf;

int main(void) {
    setbuf(stdout, 0);

    int msgid = msgget(IPC_PRIVATE, IPC_CREAT | 0600);
    pid_t pid;

    if ((pid = fork()) < 0) {
        _exit(1);
    } else if (!pid) {
        Msgbuf msg1, msg2, msg3, msg4;

        msg1.type = 2;
        strcpy(msg1.text, "Result is:\n");
        msg2.type = 4;
        strcpy(msg2.text, "False\n");
        msg3.type = 3;
        strcpy(msg3.text, "Hello\n");
        msg4.type = 1;
        strcpy(msg4.text, "True\n");
        
        msgsnd(msgid, &msg1, sizeof(msg1) - sizeof(msg1.type), 0);
        msgsnd(msgid, &msg2, sizeof(msg2) - sizeof(msg2.type), 0);
        msgsnd(msgid, &msg3, sizeof(msg3) - sizeof(msg3.type), 0);
        msgsnd(msgid, &msg4, sizeof(msg4) - sizeof(msg4.type), 0);
        _exit(0);
    }

    sleep(3);

    Msgbuf res;
    msgrcv(msgid, &res, sizeof(res) - sizeof(res.type), 3, 0);
    printf("%s", res.text);
    msgrcv(msgid, &res, sizeof(res) - sizeof(res.type), 0, 0);
    printf("%s", res.text);
    msgrcv(msgid, &res, sizeof(res) - sizeof(res.type), -4, 0);
    printf("%s", res.text);

    wait(NULL);
    msgctl(msgid, IPC_RMID, 0);

    return 0;
}

/*
[papillon_rouge@NONAME-STREET Examples (Repl.it)]$ gcc IPC_msg_easy_example.c -o IPC_msg_easy_example
[papillon_rouge@NONAME-STREET Examples (Repl.it)]$ time ./IPC_msg_easy_example
Hello
Result is:
True

real    0m3,008s
user    0m0,006s
sys     0m0,002s
[papillon_rouge@NONAME-STREET Examples (Repl.it)]$ 
*/