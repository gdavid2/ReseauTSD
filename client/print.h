#ifndef PRINT_H_INCLUDED
#define PRINT_H_INCLUDED

void prin(int m[3][3]){
    printf("=============");
    printf("#           #");
    printf("# %d | %d | %d #", m[0][0], m[0][1], m[0][2]);
    printf("# %d | %d | %d #", m[1][0], m[1][1], m[1][2]);
    printf("# %d | %d | %d #", m[2][0], m[2][1], m[2][2]);
    printf("#           #");
    printf("=============");
};

#endif // PRINT_H_INCLUDED
