#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED

#define DEM_INIT 000
#define INIT_OK  001
#define DEM_CASE 010
#define CHX_CASE 011
#define DEM_WRTE 100
#define PRINT_OK 101
#define CASE_NOK 110

void lecture_message(char message[6], char ligne[2], char colonne[2], char commande[4]){
    char cmd[4];
    char l[2];
    char c[2];

    cmd[0]= message[0];
    cmd[1]= message[1];
    cmd[2]= message[2];

    l[0]= message[3];
    c[0]= message[4];

    strcpy(commande, cmd);
    strcpy(ligne, l);
    strcpy(colonne, c);
}

void viderBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

#endif // JEU_H_INCLUDED
