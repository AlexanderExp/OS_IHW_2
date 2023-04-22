#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_ROOMS 30

sem_t room_sem;      // семафор для доступных номеров
int rooms[MAX_ROOMS];// массив номеров комнат

void *client(void *arg) {
    int client_id = *(int *) arg;
    while (1) {
        printf("Клиент %d подошел к портье\n", client_id);

        // пытаемся занять номер
        sem_wait(&room_sem);
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
            sleep(rand() % 10 + 1);

            // выселяемся
            rooms[room_id - 1] = 0;
            sem_post(&room_sem);
            printf("Клиент %d выселился из номера %d\n", client_id, room_id);
            break;
        } else {
            printf("Клиент %d не смог заселиться в отель, ждем...\n", client_id);
            sleep(rand() % 5 + 1);
        }
    }

    return NULL;
}

int main() {

    sem_init(&room_sem, 0, MAX_ROOMS);

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

    sem_destroy(&room_sem);

    return 0;
}
