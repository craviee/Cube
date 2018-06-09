#include <stdio.h>
#include <stdlib.h>

#define TAM 10

int main()
{
	FILE *et1, *et2, *et3, *et4, *et5, *et6, *et7;
	int aux1;
	et1 = fopen("1cross_q.txt", "r");
	et2 = fopen("2quinas_q.txt", "r");
	et3 = fopen("3segundacamada_q.txt", "r");
	et4 = fopen("4crossdown_q.txt", "r");
	et5 = fopen("5cantosdown_q.txt", "r");
	et6 = fopen("6place_q.txt", "r");
	et7 = fopen("7turn_q.txt", "r");
	int maior[7], media[7], menor[7];
	int maior2[7], media2[7], menor2[7];
	int etapa1[TAM], etapa2[2][TAM], etapa3[2][TAM], etapa4[2][TAM], etapa5[2][TAM], etapa6[2][TAM], etapa7[2][TAM];
	int i,j;

	for(i = 0; i < 7; i++)
	{
		maior[i] = 0;
		menor[i] = 999999;
		media[i] = 0;
		maior2[i] = 0;
		menor2[i] = 999999;
		media2[i] = 0;
	}

	for(i = 0; i < TAM; i++)
	{
		fscanf(et1,"%d", &etapa1[i]);
		fscanf(et2,"%d %d", &etapa2[0][i], &etapa2[1][i]);
		fscanf(et3,"%d %d", &etapa3[0][i], &etapa3[1][i]);
		fscanf(et4,"%d %d", &etapa4[0][i], &etapa4[1][i]);
		fscanf(et5,"%d %d", &etapa5[0][i], &etapa5[1][i]);
		fscanf(et6,"%d %d", &etapa6[0][i], &etapa6[1][i]);
		fscanf(et7,"%d %d", &etapa7[0][i], &etapa7[1][i]);

		media[0] += etapa1[i];
		media[1] += etapa2[0][i];
		media[2] += etapa3[0][i];
		media[3] += etapa4[0][i];
		media[4] += etapa5[0][i];
		media[5] += etapa6[0][i];
		media[6] += etapa7[0][i];

		media2[1] += etapa2[1][i];
		media2[2] += etapa3[1][i];
		media2[3] += etapa4[1][i];
		media2[4] += etapa5[1][i];
		media2[5] += etapa6[1][i];
		media2[6] += etapa7[1][i];

		if(etapa1[i] > maior[0])
			maior[0] = etapa1[i];
		if(etapa1[i] < menor[0])
			menor[0] = etapa1[i];

		if(etapa2[0][i] > maior[1])
			maior[1] = etapa2[0][i];
		if(etapa2[0][i] < menor[1])
			menor[1] = etapa2[0][i];

		if(etapa3[0][i] > maior[2])
			maior[2] = etapa3[0][i];
		if(etapa3[0][i] < menor[2])
			menor[2] = etapa3[0][i];
		
		if(etapa4[0][i] > maior[3])
			maior[3] = etapa4[0][i];
		if(etapa4[0][i] < menor[3])
			menor[3] = etapa4[0][i];
		
		if(etapa5[0][i] > maior[4])
			maior[4] = etapa5[0][i];
		if(etapa5[0][i] < menor[4])
			menor[4] = etapa5[0][i];
		
		if(etapa6[0][i] > maior[5])
			maior[5] = etapa6[0][i];
		if(etapa6[0][i] < menor[5])
			menor[5] = etapa6[0][i];
		
		if(etapa7[0][i] > maior[6])
			maior[6] = etapa7[0][i];
		if(etapa7[0][i] < menor[6])
			menor[6] = etapa7[0][i];

		if(etapa2[1][i] > maior2[1])
			maior2[1] = etapa2[1][i];
		if(etapa2[1][i] < menor2[1])
			menor2[1] = etapa2[1][i];

		if(etapa3[1][i] > maior2[2])
			maior2[2] = etapa3[1][i];
		if(etapa3[1][i] < menor2[2])
			menor2[2] = etapa3[1][i];
		
		if(etapa4[1][i] > maior2[3])
			maior2[3] = etapa4[1][i];
		if(etapa4[1][i] < menor2[3])
			menor2[3] = etapa4[1][i];
		
		if(etapa5[1][i] > maior2[4])
			maior2[4] = etapa5[1][i];
		if(etapa5[1][i] < menor2[4])
			menor2[4] = etapa5[1][i];
		
		if(etapa6[1][i] > maior2[5])
			maior2[5] = etapa6[1][i];
		if(etapa6[1][i] < menor2[5])
			menor2[5] = etapa6[1][i];
		
		if(etapa7[1][i] > maior2[6])
			maior2[6] = etapa7[1][i];
		if(etapa7[1][i] < menor2[6])
			menor2[6] = etapa7[1][i];
	}
	media[0] /= TAM;
	media[1] /= TAM;
	media[2] /= TAM;
	media[3] /= TAM;
	media[4] /= TAM;
	media[5] /= TAM;
	media[6] /= TAM;
	media2[1] /= TAM;
	media2[2] /= TAM;
	media2[3] /= TAM;
	media2[4] /= TAM;
	media2[5] /= TAM;
	media2[6] /= TAM;

	for(i = 0; i < 7; i++)
	{
		if(!i)
			printf("Etapa%d\tMaior = %d\t\t\tMenor = %d\t\t\tMédia = %d\n",i+1, maior[i], menor[i], media[i]);
		else
			printf("Etapa%d\tMaior = %d\tMaiorT = %d\tMenor = %d\tMenorT = %d\tMédia = %d\tMédiaT = %d\n",i+1, maior[i], maior2[i], menor[i], menor2[i], media[i], media2[i]);
	}
	return 0;
}