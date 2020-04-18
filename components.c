#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <time.h>

#include "components.h"


int parse_args(int argc, char *argv[]) {
    int *err = NULL;
    if (argc != 6) {
        fprintf(stderr, "Spatny pocet argumentu programu.\n");
        return E_ARG;
    }
    params->PI = strtol(argv[1], &err, 10);
    params->IG = strtol(argv[2], &err, 10);
    params->JG = strtol(argv[3], &err, 10);
    params->IT = strtol(argv[4], &err, 10);
    params->JT = strtol(argv[5], &err, 10);

    if (*err ||
        params->PI < 1 ||
        (params->IG < 0 && params->IG > 2000) ||
        (params->JG < 0 && params->JG > 2000) ||
        (params->IT < 0 && params->IT > 2000) ||
        (params->JT < 0 && params->JT > 2000)) {
        fprintf(stderr, "Zadane neplatne hodnoty argumentu programu.\n")
        return E_ARG;
    }
}

void init_shm() {
    shared_mem->A = 0;
    shared_mem->NE = 0;
    shared_mem->NC = 0;
    shared_mem->NB = 0;
}

int init_semaphores() {
    if ((semaphores->mutex =
                 mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0) == MAP_FAILED)) {
        fprintf(stderr, "Semafor se nepodarilo namapovat.");
        return E_SEM;
    }
    if ((semaphores->no_judge =
                 mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0) == MAP_FAILED)) {
        fprintf(stderr, "Semafor se nepodarilo namapovat.");
        return E_SEM;
    }
    if ((semaphores->confirmed =
                 mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0) == MAP_FAILED)) {
        fprintf(stderr, "Semafor se nepodarilo namapovat.");
        return E_SEM;
    }
    if ((semaphores->all_signed =
                 mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0) == MAP_FAILED)) {
        fprintf(stderr, "Semafor se nepodarilo namapovat.");
        return E_SEM;
    }
    if ((semaphores->exit_barrier =
                 mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0) == MAP_FAILED)) {
        fprintf(stderr, "Semafor se nepodarilo namapovat.");
        return E_SEM;
    }
    if (sem_init(semaphores->mutex, 1, 1) == -1 ||
         sem_init(semaphores->mutex, 1, 1) == -1 ||
         sem_init(semaphores->mutex, 1, 1) == -1 ||
         sem_init(semaphores->mutex, 1, 1) == -1 ||
         sem_init(semaphores->mutex, 1, 1) == -1 ) {
        fprintf(stderr, "Semafor se nepodarilo inicializovat.");
        return E_SEM;
    }
    return E_OK;
}

int release()