
#include "process_gen.h"
#include "components.h"

/**
 * Funkce generovani soudce
 */
void generate_judge() {
    int processed = 0;
    //zpracovava pozadavky, dokud nezpracuje vsechny
    while (processed < params.PI) {
        //soudce zamyka semafor pro potvrzeni
        sem_wait(semaphores.confirmed);
        //soudce ceka pred vstupem do mistnosti
        wait_time(params.JG);
        //uzavre semafor no_judge
        sem_wait(semaphores.no_judge);
        sem_wait(semaphores.mutex);
        fprintf(output, "%d     : JUDGE : wants to enter\n", ++(*shared_mem.p_A));
        fprintf(output, "%d     : JUDGE : enters                       : %d    : %d    : %d\n", ++(*shared_mem.p_A),
                *shared_mem.p_NE,
                *shared_mem.p_NC, *shared_mem.p_NB);
        //soudce vstoupil
        (*shared_mem.p_judge) = 1;
        sem_post(semaphores.mutex);
        //pokud se vsichni pristehovalci jeste neregistrovali, ceka az tak udelaji
        if ((*shared_mem.p_NE) != (*shared_mem.p_NC)) {
            sem_wait(semaphores.mutex);
            fprintf(output, "%d     : JUDGE : waits for imm            : %d    : %d    : %d\n", ++(*shared_mem.p_A),
                    *shared_mem.p_NE,
                    *shared_mem.p_NC, *shared_mem.p_NB);
            sem_post(semaphores.mutex);
            //ceka na pristehovalce
            sem_wait(semaphores.all_signed);
        }
        //zacina potvrzovani
        sem_wait(semaphores.mutex);
        fprintf(output, "%d     : JUDGE : starts confirmation          : %d    : %d    : %d\n", ++(*shared_mem.p_A),
                *shared_mem.p_NE,
                *shared_mem.p_NC, *shared_mem.p_NB);
        //uspi se na cas JT
        wait_time(params.JT);

        //inkrementuje cislo potvrzeni
        ++(*shared_mem.p_conf_no);
        //otevre semafor pro pristehovalce
        sem_post(semaphores.confirmed);
        //potvrdi vsechny pristehovalce najednou
        processed += (*shared_mem.p_NC);
        (*shared_mem.p_NE) = 0;
        (*shared_mem.p_NC) = 0;
        //konec potvrzovani
        fprintf(output, "%d     : JUDGE : ends confirmation            : %d    : %d    : %d\n", ++(*shared_mem.p_A),
                *shared_mem.p_NE,
                *shared_mem.p_NC, *shared_mem.p_NB);


        sem_post(semaphores.mutex);
        //uspi se pred odchodem
        wait_time(params.JT);
        //odchazi
        sem_wait(semaphores.mutex);
        (*shared_mem.p_judge) = 0;
        fprintf(output, "%d     : JUDGE : leaves                       : %d    : %d    : %d\n", ++(*shared_mem.p_A),
                *shared_mem.p_NE,
                *shared_mem.p_NC, *shared_mem.p_NB);
        sem_post(semaphores.mutex);
        //odemkne semafor pro nove prichozi
        sem_post(semaphores.no_judge);
    }
    //potvrdil vsechny pristehovalce, konci
    sem_wait(semaphores.mutex);
    fprintf(output, "%d     : JUDGE : finishes\n", ++(*shared_mem.p_A));
    sem_post(semaphores.mutex);
}

/**
 * Funkce generovani pristehovalce
 */
void generate_imm(int id) {
    int imm_conf_no; // urcuje cislo potvrzeni
    //process pristehovalce zacina
    sem_wait(semaphores.mutex);
    fprintf(output, "%d     : IMM %d : starts\n", ++(*shared_mem.p_A), id);
    sem_post(semaphores.mutex);
    //pokud neni soudce v mistnosti, vstupuje, jinak ceka
    sem_wait(semaphores.no_judge);
    sem_wait(semaphores.mutex);
    fprintf(output, "%d     : IMM %d : enters                       : %d    : %d    : %d\n", ++(*shared_mem.p_A), id,
            ++(*shared_mem.p_NE),
            *shared_mem.p_NC, ++(*shared_mem.p_NB));
    sem_post(semaphores.mutex);
    sem_post(semaphores.no_judge);
    //registruje se
    sem_wait(semaphores.mutex);
    imm_conf_no = (*shared_mem.p_conf_no);
    fprintf(output, "%d     : IMM %d : checks                       : %d    : %d    : %d\n", ++(*shared_mem.p_A), id,
            *shared_mem.p_NE,
            ++(*shared_mem.p_NC), *shared_mem.p_NB);
    sem_post(semaphores.mutex);
    //pokud je soudce v mistnosti a vsichni pristehovalci, co vstoupili, se registrovali, otevira semafor pro soudce
    if (((*shared_mem.p_judge) == 1) && (shared_mem.p_NC == shared_mem.p_NE)) {
        sem_post(semaphores.all_signed);
        sem_post(semaphores.mutex);
    } else {
        sem_post(semaphores.mutex);
    }

    //pokud se pristehovalec registroval pote, co soudce vydal potvrzeni, ceka na jeho potvrzeni
    if (((*shared_mem.p_conf_no) <= imm_conf_no) && ((*shared_mem.p_judge) == 1)) {
        sem_wait(semaphores.confirmed);
    }
    //ceka, az je potvrzen
    sem_wait(semaphores.confirmed);
    sem_post(semaphores.confirmed);
    sem_wait(semaphores.mutex);
    //zada o certifikat
    sem_wait(semaphores.mutex);
    fprintf(output, "%d     : IMM %d : wants certificate            : %d    : %d    : %d\n", ++(*shared_mem.p_A), id,
            *shared_mem.p_NE,
            *shared_mem.p_NC, *shared_mem.p_NB);

    sem_post(semaphores.mutex);
    //uspi se na dobu IT
    wait_time(params.IT);
    //vyzvedava si certifikat
    sem_wait(semaphores.mutex);
    fprintf(output, "%d     : IMM %d : got certificate              : %d    : %d    : %d\n", ++(*shared_mem.p_A), id,
            *shared_mem.p_NE,
            *shared_mem.p_NC, *shared_mem.p_NB);
    sem_post(semaphores.mutex);
    //ceka az soudce odejde
    sem_wait(semaphores.no_judge);
    //odchazi
    sem_wait(semaphores.mutex);
    fprintf(output, "%d     : IMM %d : leaves                       : %d    : %d    : %d\n", ++(*shared_mem.p_A), id,
            *shared_mem.p_NE,
            *shared_mem.p_NC, --(*shared_mem.p_NB));
    sem_post(semaphores.mutex);
    sem_post(semaphores.no_judge);
}
