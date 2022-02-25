#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h> 


#define maximoMatriz 100

int qntVertices;
int qntArestas;
int matrizAdjNova[maximoMatriz][maximoMatriz];
int numberArray[maximoMatriz];

int setMatrizAdj(int hasPeso){
    
    if(hasPeso){
        // 1,5,5 se hasPeso = true, entao definir [1][5] = 5
         
        for (int x=0;x<((qntArestas*3));x+=3){
            int valor1 = numberArray[x];
            int valor2 = numberArray[x+1];
            int valor3 = numberArray[x+2];
            matrizAdjNova[valor1-1][valor2-1] = valor3;
        }
    }
    else{
        // 1,5,5 se hasPeso = false, entao [1][5] = 1, o prox 5 é do outro
        for(int x=0;x<qntArestas*2;x+=2){
           
            int valor1 = numberArray[x];
            int valor2 = numberArray[x+1];
            
            matrizAdjNova[valor1-1][valor2-1] = 1; 
        }
    }

    
}


int lerArquivos(char *nomeArquivoEntrada){
    FILE *arq;

    
    int arrayVerticeAresta[60];
    arq = fopen(nomeArquivoEntrada, "rt");

    for (int i=0;i<2;i++){
        fscanf(arq, "%d ", &arrayVerticeAresta[i]);

    }
    qntVertices = arrayVerticeAresta[0];
    qntArestas = arrayVerticeAresta[1];

    for (int i=0;i<(qntArestas*3);i++){
        fscanf(arq, "%d ", &numberArray[i]);
        
    }
    
    int hasPeso = numberArray[(qntArestas*3)-1]!=0; // se for 1 = true; 0 = false
    // se os ultimos forem numeros muito grandes, entao quer dizer que nao tem peso. 
    // usar sempre *3 para indicar que pode ter peso.

    setMatrizAdj(hasPeso);

  fclose(arq);
}


void printarMatrizNova(){

    for (int i = 0; i < qntVertices; i++) {
        for (int j = 0; j < qntVertices; j++) {
            printf("%d ", matrizAdjNova[i][j]);
        }
        printf("\n");
    }
}

void printarSolucaoToda(){
    for (int i=0;i<qntVertices;i++)
	  for (int j=0;j<qntVertices;j++) {
        printf("%d,%d=%d\n",i+1,j+1,matrizAdjNova[i][j]);
	}
}

void printarNoArquivo(char *nomeArquivoSaida, int solCompleta){
    if(solCompleta){
         FILE *arq;

        arq = fopen(nomeArquivoSaida, "w");
        for (int i=0;i<qntVertices;i++)
            for (int j=0;j<qntVertices;j++) {
                fprintf(arq,"%d,%d=%d\n",i+1,j+1,matrizAdjNova[i][j]);
            }
        fclose(arq);
        
    }else{
        FILE *arq;

        arq = fopen(nomeArquivoSaida, "w");

        for (int i = 0; i < qntVertices; i++) {
            for (int j = 0; j < qntVertices; j++) {
                fprintf(arq,"%d ",matrizAdjNova[i][j]);
            }
            fprintf(arq,"\n");
        }
        
        fclose(arq);
    }
}

void floydWarshall(int solCompleta,int temArquivoSaida, char *nomeArquivoSaida, char *nomeArquivoEntrada) {

    lerArquivos(nomeArquivoEntrada);
    for(int x = 0; x < qntVertices; x++) {
        for(int i = 0; i < qntVertices; i++) {
            for (int j = 0; j < qntVertices; j++) {
                int soma = matrizAdjNova[i][x] + matrizAdjNova[x][j];
                if(matrizAdjNova[i][j] > soma){
                    matrizAdjNova[i][j] = soma;
                }
            }
        }
    }
   

    if(temArquivoSaida==0){
        
        if(solCompleta){
            printarSolucaoToda();
        }
        else{
            printarMatrizNova();
        }

    }else{
        printarNoArquivo(nomeArquivoSaida,solCompleta);
    }
    
  
  
}


int main(int argc, char **argv) {
    int opt;
    int mostrarSolucao = 0;
    int numInicial = -1;
    int numFinal = -1;
    char *nomeArquivoSaida;
    char *nomeArquivoEntrada;
    int temArquivoSaida = 0;
    while( (opt = getopt(argc, argv, "ho:f:si:l:")) > 0 ) {
        switch ( opt ) {
            case 'h':
                printf("-h Guia de ajuda do algoritmo de BellmanFord\n");
                printf("-o <arquivo> : redireciona a saida para o arquivo\n");
                printf("-f <arquivo> : indica o arquivo que contém o grafo de entrada\n");
                printf("-s : mostra a solução (em ordem crescente)\n");
                printf("-i : vértice inicial (dependendo do algoritmo)\n");
                printf("-l : vértice final (dependendo do algoritmo)\n");
                break;
            case 'o':
                nomeArquivoSaida = optarg;
                temArquivoSaida = 1;
                break ;
            case 'f':
                nomeArquivoEntrada = optarg;
                break ;
            case 's': 
                mostrarSolucao = 1;
                break ;
            case 'i':
                
                numInicial = atoi(optarg)-1;
                break ;
            case 'l':
                numFinal = atoi(optarg);
                break ;
            
            default:
                printf("-h para help");
        }
    }

    
    floydWarshall(mostrarSolucao,temArquivoSaida,nomeArquivoSaida,nomeArquivoEntrada);
    

        
}

