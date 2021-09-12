#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#define ANCHO 30
#define LARGO 12


void Parametros(int *,int *, int *);
void GenerarPoblacion(int [ANCHO][LARGO]);
void EvaluarPoblacion(int [ANCHO][LARGO]);
void ImprimirPoblacion(int [ANCHO][LARGO]);
void Elitismo(int [ANCHO][LARGO],int [ANCHO][LARGO]);
int Cruza(int [ANCHO][LARGO],int [ANCHO][LARGO],int,int,int,int);
void Mutacion(int [ANCHO][LARGO],int,int);
int Ruleta(int [ANCHO][LARGO]);

int main(){
	srand(time(NULL));
	
	int cont;
	int padre1,padre2;
	int generaciones;
	int p_Cruza;
	int p_Mutacion;
	int exito;
	int poblacion[ANCHO][LARGO];
	int poblacion2[ANCHO][LARGO];
	
	for(int i=0;i<ANCHO;i++){
		for(int j=0;j<LARGO;j++){
			poblacion[i][j]=0;
			poblacion2[i][j]=0;
		}
	}
	
	Parametros(&generaciones,&p_Cruza,&p_Mutacion);
	GenerarPoblacion(poblacion);
	EvaluarPoblacion(poblacion);
	printf(" \n \n Generacion 1 \n");
	cont=1;
	ImprimirPoblacion(poblacion);
	printf(" \n \n");
	do{
		cont++;
		
		Elitismo(poblacion,poblacion2);
		for(int j=2;j<30;j+=2){
			padre1=Ruleta(poblacion);
			padre2=Ruleta(poblacion);
			exito=Cruza(poblacion,poblacion2,p_Cruza,padre1,padre2,j);
			if(exito==1){
				Mutacion(poblacion2,p_Mutacion,j);
				Mutacion(poblacion2,p_Mutacion,j+1);
				j+=2;
			}
			j-=2;
		}
		EvaluarPoblacion(poblacion2);
		for(int i=0;i<ANCHO;i++){
			for(int j=0;j<LARGO;j++){
				poblacion[i][j]=poblacion2[i][j];
				poblacion2[i][j]=0;
			}
		}
		printf(" \n \n Generacion %d \n",cont);
		ImprimirPoblacion(poblacion);
		generaciones--;
		printf(" \n \n");
	}while(generaciones>1);
	
}

void Parametros(int *generaciones,int *p_Cruza,int *p_Mutacion){
	do{
		printf("Numero de generaciones: ");
		scanf("%d",generaciones );
	}while(*generaciones<0);
	do{
		printf("Probabilidad de cruza: ");
		scanf("%d", p_Cruza);		
	}while(*p_Cruza<0||*p_Cruza>100);
	do{
		printf("Probabilidad de mutacion: ");
		scanf("%d",p_Mutacion );		
	}while(*p_Mutacion<0||*p_Mutacion>100);

	
}

void GenerarPoblacion(int poblacion[ANCHO][LARGO]){
	
	for(int i=0;i<ANCHO;i++){
		for(int j=0;j<LARGO-2;j++){
			poblacion[i][j]=rand()%2;
		}
	}
}

void EvaluarPoblacion(int poblacion[ANCHO][LARGO]){
	for(int i=0;i<ANCHO;i++){
		for(int j=0;j<LARGO-2;j++){
			if(poblacion[i][j]==1){
				poblacion[i][10]+=1;
			}
		}
		if(i==0){
			poblacion[i][11]=poblacion[i][10];
		}
		else{
			poblacion[i][11]=poblacion[i-1][11]+poblacion[i][10];
		}
	}
}

void ImprimirPoblacion(int poblacion[ANCHO][LARGO]){
	for(int i=0;i<ANCHO;i++){
		printf("\n");
		for(int j=0;j<LARGO;j++){
			printf("%d   ",poblacion[i][j]);
		}
	}
}

void Elitismo(int poblacion[ANCHO][LARGO],int poblacion2[ANCHO][LARGO]){
	int mejor1=0,mejor2=0;
	int index1=0,index2=0;
	for(int i=0;i<ANCHO;i++){
		if(poblacion[i][10]>mejor1){
			mejor2=mejor1;
			mejor1=poblacion[i][10];
			index2=index1;
			index1=i;
		}
		else{
			if(poblacion[i][10]>mejor2){
				mejor2=poblacion[i][10];
				index2=i;
			}
		}
	}
	for(int i=0;i<10;i++){
		poblacion2[0][i]=poblacion[index1][i];
		poblacion2[1][i]=poblacion[index2][i];
	}
	
}

int Cruza(int poblacion[ANCHO][LARGO],	int poblacion2[ANCHO][LARGO],int p_Cruza,int padre1,int padre2,int posicion){
	int exito=rand()%101;
	if(exito<=p_Cruza){
		int particion=1+rand()&9;
		for(int i=0;i<particion;i++){
			poblacion2[posicion][i]=poblacion[padre1][i];
			poblacion2[posicion+1][i]=poblacion[padre2][i];
		}
		for(int i=particion;i<10;i++){
			poblacion2[posicion][i]=poblacion[padre2][i];
			poblacion2[posicion+1][i]=poblacion[padre1][i];
		}
		return 1;
	}
	return 0;
	
}

void Mutacion(int poblacion2[ANCHO][LARGO],int p_Mutacion,int posicion){
	int exito=rand()%101;
	if(exito<=p_Mutacion){
		int muta=rand()%10;
		if(poblacion2[posicion][muta]==0){
			poblacion2[posicion][muta]=1;
		}
		else{
			poblacion2[posicion][muta]=0;
		}
	}
	
}

int Ruleta(int poblacion[ANCHO][LARGO]){
	int padre=rand()%(poblacion[ANCHO-1][LARGO-1]+1);
	for(int i=0;i<ANCHO;i++){
		if(poblacion[i][11]>=padre){
			padre=i;
			i=ANCHO;
		}
	}
	
	return padre;
}
