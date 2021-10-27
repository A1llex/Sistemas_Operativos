#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BufferSize 10 // Tam del buffer
#define MaxElem 10 // cantidad de elementos a producir/consumir

sem_t empty; //semaforo para casillas vacias
sem_t full; //semaforo para casillas llenas
int buffer[BufferSize];
pthread_mutex_t mutex;

void *producer() {   
    int elem;
    int in = 0; //indice de la siguiente casilla vacia
    int produjo =0;

    srand (time(NULL)); //inicializa la semilla para random
    for(int i = 0; i < MaxElem; i++) {
        elem = rand() % 100; // numero pseudo-aleatorio entre 0 y 99
        sem_wait(&empty); //verificamos si hay casillas vacias
        pthread_mutex_lock(&mutex); //bloqueamos region critica
        buffer[in] = elem;
        produjo++;
        pthread_mutex_unlock(&mutex); //liberamos la region critica
        printf("Productor  PID  %d vez %i : Inserta %d en %d\n",pthread_self(),i+1, elem,in);
        in = (in+1)%BufferSize;
        sem_post(&full); //indicamos que hay una nueva casiilla llena
    }
    printf("--- Productor PID %d Termino produciendo %i---\n ",pthread_self(),produjo);
}

void *consumer() {
    int elem;
    int out = 0; //indice de la siguiente casilla a consumir
    int consumio=0;

    for(int i = 0; i < MaxElem; i++) {
        sem_wait(&full); //verificamos si hay casillas llenas
        pthread_mutex_lock(&mutex); //bloqueamos la region critica
        elem = buffer[out];
        consumio++;
        pthread_mutex_unlock(&mutex); //liberamos la region critica
        printf("Consumidor PID %d vez %i : Saca %d de %d\n",pthread_self(),i+1,elem, out);
        out = (out+1)%BufferSize;
        sem_post(&empty); //indicamos que hay una nueva casilla vacia
    }
    printf("--- Consumidor PID %d Termino consumiendo %i ---\n ",pthread_self(),consumio);
}

int main() {   
    int L = 2;
    int M = 5;

    pthread_t prod,cons;
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty,0,BufferSize); //inicialmente hay BufferSize casillas vacias
    sem_init(&full,0,0); //inicialmente hay 0 casillas vacias

    //crear productores
    pthread_t producers[L];
    for (int i = 0; i < L; i++) {
        pthread_create(&producers[i], NULL, consumer, NULL);
    }
    
    pthread_t consumers[M];
    for (int i = 0; i < M; i++) {
        pthread_create(&consumers[i], NULL, producer, NULL);
    }

    for (int i = 0; i < L; i++) {
        //esperamos al hilo productor
        pthread_join(producers[i], NULL); 
    }
    
    for (int i = 0; i < M; i++) {
        //esperamos al hilo consumidor
        pthread_join(consumers[i], NULL); 
    }
    
    pthread_mutex_destroy(&mutex); //destruimos el mutex
    sem_destroy(&empty); //destruimos el semaforo
    sem_destroy(&full); //destruimos el semaforo

    return 0;   
}