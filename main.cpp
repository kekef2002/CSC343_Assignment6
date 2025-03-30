/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <pthread.h>
#include <unistd.h> // For sleep function

// Shared data
std::string sharedData;
bool dataReady = false;

// Mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* writerThread(void* arg) {
    pthread_mutex_lock(&mutex);
    sharedData = "Hello from Thread_1!";
    dataReady = true;
    std::cout << "[Writer] Data written to shared variable.\n";

    // Signal the reader that data is ready
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    return nullptr;
}

void* readerThread(void* arg) {
    pthread_mutex_lock(&mutex);
    while (!dataReady) {
        pthread_cond_wait(&cond, &mutex);
    }
    std::cout << "[Reader] Data read from shared variable: " << sharedData << "\n";
    pthread_mutex_unlock(&mutex);

    return nullptr;
}

int main() {
    pthread_t t1, t2;

    // Create writer and reader threads
    pthread_create(&t1, nullptr, writerThread, nullptr);
    pthread_create(&t2, nullptr, readerThread, nullptr);

    // Wait for both threads to finish
    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);

    // Clean up
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
