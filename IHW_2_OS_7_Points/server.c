#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define SEMAPHORE_NAME "/hotel_semaphore"
#define SHARED_MEMORY_NAME "/hotel_shared_memory"
#define NUM_ROOMS 30

int main() {
    // Создаем именованный семафор и устанавливаем начальное значение 30
    sem_t *semaphore = sem_open(SEMAPHORE_NAME, O_CREAT, 0644, NUM_ROOMS);
    if (semaphore == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    // Создаем общую память, в которой будем хранить количество свободных номеров
    int shared_memory_fd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0644);
    if (shared_memory_fd == -1) {
        perror("shm_open");
        exit(1);
    }
    ftruncate(shared_memory_fd, sizeof(int));
    int *num_free_rooms = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_fd, 0);
    if (num_free_rooms == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    *num_free_rooms = NUM_ROOMS;

    // Основной цикл работы гостиницы
    while (1) {
        // Ожидаем появления клиента
        sleep(1);

        // Пытаемся забронировать номер, уменьшая значение семафора
        if (sem_trywait(semaphore) == 0) {
            // Если номер свободен, уменьшаем количество свободных номеров в общей памяти
            (*num_free_rooms)--;
            printf("Client checked in\n");
            sleep(2);           // Клиент остается в номере на 2 суток
            (*num_free_rooms)++;// Когда клиент выезжает, увеличиваем количество свободных номеров
            sem_post(semaphore);// Увеличиваем значение семафора
            printf("Client checked out\n");
        } else {
            // Если нет свободных номеров, сообщаем об этом
            printf("No rooms available\n");
        }
    }

    // Закрываем семафор и освобождаем память
    sem_close(semaphore);
    sem_unlink(SEMAPHORE_NAME);
    munmap(num_free_rooms, sizeof(int));
    close(shared_memory_fd);
    shm_unlink(SHARED_MEMORY_NAME);

    return 0;
}
