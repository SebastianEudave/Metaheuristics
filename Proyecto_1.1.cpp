#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <string.h>
#include<math.h>
//PARAMETROS A CAMBIAR---------------------------------------------------------------------------------
#define TI 1200 //TEMPERATURA INICIAL
#define TF 0  //TEMPERATURA FINAL
#define ALPHA 0.85 //FACTOR DE ENFRIAMIENTO
#define A 200 //NUNMERO DE ACEPTACIONES POR GRADO
#define P 0.85 //FACTOR QUE DISMINUYE EL NUMERO DE ITERACIONES
//-------------------------------------------------------------------------------------------
#define TAM 101

void GenerarSolucionInicial(float [TAM][TAM], int [TAM],int);
void GenerarSegundaSolucion(int [TAM],int);
float EvaluarSolucion(int [TAM], float[TAM][TAM],int);
void CambiarSolucion(int [TAM], int [TAM],int);

int main(){
	srand(time(NULL));
	FILE *bays29;
	float matriz[TAM][TAM];
	int solucion[TAM];
	int solucion2[TAM];
	int datos[TAM][3];
	int a = 0;
	int k = 0;
	float R = 0;
	//CAMBIAR PARAMETRO K (NUMERO DE ITERACIONES MAXIMAS EN UN GRADO)--------------------------------------
	int K = 500;
	//--------------------------------------------------------------------------------------
	int t = TI;
	float evaluacion = 0;
	float evaluacion2 = 0;
	char buffer[1000];
	char inicial = 0;
	int x,x2;
	int y,y2;
	int indice,indice2;
	int tam;
	
	do{
		fflush( stdin );
		printf("\nPresione 1 y enter para eil101 \n2 y enter para bays29 \n");
		scanf("%c",&inicial);
	}while(inicial!='1'&&inicial!='2');
	//EIL101---------------------------------------------------------------------------------------------
	if(inicial=='1'){
		tam = 101;
		bays29=fopen("eil101.txt","r");
		if (bays29==NULL) {
			exit (1);
		}
		do{
			fgets(buffer,1000,bays29);
		}while(strcmp(buffer,"NODE_COORD_SECTION\n")!=0);
		for(int i=0;i<TAM;i++){
			solucion[i] = 0;
			for(int j=0;j<3;j++){
				fscanf(bays29,"%d",&datos[i][j]);
			}
		}
		for(int i=0;i<TAM;i++){
			for(int j=0;j<TAM;j++){
				matriz[i][j]=sqrt(pow(datos[i][1]-datos[j][1],2.0)+pow(datos[i][2]-datos[j][2],2.0));
			}
		}
		
	}
	
	
	
	//BAYS29------------------------------------------------------------------------------------------------------
	else{
		tam = 29;
		bays29=fopen("bays29.txt","r");
		if (bays29==NULL) {
			exit (1);
		}
		do{
			fgets(buffer,1000,bays29);
		}while(strcmp(buffer,"EDGE_WEIGHT_SECTION\n")!=0);
		for(int i=0;i<tam;i++){
			solucion[i] = 0;
			for(int j=0;j<tam;j++){
				fscanf(bays29,"%f",&matriz[i][j]);
			}
		}
	}
	fclose ( bays29 );
	//-------------------------------------------------------------------------------------------------
	printf("\nMatriz de distancias: ");
		for(int i=0;i<tam;i++){
			printf("\n");
			for(int j=0;j<tam;j++){
				printf("%.2f  ", matriz[i][j]);
			}
		}
	
	do{
		fflush(stdin);
		printf("\n Dame la posicion inicial: ");
		scanf("%d",&x);
	}while(x<0||x>tam-1);
	solucion[0]=x;
	
	GenerarSolucionInicial(matriz,solucion,tam);
	printf("\n");
	for(int j=0;j<tam;j++){
		printf("%d  ", solucion[j]);
	}
	
	
	
	//Inicio Recocido Simulado
	do{
		do{
			CambiarSolucion(solucion, solucion2,tam);
			GenerarSegundaSolucion(solucion2,tam);
			evaluacion = EvaluarSolucion(solucion, matriz,tam);
			evaluacion2 = EvaluarSolucion(solucion2, matriz,tam);
			if(evaluacion2 < evaluacion){
				a++;
				CambiarSolucion(solucion2, solucion,tam);
			}
			else{
				R = rand()%1000/1000;
				if(R > exp((evaluacion - evaluacion2) / t)){
					a++;
					CambiarSolucion(solucion2, solucion,tam);
				}
			}
			k++;
		
		printf("\n");
		for(int j=0;j<tam;j++){
			printf("%d  ", solucion[j]);
		}
		printf("%d  ", solucion[0]);
		}while(a < A && k < K);
		t = ALPHA * t;
	    K = K*P;
		a = 0;
		k = 0;
	}while(t > TF);
	printf("\n Solucion final: \n");
	for(int j=0;j<tam;j++){
		printf("%d  ", solucion[j]);
	}
	printf("%d  ", solucion[0]);
	printf("\n");

	evaluacion = EvaluarSolucion(solucion, matriz,tam);
	printf("Evaluacion de la solucion: \n");
	printf("%f",evaluacion);
}


void GenerarSolucionInicial(float matriz[TAM][TAM],int solucion[TAM],int tam){
	float menor = 0;
	int indice = 0;
	int indice2 = 0;
	bool esParte = false;
	for(int i = 1; i < tam; i++){
		indice = solucion[i - 1];
		
		menor = 1000;
		for(int j = 0; j < tam; j++){
			for(int m = i - 1; m > -1;m--){
				if(solucion[m] == j){
					esParte = true;
					m = 0;
				}
			}
			if(matriz[indice][j] < menor && esParte == false){
				menor = matriz[indice][j];
				indice2 = j;
			}
			esParte = false;
		}
		solucion[i] = indice2;
	}
}

void GenerarSegundaSolucion(int solucion[TAM],int tam){
	int temporal;
	int indice1 = 1 + rand()%(tam - 1);
	int indice2;
	do{
		indice2 = 1 + rand()%(tam - 1);
	}while(indice2 == indice1);
	temporal = solucion[indice1];
	solucion[indice1] = solucion[indice2];
	solucion[indice2] = temporal;
}

float EvaluarSolucion(int solucion[TAM], float matriz[TAM][TAM],int tam){
	float suma = 0;
	int indice1 = 0;
	int indice2 = 0;
	for(int i = 1; i < tam; i++){
		indice1 = solucion[i];
		indice2 = solucion[i - 1];
		suma += matriz[indice1][indice2];
	}
	suma += matriz[indice1][solucion[0]];
	return suma;
}


void CambiarSolucion(int solucion[TAM], int solucion2[TAM],int tam){
	for(int i = 0; i < tam; i++){
		solucion2[i]=solucion[i];
	}
}
