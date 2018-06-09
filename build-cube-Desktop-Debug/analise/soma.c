#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *et1, *et2, *et3, *et4, *et5, *et6, *et7;
	int i,aux;
	et1 = fopen("tempo_rot_otimo.txt", "r");
	// et2 = fopen("2quinas_q.txt", "r");
	// et3 = fopen("3segundacamada_q.txt", "r");
	// et4 = fopen("4crossdown_q.txt", "r");
	// et5 = fopen("5cantosdown_q.txt", "r");
	// et6 = fopen("6place_q.txt", "r");
	// et7 = fopen("7turn_q.txt", "r");
	// long int soma[10];
	// for(i = 0; i < 10; i++)
	// 	soma[i] = 0;
	for(i = 0; i < 10; i++)
	{
	 	aux = 0;
	 	fscanf(et1,"%d", &aux);
	// 	// printf("aux: %d",aux);
	// 	soma[i] += aux;
	// 	fscanf(et2,"%d", &aux);
	// 	soma[i] += aux;
	// 	fscanf(et3,"%d", &aux);
	// 	soma[i] += aux;
	// 	fscanf(et4,"%d", &aux);
	// 	soma[i] += aux;
	// 	fscanf(et5,"%d", &aux);
	// 	soma[i] += aux;
	// 	fscanf(et6,"%d", &aux);
	// 	soma[i] += aux;
	// 	fscanf(et7,"%d", &aux);
	// 	soma[i] += aux;
		// printf("%d\n",soma[i]);
		int tempo = aux;
		int horas = (tempo/1000)/3600;
		tempo = tempo - horas*1000*3600;
		int minutos = (tempo/1000)/60;
		tempo = tempo - minutos*1000*60;
		int segundos = (tempo/1000);
		tempo = tempo - segundos*1000;
		double ms = (double)tempo/1000;
		ms = segundos + ms;
		printf("%02d:%02d:%05.2f\n",horas,minutos,ms);
	}
}