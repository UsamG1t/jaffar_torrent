#include <cstdio>

void RunTest();

void PrintTasks();

int main() {
    int c;
    std::printf("Список функций:\n1.Список задач\n2.Тесты\n\nВведите цифру\n");
    while (std::scanf("%x", &c) != EOF) {
        if (c == 1) {
            std::printf("Tasks");
        } else {
            if (c == 2) {
                std::printf("Tests");
            } else {
                std::printf("Список функций:\n1.Список задач\n2.Тесты\n\nВведите цифру\n");
            }
        }
    }
    return 1;
}
