#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h> 
#include <limits.h>

#define infinito INT_MAX/2
#define maximoMatriz 5
#define maximoArestas 5
#define true 1
#define false 0

int qntVertices;
int qntArestas;
int grafo[maximoMatriz][maximoMatriz];
int numberArray[maximoArestas];

void printLista(int lista[], int tam){
    int i;
    for(i = 0; i < tam; i++)
        printf("%d ", lista[i]);
    printf("\n");
}

void printMatriz(){
    for(int i=0; i < qntVertices; i++){
        for(int j=0; j < qntVertices; j++){
            printf("%d ", grafo[i][j]);  
        } 
        printf("\n");  
    }         
}

// ------ Funçoes de leitura de entrada -----
void inicializaMatriz(){
    for(int i = 0; i < qntVertices; i++)
        for(int j = 0; j < qntVertices; j++)
           grafo[i][j] = infinito; 
}

int setMatrizAdj(int hasPeso){    
    inicializaMatriz();

    if(hasPeso){
        // 1,5,5 se hasPeso = true, entao definir [1][5] = 5
         
        for (int x=0;x<(qntArestas*3);x+=3){
            int valor1 = numberArray[x];
            int valor2 = numberArray[x+1];
            int valor3 = numberArray[x+2];

            grafo[valor1][valor2] = valor3;
            grafo[valor2][valor1] = valor3;
        }
    }
    else{
        // 1,5,5 se hasPeso = false, entao [1][5] = 1, o prox 5 é do outro
        for(int x=0;x<(qntArestas*2);x+=2){
           
            int valor1 = numberArray[x];
            int valor2 = numberArray[x+1];
            
            grafo[valor1][valor2] = 1;
            grafo[valor2][valor1] = 1; 
        }
    }    
}

void lerArquivos(char nomeArquivo[]){
    FILE *arq;
    
    int arrayVerticeAresta[60];
    arq = fopen(nomeArquivo, "rt");

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

void escreverArquivoResultado(int distancias[], char nomeArquivo[]){
    FILE *arq;

    arq = fopen(nomeArquivo, "w");

    int i;

    for (i = 0; i < qntVertices; i++)
        fprintf(arq, "%d:%d ", i, distancias[i]);           
    
    fclose(arq);
}
// ------ fim das funçoes de leitura de entrada -----

//------ funçoes de djikstra -----
int *inicializaVetor(int tamanho, int valor){
    int i;
    int *vetor =  malloc (sizeof(int) * tamanho);

    for (i = 0; i < tamanho; i++)
        vetor[i] = valor;
        
    return vetor;
}

void printTodosOsVertices(int vetor[], int tamanho){
    int i;

    for (i = 0; i < tamanho; i++)
        printf("%d:%d ", i ,vetor[i]);           
    printf("\n");
    
    return;
}

void printUnicoVertice(int vetor[], int vertice){
    printf("%d\n", vetor[vertice]);           
}

//retorna o vetor de distancias
int *dijkstra(int vertice_inicial){
    int i, j, contador, vertice_atual, menor_distancia;

    int *distancia = inicializaVetor(qntVertices, infinito);
    int *aberto = inicializaVetor(qntVertices, true);

   //altera o vetor distancia com os pesos das arestas de vertice inicial ao resto dos vertices
    for (j = 0; j < qntVertices; j++){
        if (j == vertice_inicial){
            distancia[j] = 0;
        }
        if(grafo[vertice_inicial][j] != infinito){
            distancia[j] = grafo[vertice_inicial][j];
        }
    }

    aberto[vertice_inicial] = false;

    //loop principal
    for(contador = 1; contador < (qntVertices - 1); contador++){
        menor_distancia = infinito;

        for(i = 0; i < qntVertices; i++){
            if(aberto[i] && (distancia[i] < menor_distancia)){
                menor_distancia = distancia[i];
                vertice_atual = i;
            }
        }

        aberto[vertice_atual] = false;

        for(i = 0; i < qntVertices; i++){
            if((aberto[i]) && ((grafo[vertice_atual][i] + menor_distancia) < distancia[i])){                
                distancia[i] = grafo[vertice_atual][i] + menor_distancia;
            }
        }
    }

    return distancia;
}
//------ fim das funçoes de djikstra -----

int main(int argc, char **argv){
    int opt;
    int *distancias;
    int numInicial = -1;
    int numFinal = -1; 
    
    while( (opt = getopt(argc, argv, "ho:f:si:l:")) > 0 ) {
        switch ( opt ) {
            case 'h':
                printf("-h Guia de ajuda do algoritmo de Djikstra\n");
                printf("-o <arquivo> : redireciona a saida para o arquivo\n");
                printf("-f <arquivo> : indica o arquivo que contém o grafo de entrada\n");
                printf("-s : mostra a solução (em ordem crescente)\n");
                printf("-i : vértice inicial (dependendo do algoritmo)\n");
                printf("-l : vértice final (dependendo do algoritmo)\n");
                break;
            case 'o': ;
                char *nomeArquivoSaida = optarg;
                escreverArquivoResultado(distancias, nomeArquivoSaida);
                break ;
            case 'f': ;
                char *nomeArquivoEntrada = optarg;
                lerArquivos(nomeArquivoEntrada);
                break ;
            case 's': ;
                printTodosOsVertices(distancias, qntVertices);
                break ;
            case 'i': ;                
                numInicial = atoi(optarg);
                distancias = dijkstra(numInicial);
                break ;
            case 'l': ;
                numFinal = atoi(optarg);
                printUnicoVertice(distancias, numFinal);
                break ;            
            default:
                printf("-h para help");
        }
    }

    return 0;
}