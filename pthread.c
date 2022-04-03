//A basic pthread application code
//to compile - gcc pthread.c -pthread

#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void *fun(void *arg){
    int s;
    s = pthread_mutex_lock(&mtx);
    if (s != 0){
        printf("error locking mutex\n");
        exit(EXIT_FAILURE);
    }

    //critical section
    printf("Hello from thread %d\n", *(int *)arg);

    s = pthread_mutex_unlock(&mtx);    
    if (s != 0){
        printf("error unlocking mutex\n");
        exit(EXIT_FAILURE);
    }

    return NULL;
}

int main(){

    pthread_t t1, t2;
    int a = 1, b = 2;

    //creating the threads
    int s = pthread_create(&t1, NULL, &fun, &a);
    if (s != 0){
        printf("error creating thread");
        exit(EXIT_FAILURE);
    }
    s = pthread_create(&t2, NULL, fun, &b);
    if (s != 0){
        printf("error creating thread");
        exit(EXIT_FAILURE);
    }

    //wait for both threads to terminate
    s = pthread_join(t1, NULL);
    if (s != 0){
        printf("error joining");
        exit(EXIT_FAILURE);
    }
    s = pthread_join(t2, NULL);
    if (s != 0){
        printf("error joining");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}