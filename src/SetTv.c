/*=============================================*/
/* processus de lecture d'une zone de memoire  */
/* partagee avec les appels POSIX              */
/*=============================================*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
/*............*/
/* constantes */
/*............*/
#define AREA_TARGET_L    "TARGET_L"    /* ->nom de la zone de lecture target_l              */
#define AREA_TARGET_R    "TARGET_R"    /* ->nom de la zone de lecture target_r              */

#define STOP            "STOP"      /* ->chaine a saisir pour declencher l'arret */
#define STR_LEN         256         /* ->taille par defaut des chaines           */
#define MEMORY_LEN      64

#define NBR_ARG 2

/*######*/
/* main */
/*######*/
int main(int argc, char *argv[])
{
    char areaTarget[STR_LEN];
    double tv_set;


    /* verification qu'il y a le bon nombre d'argument */
    if (argc != NBR_ARG +1) {
        fprintf(stderr,"ERREUR : ---> nombre d'arguments invalides\n");
        return 1;
    }

    if (*argv[1] == 'L')
    {
        strcpy(areaTarget, AREA_TARGET_L);

    }
    else if (*argv[1] == 'R')
    {
        strcpy(areaTarget, AREA_TARGET_R);
    } else
    {
        fprintf(stderr,"ERREUR : ---> parametre directionnel non reconnu\n");
        return 1;
    }

    if (sscanf(argv[2], "%lf", &tv_set)  == 0) 
    {
        fprintf(stderr,"ERREUR : ---> parametre 2: Target velocity doit etre un double\n");
        return 1;
    }

    void *vAddr;                    /* ->adresse virtuelle sur la zone          */
    double *tv;                  /* ->variable partagee pour target                        */
    int  iShmFd;                    /* ->descripteur associe a la zone partagee */
    /*..................................*/
    /* tentative d'acces a la zone */
    /*..................................*/
    /* on essaie de se lier sans creer... */
    if( (iShmFd = shm_open(areaTarget, O_RDWR, 0600)) < 0)
    {  
        fprintf(stderr,"ERREUR : ---> appel a shm_open()\n");
        fprintf(stderr,"         code  = %d (%s)\n", errno, (char *)(strerror(errno)));
        return( -errno );
    };
    /* on attribue la taille a la zone partagee */
    ftruncate(iShmFd, MEMORY_LEN);
    /* tentative de mapping de la zone dans l'espace memoire du */
    /* processus                                                */
    if( (vAddr = mmap(NULL, MEMORY_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, iShmFd, 0 ))  == NULL)
    {
        fprintf(stderr,"ERREUR : ---> appel a mmap()\n");
        fprintf(stderr,"         code  = %d (%s)\n", errno, (char *)(strerror(errno)));
        return( -errno );
    };
    tv = (double *)(vAddr);

    /* saisie */
    *tv = tv_set;
    printf("tv = %lf\n", *tv);
    printf("FIN\n");
    //shm_unlink(areaTarget);
    return( 0 );
}