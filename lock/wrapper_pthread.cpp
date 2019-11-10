#include <pthread.h>

pthread_mutex_t mutex_lock=PTHREAD_MUTEX_INITIALIZER;

extern "C" void wrapper_mutex_lock(){
    pthread_mutex_lock(&mutex_lock);
}

extern "C" void wrapper_mutex_unlock(){
    pthread_mutex_unlock(&mutex_lock);
}
