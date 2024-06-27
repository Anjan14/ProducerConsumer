#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include "buffer.h"

buffer_item buffer[BUFFER_SIZE];
int insert_position = 0;
int remove_position = 0;
int count = 0;

pthread_mutex_t mutex;
sem_t empty;
sem_t full;

void buffer_init() {
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
}

bool buffer_insert_item(buffer_item item) {
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    if (count == BUFFER_SIZE) {
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        return false;
    }

    buffer[insert_position] = item;
    insert_position = (insert_position + 1) % BUFFER_SIZE;
    count++;

    pthread_mutex_unlock(&mutex);
    sem_post(&full);
    return true;
}

bool buffer_remove_item(buffer_item *item) {
    sem_wait(&full);
    pthread_mutex_lock(&mutex);

    if (count == 0) {
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        return false;
    }

    *item = buffer[remove_position];
    remove_position = (remove_position + 1) % BUFFER_SIZE;
    count--;

    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    return true;
}
