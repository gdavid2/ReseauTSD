#include <stdio.h>

#include "save.h"
#include "prin.h"

int main(){
    prin(morpion);

    morpion[2][0]=1;

    prin(morpion);

    return 0;
}
