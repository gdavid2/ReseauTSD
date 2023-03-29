/*  Application MIROIR  : cote serveur      */

#include "commun.h"
#include "jeu.h"

void miroir(char *chaine);

int main (int argc, char **argv) {

    /*---------------------------------------------- les variables */

        /* pour le serveur */
    struct sockaddr_in adUDP_serveur;   /* pour mettre l'adresse de son point d'accès */
    int point_acces_serveur;

        /* pour le service */
    struct sockaddr_in adresse_expediteur;
    struct sockaddr_in addr_j1;
    struct sockaddr_in addr_j2;

    addr_j1.sin_family=0;
    addr_j1.sin_port=0;
    addr_j1.sin_addr.s_addr=0;

    addr_j2.sin_family=0;
    addr_j2.sin_port=0;
    addr_j2.sin_addr.s_addr=0;

#ifdef  __osf__
    int lg_expediteur;
#else
    socklen_t lg_expediteur;
#endif
    char message[100];
    int recus, emis;

        /* divers */
    int retour;
    int state =0;
    char case_chx[100];

    /*---------------------------------------------- l'algorithme */

        /* vérification des arguments */
    if (argc != 2) {
        printf("Usage : %s port\n",argv[0]);
        exit(-1);
    }
        /* création d'un point d'accès */
    point_acces_serveur = socket (PF_INET,SOCK_DGRAM,0);
    if (point_acces_serveur < 0) {
        perror("ERREUR-socket ");
        exit(-1);
    }

        /* préparation de l'adresse du point d'accès */
    bzero(&adUDP_serveur,sizeof(adUDP_serveur));
    adUDP_serveur.sin_family = AF_INET;
    adUDP_serveur.sin_port = htons(atoi(argv[1]));
    adUDP_serveur.sin_addr.s_addr = INADDR_ANY;

        /* attribution de l'adresse au point d'accès */
    retour = bind(point_acces_serveur,
            (struct sockaddr *)&adUDP_serveur, sizeof(adUDP_serveur));
    if (retour < 0 ) {
        perror("ERREUR-bind ");
        exit(-1);
    }


        /* phase de transfert */
    while(1) {
        switch(state){
        case 0: //init
            printf("--------state 0\n");

            printf("\tPL_WAI\n");

            lg_expediteur=sizeof(adresse_expediteur);
            recus = recvfrom(point_acces_serveur,
                    message, sizeof(message), 0,
                    (struct sockaddr *) &adresse_expediteur, &lg_expediteur);

            if (recus < 0) {
                perror("ERREUR-recvfrom ");
            }
            else {
                if (addr_j1.sin_port == 0) {
                    addr_j1.sin_port   = adresse_expediteur.sin_port;
                    addr_j1.sin_family = adresse_expediteur.sin_family;

                    strcpy(message,"INIT_J1");
                }
                else if (adresse_expediteur.sin_port != addr_j1.sin_port && addr_j2.sin_port == 0) {
                    addr_j2.sin_port   = adresse_expediteur.sin_port;
                    addr_j2.sin_family = adresse_expediteur.sin_family;

                    strcpy(message,"INIT_J2");
                }

                emis =  sendto (point_acces_serveur,
                        message, strlen(message)+1, 0,
                        (struct sockaddr *) &adresse_expediteur, sizeof(adresse_expediteur));

                if (emis < 0) {
                    perror("ERREUR-sendto ");
                }
                else {
                    printf("\t%s\n\n",message);
                }

                if (addr_j1.sin_port != 0 && addr_j2.sin_port != 0) {
                    printf("\tPL_OK\n");

                    strcpy(message,"INIT_OK");

                    printf("\t%s\n\n",message);

                    emis =  sendto (point_acces_serveur,
                            message, strlen(message)+1, 0,
                            (struct sockaddr *) &addr_j1, sizeof(addr_j1));
                    if (emis < 0) {
                        perror("ERREUR-sendto ");
                    }

                    emis =  sendto (point_acces_serveur,
                            message, strlen(message)+1, 0,
                            (struct sockaddr *) &addr_j2, sizeof(addr_j2));
                    if (emis < 0) {
                        perror("ERREUR-sendto ");
                    }

                    state= 1;
                }
            }

            init_morpion();

            break;

        case 1: //choix case joueur
            printf("--------state 1\n");

            lg_expediteur=sizeof(adresse_expediteur);
            recus = recvfrom(point_acces_serveur,
                    message, sizeof(message), 0,
                    (struct sockaddr *) &adresse_expediteur, &lg_expediteur);

            printf("\tCASE_RCV\n");

            if (recus < 0) {
                perror("ERREUR-recvfrom ");
            }
            else if (verif_typ(message) != 0) {
                strcpy(message, "CASE_NOK");

                emis =  sendto (point_acces_serveur,
                        message, strlen(message)+1, 0,
                        (struct sockaddr *) &adresse_expediteur, sizeof(adresse_expediteur));

                printf("\tCASE_NOK\n\n");
            }
            else if (verif_endg(message) != 0) {
                strcpy(message, "END_GAME");

                printf("\tEND_GAME\n\n");

                state= 4;
            }
            else {
                strcpy(case_chx, message);

                if (adresse_expediteur.sin_port == addr_j1.sin_port) {
                    morpion[message[0]-48][message[1]-48]= 1;

                    adresse_expediteur.sin_port   = addr_j2.sin_port;
                    adresse_expediteur.sin_family = addr_j2.sin_family;
                }
                else if (adresse_expediteur.sin_port == addr_j2.sin_port) {
                    morpion[message[0]-48][message[1]-48]= 2;

                    adresse_expediteur.sin_port   = addr_j1.sin_port;
                    adresse_expediteur.sin_family = addr_j1.sin_family;
                }

                printf("\tCASE_OK\n\n");
                prin(morpion);
                printf("\n");

                strcpy(message,"ACT_MRP");
                emis =  sendto (point_acces_serveur,
                        message, strlen(message)+1, 0,
                        (struct sockaddr *) &adresse_expediteur, sizeof(adresse_expediteur));

                if (emis < 0) {
                    perror("ERREUR-sendto ");
                }
                else {
                    printf("\tACT_MRP\n\n");

                    state= verif_endg();;
                }
            }

            break;

        case 2: //actualisation du morpion
            printf("--------state 2\n");

            lg_expediteur=sizeof(adresse_expediteur);
            recus = recvfrom(point_acces_serveur,
                    message, sizeof(message), 0,
                    (struct sockaddr *) &adresse_expediteur, &lg_expediteur);

            if (recus < 0) {
                perror("ERREUR-recvfrom ");
            }
            else if (strcmp(message, "ACT_ACK") == 0) {
                printf("\tMRP_ACK\n\n");

                strcpy(message, case_chx);
                emis =  sendto (point_acces_serveur,
                        message, strlen(message)+1, 0,
                        (struct sockaddr *) &adresse_expediteur, sizeof(adresse_expediteur));

                if (recus < 0) {
                    perror("ERREUR-recvfrom ");
                }
                else {
                    printf("\tACT_CHX\n");

                    lg_expediteur=sizeof(adresse_expediteur);
                    recus = recvfrom(point_acces_serveur,
                            message, sizeof(message), 0,
                            (struct sockaddr *) &adresse_expediteur, &lg_expediteur);

                    if (recus < 0) {
                        perror("ERREUR-recvfrom ");
                    }
                    else if (strcmp(message,"CHX_ACK") == 0) {
                        printf("\tCHX_ACK\n\n");
                        state= 3;
                    }
                }
            }

            break;

        case 3: //changement joueur
            printf("--------state 3\n");
            printf("\tCHG_PLA\n");

            strcpy(message,"CHG_PLA");

            emis =  sendto (point_acces_serveur,
                    message, strlen(message)+1, 0,
                    (struct sockaddr *) &addr_j1, sizeof(addr_j1));

            if (emis < 0) {
                perror("ERREUR-sendto ");
            }
            else {
                lg_expediteur=sizeof(addr_j1);
                recus = recvfrom(point_acces_serveur,
                        message, sizeof(message), 0,
                        (struct sockaddr *) &addr_j1, &lg_expediteur);

                if (emis < 0) {
                    perror("ERREUR-sendto ");
                }
                else {
                    printf("\tCHG_ACK1\n\n");
                }
            }

            printf("\tCHG_PLA\n");

            strcpy(message,"CHG_PLA");
            emis =  sendto (point_acces_serveur,
                    message, strlen(message)+1, 0,
                    (struct sockaddr *) &addr_j2, sizeof(addr_j2));

            if (emis < 0) {
                perror("ERREUR-sendto ");
            }
            else {
                lg_expediteur=sizeof(addr_j2);
                recus = recvfrom(point_acces_serveur,
                        message, sizeof(message), 0,
                        (struct sockaddr *) &addr_j2, &lg_expediteur);

                if (emis < 0) {
                    perror("ERREUR-sendto ");
                }
                else {
                    printf("\tCHG_ACK2\n\n");
                }
            }

            state= 1;

            break;

        case 4: //victoire j1
            printf("--------state 4\n");

            printf("\tVIC_J1\n");
            strcpy(message,"VIC_J");

            emis =  sendto (point_acces_serveur,
                    message, strlen(message)+1, 0,
                    (struct sockaddr *) &addr_j1, sizeof(addr_j1));

            if (emis < 0) {
                perror("ERREUR-sendto ");
            }
            else {
                lg_expediteur=sizeof(adresse_expediteur);
                recus = recvfrom(point_acces_serveur,
                        message, sizeof(message), 0,
                        (struct sockaddr *) &addr_j1, &lg_expediteur);

                if (recus < 0) {
                    perror("ERREUR-recvfrom ");
                }
                else if (strcmp(message,"VIC_ACK") == 0) {
                    printf("\tVIC_ACK\n\n");
                }
            }

            printf("\tDEF_J2\n");
            strcpy(message,"DEF_J");

            emis =  sendto (point_acces_serveur,
                    message, strlen(message)+1, 0,
                    (struct sockaddr *) &addr_j2, sizeof(addr_j2));

            if (emis < 0) {
                perror("ERREUR-sendto ");
            }
            else {
                lg_expediteur=sizeof(adresse_expediteur);
                recus = recvfrom(point_acces_serveur,
                        message, sizeof(message), 0,
                        (struct sockaddr *) &addr_j2, &lg_expediteur);

                if (recus < 0) {
                    perror("ERREUR-recvfrom ");
                }
                else if (strcmp(message,"DEF_ACK") == 0) {
                    printf("\tDEF_ACK\n\n");
                }
            }

            break;

        case 5: //victoire j2
            printf("--------state 5\n");

            printf("\tVIC_J2\n");
            strcpy(message,"VIC_J");

            emis =  sendto (point_acces_serveur,
                    message, strlen(message)+1, 0,
                    (struct sockaddr *) &addr_j2, sizeof(addr_j2));

            if (emis < 0) {
                perror("ERREUR-sendto ");
            }
            else {
                lg_expediteur=sizeof(adresse_expediteur);
                recus = recvfrom(point_acces_serveur,
                        message, sizeof(message), 0,
                        (struct sockaddr *) &addr_j2, &lg_expediteur);

                if (recus < 0) {
                    perror("ERREUR-recvfrom ");
                }
                else if (strcmp(message,"VIC_ACK") == 0) {
                    printf("\tVIC_ACK\n\n");
                }
            }

            printf("\tDEF_J1\n");
            strcpy(message,"DEF_J");

            emis =  sendto (point_acces_serveur,
                    message, strlen(message)+1, 0,
                    (struct sockaddr *) &addr_j1, sizeof(addr_j1));

            if (emis < 0) {
                perror("ERREUR-sendto ");
            }
            else {
                lg_expediteur=sizeof(adresse_expediteur);
                recus = recvfrom(point_acces_serveur,
                        message, sizeof(message), 0,
                        (struct sockaddr *) &addr_j1, &lg_expediteur);

                if (recus < 0) {
                    perror("ERREUR-recvfrom ");
                }
                else if (strcmp(message,"DEF_ACK") == 0) {
                    printf("\tDEF_ACK\n\n");
                }
            }

            break;

        default:
            break;
        }
    }
}

void miroir(char *chaine) {
    char chaine_miroir[200];
    int i;

    for (i=0; i<strlen(chaine); i++) {
        chaine_miroir[strlen(chaine)-1-i] = chaine[i];
    }
    chaine_miroir[strlen(chaine)] = '\0';
    strcpy(chaine,chaine_miroir);
}
