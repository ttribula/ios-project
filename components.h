
#ifndef IOS_PROJECT_COMPONENTS_H
#define IOS_PROJECT_COMPONENTS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/mman.h>

/**
 * Enum navratovych kodu
 * Obsahuje navratove kody programu
 */
typedef enum {
    E_OK,
    E_ARG,
    E_SEM,
    E_SHM,
    E_OTHERS
} ERROR_CODES;

/**
 * Struktura sdilene pameti
 */
typedef struct {
    int A;          /**< Poradove cislo akce. */
    int *p_A;
    int NE;         /**< Aktualni pocet IMM, kteri vstoupili do budovy a nebylo o nich rozhodnuto. */
    int *p_NE;
    int NC;         /**< Aktualni pocet IMM, kteri se zaregistrovali a nebylo o nich rozhodnuto. */
    int *p_NC;
    int NB;         /**< Aktualni pocet IMM, kteri vstoupili do budovy. */
    int *p_NB;
    int judge;      /**< Indikuje, zda soudce vesel do budovy. */
    int *p_judge;
    int conf_no;
    int *p_conf_no;
} SHARED_MEM;

SHARED_MEM shared_mem;

/**
 * Struktura semaforu
 */
typedef struct {
    sem_t *mutex;            /**< Pristup do sdilene pameti. */
    sem_t *no_judge;         /**< Pro vstup/vystup IMM do/z mistnosti. */
    sem_t *confirmed;        /**< Vydani potvrzeni. */
    sem_t *all_signed;       /**< Znameni soudci, ze ma zacit potvrzovani. */
} SEMAPHORES;

SEMAPHORES semaphores;

/**
 * Struktura parametru
 */
typedef struct {
    int PI;     /**< Pocet procesu vygenerovanych procesu IMM. P >= 1*/
    int IG;     /**< Maximalni doba, po ktere se generuje proces IMM. IG >= 0 && IG <= 2000 .*/
    int JG;     /**< Maximalni doba, po ktere se JUDGE vrati do budovy. JG >= 0 && JG <= 2000 .*/
    int IT;     /**< Maximalni doba vyzvednuti ceritfikatu. IT >= 0 && IT <= 2000 .*/
    int JT;      /**< Maximalni doba vydavani rozhodntuti. JT >= 0 && JT <= 2000 .*/
} Params;

Params params;


FILE *output;


int parse_args(int argc, char *argv[]);
int init_shm();
int init_semaphores();
int release_memory();
int unlink_semaphores();
void clean_up();
void wait_time(int max);

#endif //IOS_PROJECT_COMPONENTS_H
