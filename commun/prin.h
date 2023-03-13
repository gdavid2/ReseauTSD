#ifndef PRIN_H_INCLUDED
#define PRIN_H_INCLUDED

void prin(int m[3][3]){
    printf("================="); printf("\n");
    printf("#               #"); printf("\n");

    printf("#   %d | %d | %d   #", m[0][0], m[0][1], m[0][2]); printf("\n");
    printf("#  -----------  #"); printf("\n");
    printf("#   %d | %d | %d   #", m[1][0], m[1][1], m[1][2]); printf("\n");
    printf("#  -----------  #"); printf("\n");
    printf("#   %d | %d | %d   #", m[2][0], m[2][1], m[2][2]); printf("\n");

    printf("#               #"); printf("\n");
    printf("================="); printf("\n");
};

#endif // PRINT_H_INCLUDED
