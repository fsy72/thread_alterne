#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>

int i = 1, compteur = 0;

sem_t sem_se, sem_te;

void *fthreadPlus_n(void *arg) {
    int n_arg = *((int*)arg);
    while(1) {
        sem_wait(&sem_se);
        if(compteur != n_arg) {
            printf("%d %d\n", i++, compteur++);
            sem_post(&sem_se);
        }
        else
            sem_post(&sem_te);
    }
    pthread_exit(NULL);
}
void *fthreadMoins_n(void *arg) {
    int n_arg = *((int*)arg);
    while(1) {
        sem_wait(&sem_te);
        if(compteur != -(n_arg)) {
            printf("%d %d\n", i++, compteur--);
            sem_post(&sem_te);
        }
        else
            sem_post(&sem_se);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    int n_th;

    if(argc < 2 || (n_th = atoi(argv[1])) <= 0) {
        printf("Usage: %s n\nn étant un entier strictement positif\n", argv[0]);
        exit(-1);
    }


    int resultat = sem_init(&sem_se, 0, 1);
    if(resultat==-1) {
        perror("Error semaphore secondaire");
    }

    resultat = sem_init(&sem_te, 0, 0);
    if(resultat==-1) {
        perror("Error semaphore tertiaire");
    }

    pthread_t th1, th2;
    resultat = pthread_create(&th1, NULL, fthreadPlus_n, &n_th);
    if(resultat==-1) {
        perror("Error thread secondaire");
    }

    resultat = pthread_create(&th2, NULL, fthreadMoins_n, &n_th);
    if(resultat==-1) {
        perror("Error thread tertiaire");
    }

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    sem_destroy(&sem_te);
    sem_destroy(&sem_se);
    return 0;
}

