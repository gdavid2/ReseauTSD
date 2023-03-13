#include <stdio.h>

#include "save.h"
#include "prin.h"
#include "jeu.h"

int main(){
    int li,co;
    char comd[4];

    prin(morpion);

    morpion[2][0]=1;

    prin(morpion);

    lecture_message("4501", li, co, comd);
    printf("ligne %d\n", li);
    printf("colonne %d\n", co);
    printf("comd %d%d%d\n", comd[0], comd[1], comd[2]);

    return 0;
}
