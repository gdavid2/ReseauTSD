/*  Application MIROIR  : cote clientent        */

#include "commun.h"
#include "jeu.h"

int
main (int argc, char **argv)
{

    /*---------------------------------------------- les variables */

    /* pour le client */
    int point_acces_client; /* le point d'accès */

    /* pour l'expédition */
    struct hostent *adIP_serveur;   /* système où est le serveur */
    /*int port;*/
    struct sockaddr_in adUDP_serveur;   /* pour mettre l'adresse de son point d'accès */

    /* pour l'envoi  */
    char envoyer[100];
    int emis;

    /* pour la réception  */
    struct sockaddr_in adresse_expediteur;
#ifdef  __osf__
    int lg_expediteur;
#else
    socklen_t lg_expediteur;
#endif
    char recu[100];
    int recus;
    int state =0;

    /*---------------------------------------------- l'algorithme */

    /* vérification des arguments */
    if (argc != 3)
    {
        printf ("Usage : %s nom_systeme port\n", argv[0]);
        exit (-1);
    }
    /* récupération des adresses-IP du système où est le serveur */
    if ((adIP_serveur = gethostbyname (argv[1])) == NULL)
    {
        herror("gethostbyname ");
        exit (-1);
    }
    /* création d'un point d'accès */
    point_acces_client = socket (PF_INET, SOCK_DGRAM, 0);
    if (point_acces_client < 0)
    {
        perror ("ERREUR-socket ");
        exit (-1);
    }

    /* préparation de l'adresse destinataire (serveur) */
    bzero (&adUDP_serveur, sizeof (adUDP_serveur));
    adUDP_serveur.sin_family = AF_INET;
    adUDP_serveur.sin_port = htons (atoi (argv[2]));
    memcpy (&adUDP_serveur.sin_addr, adIP_serveur->h_addr, 4);

    /* jeu */
    while (1) {
        switch (state) {
            case 0: //init
                printf("--------state 0\n");

                emis = sendto (point_acces_client,
                               envoyer, strlen(envoyer) + 1, 0,
                               (struct sockaddr *) &adUDP_serveur,
                               sizeof (adUDP_serveur));

                if (emis < 0) {
                    perror ("ERREUR-sendto ");
                }
                else {
                    lg_expediteur = sizeof (adresse_expediteur);
                    recus = recvfrom (point_acces_client,
                            recu, sizeof (recu), 0,
                            (struct sockaddr *) &adresse_expediteur,
                            &lg_expediteur);

                    if (recus < 0){
                        perror ("ERREUR-recvfrom ");
                    }
                    else if (strcmp(recu,"INIT_J1") == 0){
				        printf ("Connexion\n");

                        state= 1;
			        }
                    else if (strcmp(recu,"INIT_J2") == 0){
                        printf ("Connexion\n");
                        printf("Le jeu commence\n");

                        state= 2;
                    }
                }

                break;

            case 1: //joueur attend la connexion de son adversaire
                printf("--------state 1\n");

                lg_expediteur = sizeof(adresse_expediteur);

                recus = recvfrom (point_acces_client,
                        recu, sizeof(recu), 0,
                        (struct sockaddr *) &adresse_expediteur,
                        &lg_expediteur);

                if (recus < 0) {
                    perror ("ERREUR-recvfrom ");
                }
                else if(strcmp(recu,"INIT_OK") == 0) {
                    printf("Le jeu commence\n");

                    state= 3;
                }

                break;

            case 2: //joueur attend le coup de son adversaire
                printf("--------state 2\n");

                lg_expediteur = sizeof(adresse_expediteur);
                recus = recvfrom (point_acces_client,
                        recu, sizeof(recu), 0,
                        (struct sockaddr *) &adresse_expediteur,
                        &lg_expediteur);

                if (recus < 0) {
                    perror ("ERREUR-recvfrom ");
                }
                else if(strcmp(recu,"ACT_MRP") == 0) {
                    strcpy(envoyer, "ACT_ACK");
                    emis = sendto (point_acces_client,
                                   envoyer, strlen (envoyer) + 1, 0,
                                   (struct sockaddr *) &adUDP_serveur,
                                   sizeof (adUDP_serveur));

                    if (emis < 0) {
                        perror ("ERREUR-sendto ");
                    }
                    else {
                        state= 4;
                    }
                }
                else if(strcmp(recu,"INIT_OK") == 0) {
                    printf("En attente du coup de votre adversaire...\n");
                }
                else if(strcmp(recu,"VIC_J") == 0) {
                    strcpy(envoyer, "VIC_ACK");
                    emis = sendto (point_acces_client,
                                   envoyer, strlen (envoyer) + 1, 0,
                                   (struct sockaddr *) &adUDP_serveur,
                                   sizeof (adUDP_serveur));

                    if (emis < 0) {
                        perror ("ERREUR-sendto ");
                    }
                    else {
                        printf("Vous avec gagne !!\n\n");
                    }
                }
                else if(strcmp(recu,"DEF_J") == 0) {
                    strcpy(envoyer, "DEF_ACK");
                    emis = sendto (point_acces_client,
                                   envoyer, strlen (envoyer) + 1, 0,
                                   (struct sockaddr *) &adUDP_serveur,
                                   sizeof (adUDP_serveur));

                    if (emis < 0) {
                        perror ("ERREUR-sendto ");
                    }
                    else {
                        printf("Vous avec perdu :'(\n\n");
                    }
                }

                break;

            case 3: //joueur joue
                printf("--------state 3\n");

                printf("A vous de jouer !\n");
                prin(morpion);
                printf("Choisissez une case.\n");

                printf("ligne (de 0 a 2) : ");
                scanf("%c", &envoyer[0]);
                viderBuffer();

                printf("colonne (de 0 a 2) : ");
                scanf("%c", &envoyer[1]);
                viderBuffer();

                emis = sendto (point_acces_client,
                              envoyer, strlen (envoyer) + 1, 0,
                              (struct sockaddr *) &adUDP_serveur,
                              sizeof (adUDP_serveur));

                if (emis < 0) {
                    perror ("ERREUR-sendto ");
                }
                else {
                    lg_expediteur = sizeof (adresse_expediteur);

                    recus = recvfrom (point_acces_client,
                            recu, sizeof (recu), 0,
                            (struct sockaddr *) &adresse_expediteur,
                            &lg_expediteur);



                    if (recus < 0) {
                        perror ("ERREUR-recvfrom ");
                    }
                    else if (strcmp(recu,"TYP_NOK") == 0) {
                        printf("\nligne et colonne mal renseignees, recommencer.\n\n");
                    }
                    else if (strcmp(recu,"CASE_NOK") == 0) {
                        printf("\nveuillez choisir une case libre (=0)\n\n");
                    }
                    else if(strcmp(recu,"VIC_J") == 0) {
                        strcpy(envoyer, "VIC_ACK");
                        emis = sendto (point_acces_client,
                                       envoyer, strlen (envoyer) + 1, 0,
                                       (struct sockaddr *) &adUDP_serveur,
                                       sizeof (adUDP_serveur));

                        if (emis < 0) {
                            perror ("ERREUR-sendto ");
                        }
                        else {
                            printf("Vous avec gagne !!\n\n");

                            state= 0;
                        }
                    }
                    else if(strcmp(recu,"DEF_J") == 0) {
                        strcpy(envoyer, "DEF_ACK");
                        emis = sendto (point_acces_client,
                                       envoyer, strlen (envoyer) + 1, 0,
                                       (struct sockaddr *) &adUDP_serveur,
                                       sizeof (adUDP_serveur));

                        if (emis < 0) {
                            perror ("ERREUR-sendto ");
                        }
                        else {
                            printf("Vous avec perdu :'(\n\n");

                            state= 0;
                        }
                    }
                    else if (strcmp(recu,"CHG_PLA") == 0) {
                        morpion[envoyer[0]-48][envoyer[1]-48]= 1;

                        printf("\nVous avez joue.\n");
                        prin(morpion);
                        printf("En attente du coup de votre adversaire...\n");

                        strcpy(envoyer, "CHG_ACK");

                        emis = sendto (point_acces_client,
                               envoyer, strlen (envoyer) + 1, 0,
                               (struct sockaddr *) &adUDP_serveur,
                               sizeof (adUDP_serveur));

                        if (recus < 0) {
                            perror ("ERREUR-recvfrom ");
                        }
                        else {
                            state= 2;
                        }
                    }
                }

                break;

            case 4: //joueur apprend le coup de son adversaire
                printf("--------state 4\n");

                lg_expediteur = sizeof(adresse_expediteur);
                recus = recvfrom (point_acces_client,
                        recu, sizeof(recu), 0,
                        (struct sockaddr *) &adresse_expediteur,
                        &lg_expediteur);

                if (recus < 0) {
                    perror ("ERREUR-sendto ");
                }
                else {
                    morpion[recu[0]-48][recu[1]-48]= 2;

                    printf("Votre adversaire a joue.\n");
                    prin(morpion);

                    strcpy(envoyer, "CHX_ACK");
                    emis = sendto (point_acces_client,
                           envoyer, strlen (envoyer) + 1, 0,
                           (struct sockaddr *) &adUDP_serveur,
                           sizeof (adUDP_serveur));

                    if (emis < 0) {
                        perror ("ERREUR-recvfrom ");
                    }
                    else {
                        state= 5;
                    }
                }

                break;

            case 5: //IDLE
                printf("--------state 5\n");

                lg_expediteur = sizeof(adresse_expediteur);
                recus = recvfrom (point_acces_client,
                        recu, sizeof(recu), 0,
                        (struct sockaddr *) &adresse_expediteur,
                        &lg_expediteur);

                if (recus < 0) {
                    perror ("ERREUR-sendto ");
                }
                else if (strcmp(recu,"CHG_PLA") == 0) {
                    strcpy(envoyer, "CHG_ACK");

                    emis = sendto (point_acces_client,
                           envoyer, strlen (envoyer) + 1, 0,
                           (struct sockaddr *) &adUDP_serveur,
                           sizeof (adUDP_serveur));

                    if (emis < 0) {
                        perror ("ERREUR-recvfrom ");
                    }
                    else {
                        state= 3;
                    }
                }

                break;

            default:
                break;
        }
    }

    /* fermeture du point d'accès */
    close (point_acces_client);

    exit (0);
}
