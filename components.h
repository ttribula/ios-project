
#ifndef IOS_PROJECT_COMPONENTS_H
#define IOS_PROJECT_COMPONENTS_H

#define MUTEX_NAME "/xtribu02-mutex"
#define IMM_QUEUE "/xtribu02-immque"
#define EXIT_BARRIER "/xtribu02-exitbarr"

/**
 * Enum navratovych kodu
 * Obsahuje navratove kody programu
 */
typedef enum {
    E_OK,
    E_BAD_ARGS,
    E_SEM,
    E_SHM,
    E_OTHERS
} ERROR_CODES;

/**
 * Struktura sdilene pameti
 */
typedef struct {
    int A,      /**< Poradove cislo akce. */
    int NE,     /**< Aktualni pocet IMM, kteri vstoupili do budovy a nebylo o nich rozhodnuto. */
    int NC,     /**< Aktualni pocet IMM, kteri se zaregistrovali a nebylo o nich rozhodnuto. */
    int NB      /**< Aktualni pocet IMM, kteri vstoupili do budovy. */
} SHARED_MEM;

SHARED_MEM shared_mem;

/**
 * Struktura semaforu
 */
typedef struct {
    sem_t *mutex,            /**< Pristup do sdilene pameti. */
    sem_t *no_judge,         /**< Pro vstup IMM do mistnosti. */
    sem_t *confirmed,        /**< Potvrzeni. */
    sem_t *all_signed,       /**< Pro zacatek potvrzovani. */
    sem_t *exit_barrier      /**< Potvrzeni vystupu vsech IMM. */
} SEMAPHORES;

SEMAPHORES semaphores;

/**
 * Struktura parametru
 */
typedef struct {
    int PI,     /**< Pocet procesu vygenerovanych procesu IMM. P >= 1*/
    int IG,     /**< Maximalni doba, po ktere se generuje proces IMM. IG >= 0 && IG <= 2000 .*/
    int JG,     /**< Maximalni doba, po ktere se JUDGE vrati do budovy. JG >= 0 && JG <= 2000 .*/
    int IT,     /**< Maximalni doba vyzvednuti ceritfikatu. IT >= 0 && IT <= 2000 .*/
    int JT      /**< Maximalni doba vydavani rozhodntuti. JT >= 0 && JT <= 2000 .*/
} Params;

Params params;


int parse_args(int argc, char *argv[], Params *params);

void init_shm(SHM shared_mem);

#endif //IOS_PROJECT_COMPONENTS_H
