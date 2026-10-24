#include <stdio.h>
#include <unistd.h>
#include <signal.h>

volatile int count = 0;

void(*prev)(int);

void
handler(int sig) {
    signal(SIGINT, handler);
    printf("%d\n", ++count);

    if (count == 4) {
        signal(SIGINT, prev);   // - восстанавливаем предыдущую реакцию(вдруг там была не дефолтная)
    }
    
    return;
}

int
main(int argc, char** argv)
{
    prev = signal(SIGINT, handler);
    while(1) pause();
    return 0;
}