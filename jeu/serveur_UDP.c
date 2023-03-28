/*  Application MIROIR  : cote serveur      */

#include "commun.h"
#include "jeu.h"

void miroir(char *chaine);

int main (int argc, char **argv) {

    /*---------------------------------------------- les variables */

        /* pour le serveur */
    struct sockaddr_in adUDP_serveur;   /* pour mettre l'adresse de son point d'acc�s */
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

    /*---------------------------------------------- l'algorithme */

        /* v�rification des arguments */
    if (argc != 2) {
        printf("Usage : %s port\n",argv[0]);
        exit(-1);
    }
        /* cr�ation d'un point d'acc�s */
    point_acces_serveur = socket (PF_INET,SOCK_DGRAM,0);
    if (point_acces_serveur < 0) {
        perror("ERREUR-socket ");
        exit(-1);
    }

        /* pr�paration de l'adresse du point d'acc�s */
    bzero(&adUDP_serveur,sizeof(adUDP_serveur));
    adUDP_serveur.sin_family = AF_INET;
    adUDP_serveur.sin_port = htons(atoi(argv[1]));
    adUDP_serveur.sin_addr.s_addr = INADDR_ANY;

        /* attribution de l'adresse au point d'acc�s */
    retour = bind(point_acces_serveur,
            (struct sockaddr *)&adUDP_serveur, sizeof(adUDP_serveur));
    if (retour < 0 ) {
        perror("ERREUR-bind ");
        exit(-1);
    }


        /* phase de transfert */
    while(1) {
        switch(state){
        case 0: //INIT
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

                    strcpy(message,"INIT_J1");                            ///INIT_J1///
                }
                else if (adresse_expediteur.sin_port != addr_j1.sin_port && addr_j2.sin_port == 0) {
                    addr_j2.sin_port=adresse_expediteur.sin_port;
                    addr_j2.sin_family=adresse_expediteur.sin_family;

                    strcpy(message,"INIT_J2");                            ///INIT_J2///
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
                    state= 1;
                    printf("\tPL_OK\n");

                    strcpy(message,"INIT_OK");                           ///INIT_OK///

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
                }
            }

            init_morpion();

            break;

        case 1: //attente choix case joueur 1
            lg_expediteur=sizeof(adresse_expediteur);
            recus = recvfrom(point_acces_serveur,
                    message, sizeof(message), 0,
                    (struct sockaddr *) &adresse_expediteur, &lg_expediteur);

            if (recus < 0) {
                perror("ERREUR-recvfrom ");
            }
            else if (verif_typ(message) != 0) {
                strcpy(message, "TYP_NOK");

                printf("\tTYP_NOK\n");
            }
            else if (verif_case(message) != 0) {
                strcpy(message, "CASE_NOK");

                printf("\tCASE_NOK\n");
            }
            else {
                morpion[message[0]-48][message[1]-48]= 1;

                printf("\tCASE_RCV\n");

                strcpy(message,"CASE_OK");
                emis =  sendto (point_acces_serveur,
                        message, strlen(message)+1, 0,
                        (struct sockaddr *) &adresse_expediteur, sizeof(adresse_expediteur));

                printf("\tCASE_OK\n\n");
                prin(morpion);
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
