#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "buffer.h"

extern void buffer_init();
extern void *producer(void *param);
extern void *consumer(void *param);

int total_produced_threads = 0;
int total_consumed_threads = 0;

void *producer(void *param) {
    buffer_item item;
    int max_sleep = *(int *)param;
    while (1) {
        sleep(rand() % max_sleep);  // Random sleep time
        item = rand();  // Produce a random number
        if (buffer_insert_item(item)) {
            printf("Producer produced %d\n", item);
            total_produced_threads++;
        } else {
            printf("Buffer full. Producer waiting.\n");
        }
    }
}

void *consumer(void *param) {
    buffer_item item;
    int max_sleep = *(int *)param;
    while (1) {
        sleep(rand() % max_sleep);  // Random sleep time
        if (buffer_remove_item(&item)) {
            printf("Consumer consumed %d\n", item);
            total_consumed_threads++;
        } else {
            printf("Buffer empty. Consumer waiting.\n");
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 6) {
        fprintf(stderr, "Usage: %s <sleep time> <max sleep> <num producers> <num consumers> <display buffer (yes/no)>\n", argv[0]);
        return -1;
    }

    int sleep_time = atoi(argv[1]);
    int max_sleep = atoi(argv[2]);
    int num_producers = atoi(argv[3]);
    int num_consumers = atoi(argv[4]);
    bool display_buffer = (argv[5][0] == 'y' || argv[5][0] == 'Y');

    buffer_init();

    pthread_t producers[num_producers];
    pthread_t consumers[num_consumers];

    for (int i = 0; i < num_producers; i++) {
        pthread_create(&producers[i], NULL, producer, &max_sleep);
    }

    for (int i = 0; i < num_consumers; i++) {
        pthread_create(&consumers[i], NULL, consumer, &max_sleep);
    }

    sleep(sleep_time);

    for (int i = 0; i < num_producers; i++) {
        pthread_cancel(producers[i]);
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < num_consumers; i++) {
        pthread_cancel(consumers[i]);
        pthread_join(consumers[i], NULL);
    }

    // Display statistics
    printf("\nPRODUCER / CONSUMER SIMULATION COMPLETE\n");
    printf("=======================================\n");
    printf("Simulation Time:\t\t%d\n", sleep_time);
    printf("Maximum Thread Sleep Time:\t%d\n", max_sleep);
    printf("Number of Producer Threads:\t%d\n", num_producers);
    printf("Number of Consumer Threads:\t%d\n", num_consumers);
    printf("Size of Buffer:\t\t\t%d\n", BUFFER_SIZE);
    printf("\nTotal Number of Items Produced:\t%d\n", total_produced);
    printf("\nTotal Number of Items Consumed:\t%d\n", total_consumed);
    printf("\nNumber Of Times Buffer Was Full:\t%d\n", buffer_full_count);
    printf("Number Of Times Buffer Was Empty:\t%d\n", buffer_empty_count);
    printf("\nNumber Of Items Remaining in Buffer:\t%d\n", count);

    return 0;
}
