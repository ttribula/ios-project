
#include "process_gen.h"
#include "components.h"


int main(int argc, char *argv[]) {

    if (!(output = fopen("./proj2.out", "w"))) {
        fprintf(stderr, "CHYBA: Nelze otevrit soubor proj2.out.\n");
        exit(E_OTHERS);
    }

    //output = stdout; // for quick debugging

    int err;
    if ((err = parse_args(argc, argv))) {// zpracovani argumentu
        return err;
    }

    if ((err = init_shm())) {           // inicializace sdilene pameti
        clean_up();
        fprintf(stderr, "CHYBA: Nepodarilo se inicializovat sdilenou pamet.");
        return err;
    }

    if ((err = init_semaphores())) {    // inicializace semaforu
        clean_up();
        fprintf(stderr, "CHYBA: Nepodarilo se inicializovat semafory.");
        return err;
    }
    pid_t main_process;
    setbuf(output, NULL);
    main_process = fork();
    if (main_process == 0) {
        //proces soudce
        generate_judge();
        exit(0);
    } else if (main_process > 0) {
        //proces generujici pristehovalce
        for (int i = 0; i < params.PI; ++i) {
            pid_t imm = fork();
            if (imm == 0) {
                generate_imm(i + 1);
                exit(0);
            } else if (imm > 0) {
                continue;
            } else {
                fprintf(stderr, "CHYBA: Nastala chyba pri fork() pristehovalce.");
                clean_up();
                exit(E_OTHERS);
            }
            wait_time(params.IG);
        }
    } else {
        fprintf(stderr, "CHYBA: Nastala chyba pri fork() soudce.");
        clean_up();
        exit(E_OTHERS);
    }
    //cekani na ukonceni vsech procesu
    for(int i = 0; i < params.PI + 1; i++) {
        wait(NULL);
    }

    clean_up();

    fclose(output);

    return 0;
}

