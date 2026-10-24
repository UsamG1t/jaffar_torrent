#include <dirent.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

enum
{
    Block_Size = 4096,
    NUM_OF_DISKS_RAID0 = 2,
    NUM_OF_DISKS_RAID1 = 4,
    NUM_OF_DISKS_RAID3 = 5,
    NUM_OF_DISKS_RAID4 = 5,
    NUM_OF_DISKS_RAID5 = 5
};

// N дисков, рассмотрим 4: (0)       (1)       (2)       (3) 
//                         (4)       (5)       (6)       (7)
//                         (8)       (9)       (10)      (11)
//                         (12)      (13)      (14)      (15)
// RAID 0 (Ничего из себя не представляет, просто построчная запись)
// Реализовать функции чтения и записи с блоками размера 4096 байт в устройство RAID 0 в 2 диска. Ф-ция принимает 3 параметра
// unsigned int N -  номер полосы данных в устройстве
// char *buf - указатель на содержимое блока 
// int disk[2] - массив файловых дескрипторов

void
write_RAID0(unsigned int N, char *buf, int Disk[NUM_OF_DISKS_RAID0])
{
    int num_of_disk = N % NUM_OF_DISKS_RAID0; //Номер текущего диска
    int num_in_disk = Block_Size * (N / NUM_OF_DISKS_RAID0); // Смещение в диске
    int byte_shift = num_in_disk * Block_Size; // байтовый сдвиг по диску

    lseek(Disk[num_of_disk], byte_shift, SEEK_SET);
    write(Disk[num_of_disk], buf, Block_Size);
}

void
read_RAID0(unsigned int N, char *buf, int Disk[NUM_OF_DISKS_RAID0])
{
    int num_of_disk = N % NUM_OF_DISKS_RAID0; //Номер текущего диска
    int num_in_disk = Block_Size * (N / NUM_OF_DISKS_RAID0); // Смещение в диске
    int byte_shift = num_in_disk * Block_Size; // байтовый сдвиг по диску

    lseek(Disk[num_of_disk], byte_shift, SEEK_SET);
    read(Disk[num_of_disk], buf, Block_Size);
}

///////////////////////////////////////////////////////////////


// RAID 1 (Дублирует диски)
//  (0)       (0)       (1)       (1) 
//  (2)       (2)       (3)       (3)
//  (4)       (4)       (5)       (5)
//  (6)       (6)       (7)       (7)
// Реализовать функции чтения и записи с блоками размера 4096 байт в устройство RAID 1 в 4 диска. Ф-ция принимает 3 параметра
// unsigned int N -  номер полосы данных в устройстве
// char *buf - указатель на содержимое блока 
// int disk[2] - массив файловых дескрипторов

void
write_RAID1(unsigned int N, char *buf, int Disk[NUM_OF_DISKS_RAID1])
{
    int num_of_disk = ( N % (NUM_OF_DISKS_RAID1 / 2) ) * 2; //Номер текущего диска
    int num_in_disk = Block_Size * (N / (NUM_OF_DISKS_RAID1 / 2)); // Смещение в диске
    int byte_shift = num_in_disk * Block_Size; // байтовый сдвиг по диску

    lseek(Disk[num_of_disk], byte_shift, SEEK_SET);
    write(Disk[num_of_disk], buf, Block_Size);
    lseek(Disk[num_of_disk + 1], byte_shift, SEEK_SET);
    write(Disk[num_of_disk + 1], buf, Block_Size);
}

// Преимущество всех последующих RAIDов - возможность восстановить данные при поломке основного(-ых) диска(-ов). 
// Во всех функциях всех последующих RAIDов считаем, что корректные данные в резервных дисках и что эти диски не ломаются.

void
read_RAID1(unsigned int N, char *buf, int Disk[NUM_OF_DISKS_RAID1])
{
    int num_of_disk = ( N % (NUM_OF_DISKS_RAID1 / 2) ) * 2; //Номер текущего диска
    int num_in_disk = Block_Size * (N / (NUM_OF_DISKS_RAID1 / 2)); // Смещение в диске
    int byte_shift = num_in_disk * Block_Size; // байтовый сдвиг по диску
    char buf_1[Block_Size];

    lseek(Disk[num_of_disk], byte_shift, SEEK_SET);
    read(Disk[num_of_disk], buf, Block_Size);
    lseek(Disk[num_of_disk + 1], byte_shift, SEEK_SET);
    read(Disk[num_of_disk + 1], buf_1, Block_Size);
    if (memcmp(buf, &buf_1, Block_Size)) { // проверяем корректность данных
        lseek(Disk[num_of_disk], byte_shift, SEEK_SET);
        write(Disk[num_of_disk], buf_1, Block_Size); // Если некорректные, то перезаписываем данные из резервного диска
        memcpy(buf, &buf_1, Block_Size);
    }
}

/////////////////////////////////////////////////////////////////////////


// RAID 2 (Код Хэмминга)
// Вместе с N дисками данных в системе есть log_2(N) дисков проверочных бит.
// По полосам дисков считаются коды Хэмминга (вместе с теми самыми проверочными битами), по которым можно обнаружить две ошибки и исправить одну ошибку


//////////////////////////////////////////////////////////////////////////

/*
В конспекте и здесь написаны алгоритмы 3-4-5 рэйдов для записи данных не через обновление проверочного блока, а через пересборку данных. Машечкин не очень ратует за такую схему, поэтому кроме указанных здесь будут и схемы с обновлением
*/

// RAID 3 (N основных + 1 дополнительный {А1}   {А2}  ...  {АN}    {А1 XOR A2 XOR ... XOR AN}) запись 1 байта
// Реализовать функции чтения и записи с блоками размера 4096 байт в устройство RAID 1 в 4 диска. Ф-ция принимает 3 параметра
// unsigned int N -  номер полосы данных в устройстве
// char *buf - указатель на содержимое блока 
// int disk[2] - массив файловых дескрипторов

void
write_RAID3(unsigned int N, char *buf, int Disk[NUM_OF_DISKS_RAID3])
{
    int num_of_disk = N % (NUM_OF_DISKS_RAID3 - 1); //Номер текущего диска
    int num_in_disk = Block_Size * (N / (NUM_OF_DISKS_RAID3 - 1)); // Смещение в диске
    int byte_shift = num_in_disk * Block_Size; // байтовый сдвиг по диску
    
    for (int j = 0; j < NUM_OF_DISKS_RAID3; j++) { // Делаем во всех дисках одинаковые сдвиги
        lseek(Disk[j], byte_shift, SEEK_SET);
    }

    for (int i = 0; i < Block_Size; i++) {
        char check_byte = 0, byte = 0;

        for (int j = 0; j < NUM_OF_DISKS_RAID3 - 1; j++) {
            if (j == num_in_disk) { // Если это наш диск, в него пишем 
                write(Disk[num_of_disk], &buf[i], 1);
                check_byte ^= buf[i]; // Ксорим информацию для проверочного диска
                continue;
            }
            read(Disk[j], &byte, 1); // Из не наших дисков просто ксорим данные
            check_byte ^= byte;
        }

        write(Disk[NUM_OF_DISKS_RAID3-1], &check_byte, 1); // Записываем сксоренный байт
    }
}

void
write_RAID3_with_upd(unsigned int N, char *buf, int Disk[NUM_OF_DISKS_RAID3])
{
    int num_of_disk = N % (NUM_OF_DISKS_RAID3 - 1); //Номер текущего диска
    int num_in_disk = Block_Size * (N / (NUM_OF_DISKS_RAID3 - 1)); // Смещение в диске
    int byte_shift = num_in_disk * Block_Size; // байтовый сдвиг по диску

    lseek(Disk[num_of_disk], byte_shift, SEEK_SET);
    lseek(Disk[NUM_OF_DISKS_RAID3 - 1], byte_shift, SEEK_SET);

    for (int i = 0; i < Block_Size; i++) {
        char check_byte = 0, byte = 0;
        read(Disk[NUM_OF_DISKS_RAID3 - 1], &check_byte, 1); // берём старый проверочный байт
        read(Disk[num_of_disk], &byte, 1); // берём старый байт

        lseek(Disk[NUM_OF_DISKS_RAID3 - 1], -1, SEEK_CUR);                                        lseek(Disk[num_of_disk], -1, SEEK_CUR);

        check_byte ^= byte; // "удаляем" старое значение
        check_byte ^= buf[i]; // "добавляем" новое значение

        write(Disk[num_of_disk], &buf[i], 1); // пишем байт в диск
        write(Disk[NUM_OF_DISKS_RAID3-1], &check_byte, 1); // Записываем сксоренный байт
    }
}

void
read_RAID3(unsigned int N, char *buf, int Disk[NUM_OF_DISKS_RAID3])
{
    int num_of_disk = N % (NUM_OF_DISKS_RAID3 - 1); //Номер текущего диска
    int num_in_disk = Block_Size * (N / (NUM_OF_DISKS_RAID3 - 1)); // Смещение в диске
    int byte_shift = num_in_disk * Block_Size; // байтовый сдвиг по диску

    for (int j = 0; j < NUM_OF_DISKS_RAID3; j++) { // Делаем во всех дисках одинаковые сдвиги
        lseek(Disk[j], byte_shift, SEEK_SET);
    }

    for (int i = 0; i < Block_Size; i++) {
        char check_byte = 0, byte = 0;

        for (int j = 0; j < NUM_OF_DISKS_RAID3; j++) {
            if (j == num_in_disk) { // Если это наш диск, из него читаем 
                read(Disk[num_of_disk], &buf[i], 1);
                continue;
            }
            read(Disk[j], &byte, 1); // Из не наших дисков (и из проверочного тоже) просто ксорим данные
            check_byte ^= byte;
        }

        if (buf[i] != check_byte) { // Если данные испорчены
            buf[i] = check_byte; //Исправляем у себя
            lseek(Disk[num_of_disk], -1, SEEK_CUR);
            write(Disk[num_of_disk], &buf[i], 1); // И в диске
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////

// RAID 4 (N основных + 1 дополнительный {А1}   {А2}  ...  {АN}    {А1 XOR A2 XOR ... XOR AN}) запись по полосам
// Реализовать функции чтения и записи с блоками размера 4096 байт в устройство RAID 1 в 4 диска. Ф-ция принимает 3 параметра
// unsigned int N -  номер полосы данных в устройстве
// char *buf - указатель на содержимое блока 
// int disk[2] - массив файловых дескрипторов

void
write_RAID4(unsigned int N, char *buf, int Disk[NUM_OF_DISKS_RAID4])
{
    int num_of_disk = N % (NUM_OF_DISKS_RAID4 - 1); //Номер текущего диска
    int num_in_disk = Block_Size * (N / (NUM_OF_DISKS_RAID4 - 1)); // Смещение в диске
    int byte_shift = num_in_disk * Block_Size; // байтовый сдвиг по диску
    char check_buffers[NUM_OF_DISKS_RAID4][Block_Size] = {};

    for (int j = 0; j < NUM_OF_DISKS_RAID4 - 1; j++) { // Делаем во всех информационных дисках одинаковые сдвиги и читаем из них
        lseek(Disk[j], byte_shift, SEEK_SET);
        if (j == num_of_disk) {
            write(Disk[j], buf, Block_Size); // В наш диск предварительно пишем новые данные
            lseek(Disk[j], -Block_Size, SEEK_CUR); // Мы же при записи сдвинули указатель, возвращаем в начало строки его
        }
        read(Disk[j], check_buffers[j], Block_Size); // Читаем из информационных дисков
    }

    for (int i = 0; i < Block_Size; i++) {
        for (int j = 0; j < NUM_OF_DISKS_RAID4 - 1; j++) {
            check_buffers[NUM_OF_DISKS_RAID4 - 1][i] ^= check_buffers[j][i]; // В проверочный диск собираем ксор данных (да, побайтово, а по-другому никак, ксор не умеет в строки)
        }
    }
    lseek(Disk[NUM_OF_DISKS_RAID4 - 1], byte_shift, SEEK_SET);
    write(Disk[NUM_OF_DISKS_RAID4 - 1], check_buffers[NUM_OF_DISKS_RAID4 - 1], Block_Size); // Записываем итог в проверочный диск
}

void
write_RAID4_with_upd(unsigned int N, char *buf, int Disk[NUM_OF_DISKS_RAID4])
{
    int num_of_disk = N % (NUM_OF_DISKS_RAID4 - 1); //Номер текущего диска
    int num_in_disk = Block_Size * (N / (NUM_OF_DISKS_RAID4 - 1)); // Смещение в диске
    int byte_shift = num_in_disk * Block_Size; // байтовый сдвиг по диску
    char check_buffer[2][Block_Size] = {};

    lseek(Disk[num_of_disk], byte_shift, SEEK_SET);
    lseek(Disk[NUM_OF_DISKS_RAID4 - 1], byte_shift, SEEK_SET);

    read(Disk[NUM_OF_DISKS_RAID4 - 1], check_buffer[0], Block_Size); // Читаем проверочную информацию
    read(Disk[num_of_disk], check_buffer[1], Block_Size); // Читаем старую запись

    // Мы же при записи сдвинули указатель, возвращаем в начало строки его
    lseek(Disk[NUM_OF_DISKS_RAID4 - 1], -Block_Size, SEEK_CUR); 
    lseek(Disk[num_of_disk], -Block_Size, SEEK_CUR);

    for (int i = 0; i < Block_Size; i++) {
        check_buffer[0][i] ^= check_buffer[1][i]; // В проверочном диске удаляем старую запись (да, побайтово, а по-другому никак, ксор не умеет в строки)
    }
    for (int i = 0; i < Block_Size; i++) {
        check_buffer[0][i] ^= buf[i]; // Добавляем новую запись
    }

    write(Disk[num_of_disk], buf, Block_Size); // Записываем новую информацию
    write(Disk[NUM_OF_DISKS_RAID4 - 1], check_buffer[0], Block_Size); // Записываем итог в проверочный диск    
}

void
read_RAID4(unsigned int N, char *buf, int Disk[NUM_OF_DISKS_RAID4])
{
    int num_of_disk = N % (NUM_OF_DISKS_RAID4 - 1); //Номер текущего диска
    int num_in_disk = Block_Size * (N / (NUM_OF_DISKS_RAID4 - 1)); // Смещение в диске
    int byte_shift = num_in_disk * Block_Size; // байтовый сдвиг по диску
    char check_buffers[NUM_OF_DISKS_RAID4][Block_Size] = {};

    for (int j = 0; j < NUM_OF_DISKS_RAID4; j++) { // Делаем во всех дисках одинаковые сдвиги и читаем из них
        lseek(Disk[j], byte_shift, SEEK_SET);
        if (j == num_of_disk) {
            read(Disk[j], buf, Block_Size); // В наш диск читаем сразу в буфер
            continue;
        }
        read(Disk[j], check_buffers[j], Block_Size); // Для остальных дисков читаем в чек-массив
    }

    // собираем проверчную информацию
    for (int i = 0; i < Block_Size; i++) {
        for (int j = 0; j < NUM_OF_DISKS_RAID4; j++) {
            if (j == num_of_disk) {
                continue; // наш диск игнорим
            }
            check_buffers[num_of_disk][i] ^= check_buffers[j][i]; // На место нашего диска в чек-памяти собираем точно корректную версию (по ксорам всех остальных)
        }
    }

    if (memcmp(buf, check_buffers[num_of_disk], Block_Size)) { // Если данные испорчены
        memcpy(buf, check_buffers[num_of_disk], Block_Size); // Исправляем у себя
        lseek(Disk[num_of_disk], byte_shift, SEEK_SET);
        write(Disk[num_of_disk], check_buffers[num_of_disk], Block_Size); // И в диске
    }
}

///////////////////////////////////////////////////////////////////////////////


// RAID 5 (N дисков {А1}   {А2}  ...  {АN}    {А1 XOR A2 XOR ... XOR AN} идут лесенкой...) запись 4096 байтов

//  (    0    )(    1    )(    2    )(    3    )( 0-3 xor ) 
//  (    4    )(    5    )(    6    )( 4-7 xor )(    7    ) 
//  (    8    )(    9    )( 8-11 xor)(   10    )(   11    ) 
//  (   12    )(12-15 xor)(   13    )(   14    )(   15    ) 


// Реализовать функции чтения и записи с блоками размера 4096 байт в устройство RAID 1 в 4 диска. Ф-ция принимает 3 параметра
// unsigned int N -  номер полосы данных в устройстве
// char *buf - указатель на содержимое блока 
// int disk[2] - массив файловых дескрипторов

#define disk_num(num, shift) (((num) < (shift)) ? (num) : (num) + 1) 
// Полосы левее проверочной остались в своём диске, полосы правее - уехали в следующий

void
write_RAID5(unsigned int N, char *buf, int Disk[NUM_OF_DISKS_RAID5])
{
    int num_of_disk = N % (NUM_OF_DISKS_RAID5 - 1); //Номер текущего диска
    int num_in_disk = Block_Size * (N / (NUM_OF_DISKS_RAID5 - 1)); // Смещение в диске
    int byte_shift = num_in_disk * Block_Size; // байтовый сдвиг по диску
    int shift = (NUM_OF_DISKS_RAID5 - 1) - ((N / (NUM_OF_DISKS_RAID5 - 1)) % NUM_OF_DISKS_RAID5); // То, в какой диск переехала проверочная полоса 
    char check_buffers[NUM_OF_DISKS_RAID5][Block_Size] = {};

    for (int j = 0; j < NUM_OF_DISKS_RAID5 - 1; j++) { // Делаем во всех информационных дисках одинаковые сдвиги и читаем из них
        lseek(Disk[disk_num(j, shift)], byte_shift, SEEK_SET);
        if (j == num_of_disk) {
            write(Disk[disk_num(j, shift)], buf, Block_Size); // В наш диск предварительно пишем новые данные
            lseek(Disk[disk_num(j, shift)], -Block_Size, SEEK_CUR); // Мы же при записи сдвинули указатель, возвращаем в начало строки его
        }
        read(Disk[disk_num(j, shift)], check_buffers[j], Block_Size); // Читаем из информационных дисков (При этом в чек-массиве (второй параметр) у нас записывается без странных сдвигов; информационные  - первые массивы, проверочный - последний)
    }

    for (int i = 0; i < Block_Size; i++) {
        for (int j = 0; j < NUM_OF_DISKS_RAID5 - 1; j++) {
            check_buffers[NUM_OF_DISKS_RAID5 - 1][i] ^= check_buffers[j][i]; // В проверочный диск собираем ксор данных (да, побайтово, а по-другому никак, ксор не умеет в строки)
        }
    }
    lseek(Disk[shift], byte_shift, SEEK_SET);
    write(Disk[shift], check_buffers[NUM_OF_DISKS_RAID5 - 1], Block_Size); // Записываем итог в проверочный диск
}

void
write_RAID5_with_upd(unsigned int N, char *buf, int Disk[NUM_OF_DISKS_RAID5])
{
    int num_of_disk = N % (NUM_OF_DISKS_RAID5 - 1); //Номер текущего диска
    int num_in_disk = Block_Size * (N / (NUM_OF_DISKS_RAID5 - 1)); // Смещение в диске
    int byte_shift = num_in_disk * Block_Size; // байтовый сдвиг по диску
    int shift = (NUM_OF_DISKS_RAID5 - 1) - ((N / (NUM_OF_DISKS_RAID5 - 1)) % NUM_OF_DISKS_RAID5); // То, в какой диск переехала проверочная полоса 
    char check_buffer[2][Block_Size] = {};

    lseek(Disk[disk_num(num_of_disk, shift)], byte_shift, SEEK_SET);
    lseek(Disk[shift], byte_shift, SEEK_SET);

    read(Disk[shift], check_buffer[0], Block_Size); // Читаем проверочную информацию
    read(Disk[disk_num(num_of_disk, shift)], check_buffer[1], Block_Size); // Читаем старую запись

    // Мы же при записи сдвинули указатель, возвращаем в начало строки его
    lseek(Disk[shift], -Block_Size, SEEK_CUR); 
    lseek(Disk[disk_num(num_of_disk, shift)], -Block_Size, SEEK_CUR);

    for (int i = 0; i < Block_Size; i++) {
        check_buffer[0][i] ^= check_buffer[1][i]; // В проверочном диске удаляем старую запись (да, побайтово, а по-другому никак, ксор не умеет в строки)
    }
    for (int i = 0; i < Block_Size; i++) {
        check_buffer[0][i] ^= buf[i]; // Добавляем новую запись
    }

    write(Disk[disk_num(num_of_disk, shift)], buf, Block_Size); // Записываем новую информацию
    write(Disk[shift], check_buffer[0], Block_Size); // Записываем итог в проверочный диск
}

void
read_RAID5(unsigned int N, char *buf, int Disk[NUM_OF_DISKS_RAID5])
{
    int num_of_disk = N % (NUM_OF_DISKS_RAID4 - 1); //Номер текущего диска
    int num_in_disk = Block_Size * (N / (NUM_OF_DISKS_RAID4 - 1)); // Смещение в диске
    int byte_shift = num_in_disk * Block_Size; // байтовый сдвиг по диску
    int shift = (NUM_OF_DISKS_RAID5 - 1) - ((N / (NUM_OF_DISKS_RAID5 - 1)) % NUM_OF_DISKS_RAID5); // То, в какой диск переехала проверочная полоса 
    char check_buffers[NUM_OF_DISKS_RAID4][Block_Size] = {};

    for (int j = 0; j < NUM_OF_DISKS_RAID5 - 1; j++) { // Делаем во всех дисках одинаковые сдвиги и читаем из них
        lseek(Disk[disk_num(j, shift)], byte_shift, SEEK_SET);
        if (j == num_of_disk) {
            read(Disk[disk_num(j, shift)], buf, Block_Size); // В наш диск читаем сразу в буфер
            continue;
        }
        read(Disk[disk_num(j, shift)], check_buffers[j], Block_Size); // Для остальных дисков читаем в чек-массив
    }

    //проверочный диск обрабатываем отдельно, он формуле в макросе не подчиняется
    lseek(Disk[shift], byte_shift, SEEK_SET);
    read(Disk[shift], check_buffers[NUM_OF_DISKS_RAID5 - 1], Block_Size);


    // собираем проверчную информацию
    for (int i = 0; i < Block_Size; i++) {
        for (int j = 0; j < NUM_OF_DISKS_RAID5; j++) {
            if (j == num_of_disk) {
                continue; // наш диск игнорим
            }
            check_buffers[num_of_disk][i] ^= check_buffers[j][i]; // На место нашего диска в чек-памяти собираем точно корректную версию (по ксорам всех остальных)
        }
    }

    if (memcmp(buf, check_buffers[num_of_disk], Block_Size)) { // Если данные испорчены
        memcpy(buf, check_buffers[num_of_disk], Block_Size); // Исправляем у себя
        lseek(Disk[disk_num(num_of_disk, shift)], byte_shift, SEEK_SET);
        write(Disk[disk_num(num_of_disk, shift)], check_buffers[num_of_disk], Block_Size); // И в диске
    }
}