#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define MAX_ROOMS 30

void *room_sem;      // указатель на неименованный семафор для доступных номеров
int rooms[MAX_ROOMS];// массив номеров комнат

void *client(void *arg) {
    int client_id = *(int *) arg;
    while (1) {
        printf("Клиент %d подошел к портье\n", client_id);

        // пытаемся занять номер
        sem_wait((sem_t *) room_sem);
        int room_id = -1;
        for (int i = 0; i < MAX_ROOMS; ++i) {
            if (rooms[i] == 0) {
                rooms[i] = client_id;
                room_id = i + 1;
                break;
            }
        }

        if (room_id != -1) {
            printf("Клиент %d заселился в номер %d\n", client_id, room_id);

            // проводим время в отеле
            sleep(rand() % 20 + 1);

            // выселяемся
            rooms[room_id - 1] = 0;
            sem_post((sem_t *) room_sem);
            printf("Клиент %d выселился из номера %d\n", client_id, room_id);
            break;
        } else {
            printf("Клиент %d не смог заселиться в отель, ждем...\n", client_id);
            sleep(rand() % 20 + 1);
        }
    }

    return NULL;
}

int main() {
    size_t room_sem_size = sizeof(sem_t);

    // создаем область разделяемой памяти для неименованного семафора
    int fd = shm_open("/room_sem", O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("Не удалось создать область разделяемой памяти");
        return EXIT_FAILURE;
    }
    ftruncate(fd, room_sem_size);
    room_sem = mmap(NULL, room_sem_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    // инициализируем неименованный семафор
    sem_init((sem_t *) room_sem, 1, MAX_ROOMS);

    // инициализируем номера
    for (int i = 0; i < MAX_ROOMS; ++i) {
        rooms[i] = 0;
    }

    FILE *input = fopen("../input.txt", "r");
    if (!input) {
        perror("Не удалось открыть файл input.txt");
        return EXIT_FAILURE;
    }
    int num_clients;
    fscanf(input, "%d", &num_clients);
    pthread_t clients[num_clients];// массив потоков-клиентов
    // создаем потоки клиентов
    for (int i = 0; i < num_clients; ++i) {
        int *client_id = malloc(sizeof(int));
        *client_id = i + 1;
        pthread_create(&clients[i], NULL, client, client_id);
    }

    // ждем окончания работы потоков
    for (int i = 0; i < num_clients; ++i) {
        pthread_join(clients[i], NULL);
    }

    // удаляем именованный семафор
    sem_close(room_sem);
    sem_unlink("/room_sem");

    return 0;
}
