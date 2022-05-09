#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#define n_childs 2

void *oldhandle;
void sighandle(int sig) {
    // printf("[%d] signal %d cached\n", getpid(), sig);
}

int main() {

    pid_t pidchilds[n_childs];
    int i, h;

    //SET SIGNAL
    oldhandle = signal(SIGUSR1, sighandle);
    if (oldhandle == SIG_ERR) {
        perror("signal:");
        exit(EXIT_FAILURE);
    }


    //CREA LOS HIJOS SIEMPRE
    for (i = 0 ; i < n_childs ; i++) {
        pidchilds[i] = fork();
        if (pidchilds[i] == 0) {
            h = fork();
            break;
        }
    }

    if (i == 2) { //PARA ENTRAR EN EL PADRE
        printf("Padre\n");
        usleep(10000);
        kill(pidchilds[1], SIGUSR1);
        pause();
        printf("Padre_end\n");
        for (int j = 0; j < n_childs; j++) {
            wait(NULL);
        }
    }
    else { //PARA LOS HIJOS
        pause();
        printf("Hijo PID[%d]\n", getpid());
        if (h !=  0) {
            usleep(10000);
            kill(h, SIGUSR1);
            pause();
            printf("Hijo PID[%d]\n", getpid());
            if (i!= 0) {
                kill(pidchilds[i-1], SIGUSR1);
            }
            else {
                kill(getppid(), SIGUSR1);
            }
        }
        else {
            kill(getppid(), SIGUSR1);
        }
    }
    
    //DEJA TODO COMO ESTABA (BUENA PRACTICA)
    if(signal(SIGUSR1, oldhandle) == SIG_ERR){
        perror("signal:");
        exit(EXIT_FAILURE);
    }


    return EXIT_SUCCESS;
}