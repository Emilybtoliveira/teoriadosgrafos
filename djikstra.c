#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define infinito INT_MAX/2
#define true 1
#define false 0

int *inicializaVetor(int tamanho, int valor){
    int i;
    int *vetor =  malloc (sizeof(int) * tamanho);

    for (i = 0; i < tamanho; i++)
        vetor[i] = valor;
        
    return vetor;
}

void printVetor(int vetor[], int tamanho){
    int i;

    for (i = 0; i < tamanho; i++)
        printf("%d:%d ", i ,vetor[i]);           
    printf("\n");
    
    return;
}

//retorna o vetor de distancias
int *dijkstra(int n_vertices, int grafo[][n_vertices], int vertice_inicial){
    int i, j, contador, vertice_atual, menor_distancia;

    int *distancia = inicializaVetor(n_vertices, infinito);
    int *aberto = inicializaVetor(n_vertices, true);

   //altera o vetor distancia com os pesos das arestas de vertice inicial ao resto dos vertices
    for (j = 0; j < n_vertices; j++){
        if (j == vertice_inicial){
            distancia[j] = 0;
        }
        if(grafo[vertice_inicial][j] != infinito){
            distancia[j] = grafo[vertice_inicial][j];
        }
    }

    aberto[vertice_inicial] = false;

    //loop principal
    for(contador = 1; contador < (n_vertices - 1); contador++){
        menor_distancia = infinito;

        for(i = 0; i < n_vertices; i++){
            if(aberto[i] && (distancia[i] < menor_distancia)){
                menor_distancia = distancia[i];
                vertice_atual = i;
            }
        }

        aberto[vertice_atual] = false;

        for(i = 0; i < n_vertices; i++){
            if((aberto[i]) && ((grafo[vertice_atual][i] + menor_distancia) < distancia[i])){
                distancia[i] = grafo[vertice_atual][i] + menor_distancia;
            }
        }
    }

    return distancia;
}

int main(int arg, char *args[]){

    int n_vertices, m_arestas, i, j, v1, v2, w;
    int vertice_inicial = atoi(args[1]);

    scanf("%d %d", &n_vertices, &m_arestas);

    int grafo[n_vertices][n_vertices];

    for(i = 0; i < n_vertices; i++)
        for(j = 0; j < n_vertices; j++)
           grafo[i][j] = infinito; 

    for(i = 0; i < m_arestas; i++){
        scanf("%d%d%d", &v1, &v2, &w);
            grafo[v1][v2] = w;
            grafo[v2][v1] = w;       
    }  

    int *distancias = dijkstra(n_vertices, grafo, vertice_inicial);

    printVetor(distancias, n_vertices);

    return 0;
}