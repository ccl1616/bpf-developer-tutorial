#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>

#define NUM_THREADS 3
#define WORK_CYCLES 10

// Global mutex for lock contention demonstration
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Function that does some CPU work (light)
void cpu_work(int thread_id, int cycle) {
    printf("Thread %d: Cycle %d - Doing light CPU work\n", thread_id, cycle);
    
    // Light CPU work - just some simple calculations
    volatile int sum = 0;
    for (int i = 0; i < 100000; i++) {
        sum += i * i;
    }
    
    printf("Thread %d: Cycle %d - CPU work done (sum=%d)\n", thread_id, cycle, sum);
}

// Function that does blocking I/O work
void io_work(int thread_id, int cycle) {
    printf("Thread %d: Cycle %d - Starting I/O work\n", thread_id, cycle);
    
    // Simulate I/O blocking with sleep
    usleep(500000); // Sleep for 500ms (500,000 microseconds)
    
    printf("Thread %d: Cycle %d - I/O work completed\n", thread_id, cycle);
}

// Function that demonstrates lock contention
void lock_work(int thread_id, int cycle) {
    printf("Thread %d: Cycle %d - Waiting for lock\n", thread_id, cycle);
    
    pthread_mutex_lock(&lock);
    printf("Thread %d: Cycle %d - Got lock, doing work\n", thread_id, cycle);
    
    // Hold the lock for a while to create contention
    usleep(200000); // Sleep for 200ms while holding lock
    
    pthread_mutex_unlock(&lock);
    printf("Thread %d: Cycle %d - Released lock\n", thread_id, cycle);
}

// Worker thread function
void* worker_thread(void* arg) {
    int thread_id = *(int*)arg;
    
    printf("Thread %d started\n", thread_id);
    
    for (int cycle = 1; cycle <= WORK_CYCLES; cycle++) {
        // Mix of different types of work
        switch (cycle % 3) {
            case 0:
                cpu_work(thread_id, cycle);
                break;
            case 1:
                io_work(thread_id, cycle);
                break;
            case 2:
                lock_work(thread_id, cycle);
                break;
        }
        
        // Small delay between cycles
        usleep(100000); // 100ms delay
    }
    
    printf("Thread %d finished\n", thread_id);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    
    printf("Starting test target program (PID: %d)\n", getpid());
    printf("This program will run for about %d seconds\n", WORK_CYCLES * 2);
    printf("It demonstrates:\n");
    printf("- Light CPU work (no heavy computation)\n");
    printf("- I/O blocking (sleep operations)\n");
    printf("- Lock contention (mutex waits)\n");
    printf("\n");
    
    // Create worker threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        if (pthread_create(&threads[i], NULL, worker_thread, &thread_ids[i]) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }
    
    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("\nAll threads completed. Program finished.\n");
    return 0;
}
