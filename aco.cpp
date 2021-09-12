#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#define TAM 7

void ConstruirVisibilidad(int [TAM][TAM],float [TAM][TAM]);
void ConstruirFeromona(int [TAM][TAM],float [TAM][TAM], float);
void ImprimirMatriz(int [TAM][TAM]);
void ImprimirMatriz(float [TAM][TAM]);
void ElegirCamino(float [TAM], float [TAM][TAM], float [TAM][TAM],int [TAM], int, int);
void ActualizarTao(float, float, int, int [TAM], float[TAM][TAM], int[TAM][TAM]);



int main(){
	float tao = 0.1;
	int q = 1;
	int alpha = 1;
	int beta = 1;
	float p = 0.01;
	float posibilidad[TAM];
	int matrizD[TAM][TAM] = {{0,5,3,0,0,6,0},{5,0,4,0,0,0,5},{3,4,0,0,6,3,3},{0,0,0,0,5,0,4},{0,0,6,5,0,5,0},{6,0,0,0,5,0,0},{0,5,3,4,0,0,0}};
	int solucion[TAM];
	float visibilidad[TAM][TAM];
	float feromona[TAM][TAM];
	ConstruirVisibilidad(matrizD,visibilidad);
	ConstruirFeromona(matrizD,feromona,tao);
	printf("\nDistancias  \n ");
	ImprimirMatriz(matrizD);
	printf("\nVisibilidad  \n ");
	ImprimirMatriz(visibilidad);
	printf("\nFeromonas  \n ");
	ImprimirMatriz(feromona);
	solucion[0] = 0;
	ElegirCamino(posibilidad, visibilidad, feromona, solucion, alpha, beta);
	printf("\nSolucion: ");
	for(int j = 0; j < TAM; j++){
			printf("%d ",solucion[j]);
			if(solucion[j] == 3){
				j = TAM;
			}
		}
	printf("\n ");
	ActualizarTao(tao, p, q, solucion, feromona, matrizD);
	printf("Feromonas actualizadas \n ");
	ImprimirMatriz(feromona);
	
}

void ConstruirVisibilidad(int matrizD[TAM][TAM],float visibilidad[TAM][TAM]){
	for(int i = 0; i < TAM; i++){
		for(int j = i; j < TAM; j++){
			if(matrizD[i][j] != 0){
				visibilidad[i][j] = 1.0 / matrizD[i][j];
				visibilidad[j][i] = 1.0 / matrizD[i][j];
			}
			else{
				visibilidad[i][j] = 0;
				visibilidad[j][i] = 0;
			}
		}
	}
}

void ConstruirFeromona(int matrizD[TAM][TAM],float feromona[TAM][TAM], float tao){
	for(int i = 0; i < TAM; i++){
		for(int j = i; j < TAM; j++){
			if(matrizD[i][j] != 0){
				feromona[i][j] = tao;
				feromona[j][i] = tao;
			}
			else{
				feromona[i][j] = 0;
				feromona[j][i] = 0;
			}
		}
	}
}

void ImprimirMatriz(int matriz[TAM][TAM]){
	printf("\n");
	for(int i = 0; i < TAM; i++){
		
		for(int j = 0; j < TAM; j++){
			printf("%d",matriz[i][j]);
		}
		printf("\n");
	}
}

void ImprimirMatriz(float matriz[TAM][TAM]){
	printf("\n");
	for(int i = 0; i < TAM; i++){
		
		for(int j = 0; j < TAM; j++){
			printf("%f",matriz[i][j]);
		}
		printf("\n");
	}
}

void ElegirCamino(float posibilidad[TAM], float visibilidad[TAM][TAM], float feromona[TAM][TAM],int solucion[TAM], int alpha, int beta){
	float sumatoria;
	float aceptacion;
	bool si = false;
	srand(time(NULL));
	for(int i = 0; i < TAM; i++){
		
		sumatoria=0.0;
		for(int j = 0; j < TAM; j++){
			si = false;
			if(visibilidad[solucion[i]][j] > 0){
				for(int m = i; m > -1; m--){
					if(j == solucion[m]){
						posibilidad[j] = 0;
						si = true;
					}
				}
				if(si == false){
					sumatoria += pow(feromona[i][j], alpha)*pow(visibilidad[i][j], beta);
					posibilidad[j] = pow(feromona[i][j], alpha)*pow(visibilidad[i][j], beta);
				}
			}
			else{
				posibilidad[j] = 0;
			}
			if(j == TAM - 1 && sumatoria == 0){
				j = -1;
				i--;
			}
		}
		
		for(int j = 0; j < TAM; j++){
			if(posibilidad[j] > 0){
				posibilidad[j] /= sumatoria;
			}
		}
		sumatoria = 0.0;
		for(int j = 0; j < TAM; j++){
			if(posibilidad[j] > 0){
				sumatoria += posibilidad[j];
				posibilidad[j] = sumatoria;
			}
		}
		aceptacion = (rand()%1000) / 1000.0;
		for(int j = 0; j < TAM; j++){
			if(aceptacion < posibilidad[j]){
				solucion[i + 1] = j;
				
				j = TAM;
			}
		}
		if(solucion[i + 1] == 3){
			i = TAM;
		}
	}
}

void ActualizarTao(float tao, float p, int q, int solucion[TAM], float feromona[TAM][TAM], int matriz[TAM][TAM]){
	float l = 0;
	for(int i = 0; i < TAM; i++){
		l += matriz[solucion[i]][solucion[i + 1]];
		if(solucion[i + 1] == 3){
			i = TAM;
		}
	}
	for(int i = 0; i < TAM; i++){
		for(int j = 0; j < TAM; j++){
			feromona[i][j] = (1 - p) * feromona[i][j];
		}
	}	
	for(int i = 0; i < TAM; i++){
		feromona[solucion[i]][solucion[i + 1]] += (q / l);
		feromona[solucion[i+1]][solucion[i]] = feromona[solucion[i]][solucion[i + 1]];
		if(solucion[i + 1] == 3){
			i = TAM;
		}
	}
}

