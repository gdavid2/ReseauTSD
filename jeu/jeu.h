#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED

int morpion[3][3] ={0};

void init_morpion(){
	morpion[0][0] =0;
	morpion[0][1] =0;
	morpion[0][2] =0;
	morpion[1][0] =0;
	morpion[1][1] =0;
	morpion[1][2] =0;
	morpion[2][0] =0;
	morpion[2][1] =0;
	morpion[2][2] =0; 	
}

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

int verif_typ(char message[100]) {
	char msg[100];
	
	strcpy(msg, message);
	
	if (msg[0] != '0' &&
		msg[0] != '1' &&
		msg[0] != '2')
		return -1;
	else if (msg[1] != '0' &&
			 msg[1] != '1' &&
			 msg[1] != '2')
		return -1;
	else return 0;
}

int verif_case(char message[100]) {
	char msg[100];
	
	strcpy(msg, message);
	
	if (morpion[msg[0]-48][msg[1]-48] != 0) return -1;
	else return 0;
}

int verif_endg() {
	int i;
	
	for (i=0;i<3;i++) {
		if ((morpion[0][i] * morpion[1][i] * morpion[2][i]) == 1) return 4;
		else if ((morpion[0][i] * morpion[1][i] * morpion[2][i]) == 8) return 5;
	}
	
	i =0;
	for (i=0;i<3;i++) {
		if ((morpion[i][0] * morpion[i][1] * morpion[i][2]) == 1) return 4;
		else if ((morpion[i][0] * morpion[i][1] * morpion[i][2]) == 8) return 5;
	}
	
	if ((morpion[0][0] * morpion[1][1] * morpion[2][2]) == 1) return 4;
	else if ((morpion[0][0] * morpion[1][1] * morpion[2][2]) == 8) return 5;
	
	if ((morpion[0][2] * morpion[1][1] * morpion[2][0]) == 1) return 4;
	else if ((morpion[0][2] * morpion[1][1] * morpion[2][0]) == 8) return 5;
	
	return 2;
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
