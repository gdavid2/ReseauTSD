#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED

void lecture_message(char message[100], int* ligne, int* colonne, char commande[4]){
    char cmd[4];
    int l,c;

    cmd[0]= message[0];
    cmd[1]= message[1];
    cmd[2]= message[2];

    l= (int)message[3] - 48;
    c= (int)message[4] - 48;

    strcpy(commande, cmd);
    ligne=l;
    colonne=c;
}

#endif // JEU_H_INCLUDED
