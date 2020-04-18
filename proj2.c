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

int main(int argc, char *argv[]) {
    int err;
    if ((err = parse_args(argc, argv, &params)) ||  // zpracovani argumentu
        (err = init_shm(shared_mem)) ||             // inicializace sdilene pameti
        (err = init_semaphores(semaphores))) {      // inicializace semaforu


        exit(err);
    }



}

