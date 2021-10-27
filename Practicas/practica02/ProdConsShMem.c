#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

#define BufferSize 5 // Tam del buffer
#define MaxElem 10 // cantidad de elementos a producir/consumir

const char semEmptyname[] = "mySemEmpty";   // nombres
const char semFullname[] = "mySemFull";     // de los
const char semMutexname[] = "mySemMutex";   // semaforos
sem_t *sem_id_empty;
sem_t *sem_id_full;
sem_t *sem_id_mutex;

typedef struct sMem { // estructura de la memoria que se va a compartir
    int buffer[BufferSize]; 
} SharedBuffer;

SharedBuffer *shbuf_ptr; // puntero a la estructura 

void producer() {   
    int elem;
    int in = 0;
    
    srand (time(NULL)); //inicializa la semilla para random
    for(int i = 0; i < MaxElem; i++) {
        elem = rand() % 100; // numero pseudo-aleatorio entre 0 y 99
        sem_wait(sem_id_empty); //verificamos si hay casillas vacias
        sem_wait(sem_id_mutex); //bloqueamos acceso a buffer
        shbuf_ptr->buffer[in] = elem;
        sem_post(sem_id_mutex); //liberamos acceso a buffer
        printf("Productor: Inserta %d en %d\n", elem,in);
        in = (in+1)%BufferSize;
        sem_post(sem_id_full); //indicamos que hay una nueva casiilla llena
    }
}

void consumer() {
    int elem;
    int out=0;

    for(int i = 0; i < MaxElem; i++) {
        sem_wait(sem_id_full); //verificamos si hay casillas llenas
        sem_wait(sem_id_mutex); //bloqueamos acceso a buffer
        elem = shbuf_ptr->buffer[out];
        sem_post(sem_id_mutex); //liberamos la region critica
        printf("Consumidor: Saca %d de %d\n",elem, out);
        out = (out+1)%BufferSize;
        sem_post(sem_id_empty); //indicamos que hay una nueva casilla vacia
    }
}

int main() {   
    int L = 5;
    int M = 5 ;

    pid_t pid;
    key_t memKey = 6655; // llave para acceder a la memoria compartida 
    int memSize = sizeof(SharedBuffer); // tam de la memoria compartida

    int shmem_id = shmget(memKey, memSize, IPC_CREAT); // obtenemos el id de la memoria compartida
    if (shmem_id == -1) {
        printf("shmget fallo");
        exit(1);
    }
    shbuf_ptr = shmat(shmem_id, NULL, 0); // se liga la direccion de la memoria compartida
    if (shbuf_ptr == (void *) - 1) {
        printf("shmat fallo");
        exit(1);
    }
    sem_id_empty = sem_open(semEmptyname, O_CREAT, 0600, BufferSize); // se abre/crea el semaforo
    if (sem_id_empty == SEM_FAILED){
        printf("Productor  : [sem_open] Failed\n"); 
        exit(1);
    }
    sem_id_full = sem_open(semFullname, O_CREAT, 0600, 0); // se abre/crea el semaforo
    if (sem_id_full == SEM_FAILED){
        printf("Productor  : [sem_open] Failed\n"); 
        exit(1);
    }
    sem_id_mutex = sem_open(semMutexname, O_CREAT, 0600, 1); // se abre/crea el semaforo
    if (sem_id_mutex == SEM_FAILED){
        printf("Productor  : [sem_open] Failed\n"); 
        exit(1);
    }

    pid = fork();

    pid_t producers[L];
    for (int i = 0; i < L; i++) {
        ptid_t[i] = fork();
        producer();
    }
    for (int i = 0; i < L; i++)
    {
      wait(NULL);
    }
    
    pid_t consumer[M];
    for (int i = 0; i < M; i++) {
        ptid_t[i] = fork();
        consumer();
    }
    for (int i = 0; i < M; i++)
    {
      wait(NULL);
    }

    sem_unlink(semEmptyname); // se desconecta el semaforo
    sem_unlink(semFullname); // se desconecta el semaforo
    sem_unlink(semMutexname); // se desconecta el semaforo
    hmdt(shbuf_ptr);                 // se desvincula la direccion de la memoria compartida
    shmctl(shmem_id, IPC_RMID, NULL); // se marca el segmento de memoria compartida para ser destruido


    return 0;   
}