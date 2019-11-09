#include <pthread.h>

pthread_mutex_t mutex_lock;


extern "C" void wrapper_mutex_init(){
    pthread_mutex_init(&mutex_lock, NULL);
}

extern "C" void wrapper_mutex_lock(){
    pthread_mutex_lock(&mutex_lock);
}

extern "C" void wrapper_mutex_unlock(){
    pthread_mutex_unlock(&mutex_lock);
}
