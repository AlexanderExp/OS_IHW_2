#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

#define MAX_CLIENTS 50

sem_t *room_sem;
pthread_mutex_t mutex;

void *client(void *arg) {
    int id = *(int *)arg;

    while(1) {
        // попытаться войти в номер
        sem_wait(room_sem);

        // заблокировать мьютекс для изменения состояния номеров
        pthread_mutex_lock(&mutex);

        // заселиться в номер
        printf("Client %d has entered the room.\n", id);
        sleep(1);

        // освободить номер
        sem_post(room_sem);
        printf("Client %d has left the room.\n", id);

        // разблокировать мьютекс
        pthread_mutex_unlock(&mutex);

        // выйти из цикла, если клиент был последним
        if(id == MAX_CLIENTS) {
            break;
        }
    }

    return NULL;
}

int main() {
    int i, client_ids[MAX_CLIENTS];
    pthread_t clients[MAX_CLIENTS];

    // создать семафор для номеров
    room_sem = sem_open("room_sem", O_CREAT, 0666, 30);

    // инициализировать мьютекс
    pthread_mutex_init(&mutex, NULL);

    // создать клиентов
    for(i = 1; i <= MAX_CLIENTS; i++) {
        client_ids[i-1] = i;
        pthread_create(&clients[i-1], NULL, client, &client_ids[i-1]);
    }

    // ждать завершения всех клиентов
    for(i = 0; i < MAX_CLIENTS; i++) {
        pthread_join(clients[i], NULL);
    }

    // закрыть и удалить семафор
    sem_close(room_sem);
    sem_unlink("room_sem");

    // уничтожить мьютекс
    pthread_mutex_destroy(&mutex);

    return 0;
}
