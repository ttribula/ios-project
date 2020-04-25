
#include "components.h"

/**
 * Zpracovani argumentu.
 * @param argc
 * @param argv
 * @return Chybovy kod
 */
int parse_args(int argc, char *argv[]) {
    char *err = NULL;
    if (argc != 6) {
        fprintf(stderr, "CHYBA: Spatny pocet argumentu programu.\n");
        return E_ARG;
    }
    params.PI = strtol(argv[1], &err, 10);
    params.IG = strtol(argv[2], &err, 10);
    params.JG = strtol(argv[3], &err, 10);
    params.IT = strtol(argv[4], &err, 10);
    params.JT = strtol(argv[5], &err, 10);

    if (*err ||
        params.PI < 1 ||
        (params.IG < 0 && params.IG > 2000) ||
        (params.JG < 0 && params.JG > 2000) ||
        (params.IT < 0 && params.IT > 2000) ||
        (params.JT < 0 && params.JT > 2000)) {
        fprintf(stderr, "CHYBA: Zadane neplatne hodnoty argumentu programu.\n");
        return E_ARG;
    }
    return E_OK;
}

/**
 * Inicializace sdilene pameti
 * @return Chybovy kod
 */
int init_shm() {
    if (((shared_mem.A = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1) ||
        !(shared_mem.p_A = (int *) shmat(shared_mem.A, NULL, 1)) ||
        ((shared_mem.NE = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1) ||
        !(shared_mem.p_NE = (int *) shmat(shared_mem.NE, NULL, 0)) ||
        ((shared_mem.NC = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1) ||
        !(shared_mem.p_NC = (int *) shmat(shared_mem.NC, NULL, 0)) ||
        ((shared_mem.judge = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1) ||
        !(shared_mem.p_judge = (int *) shmat(shared_mem.judge, NULL, 0)) ||
        ((shared_mem.conf_no = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1) ||
        !(shared_mem.p_conf_no = (int *) shmat(shared_mem.conf_no, NULL, 0)) ||
        ((shared_mem.NB = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1) ||
        !(shared_mem.p_NB = (int *) shmat(shared_mem.NB, NULL, 0))) {
        fprintf(stderr, "CHYBA: Nepodarilo se vytvorit sdilenou pamet.\n");
        return E_SHM;
    }
    return E_OK;
}

/**
 * Inicializace semaforu
 * @return Chybovy kod
 */
int init_semaphores() {
    if ((semaphores.mutex = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) ==
        MAP_FAILED) {
        return E_SEM;
    }
    if ((semaphores.no_judge = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) ==
        MAP_FAILED) {
        return E_SEM;
    }
    if ((semaphores.all_signed = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) ==
        MAP_FAILED) {
        return E_SEM;
    }
    if ((semaphores.confirmed = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) ==
        MAP_FAILED) {
        return E_SEM;
    }
    if ((sem_init(semaphores.mutex, 1, 1)) == -1) {
        return E_SEM;
    }
    if ((sem_init(semaphores.no_judge, 1, 1)) == -1) {
        return E_SEM;
    }
    if ((sem_init(semaphores.all_signed, 1, 0)) == -1) {
        return E_SEM;
    }
    if ((sem_init(semaphores.confirmed, 1, 1)) == -1) {
        return E_SEM;
    }

    return E_OK;
}

/**
 * Mazani semaforu
 * @return Chybovy kod
 */
int unlink_semaphores() {
    sem_destroy(semaphores.mutex);
    sem_destroy(semaphores.no_judge);
    sem_destroy(semaphores.all_signed);
    sem_destroy(semaphores.confirmed);
    return E_OK;
}

/**
 * Uvolneni sdilene pameti
 * @return Chybovy kod
 */
int release_memory() {
    shmctl(shared_mem.A, IPC_RMID, NULL);
    shmctl(shared_mem.NE, IPC_RMID, NULL);
    shmctl(shared_mem.NC, IPC_RMID, NULL);
    shmctl(shared_mem.NB, IPC_RMID, NULL);
    shmctl(shared_mem.judge, IPC_RMID, NULL);
    shmctl(shared_mem.conf_no, IPC_RMID, NULL);
    return E_OK;
}

/**
 * Uvolneni semaforu a sdilene pameti
 */
void clean_up() {
    unlink_semaphores();
    release_memory();
}

/**
 * Funkce pro uspani procesu na nahodny cas 0 - max
 * @param max Maximalni doba uspani procesu
 */
void wait_time(int max) {
    if (max > 0) {
        usleep(((random() % max) * 1000));
    }
}