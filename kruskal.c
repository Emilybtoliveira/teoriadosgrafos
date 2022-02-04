// Kruskal's algorithm - minimum spanning tree
#include <stdio.h>
#include <stdlib.h>

typedef struct{
  int origem, destino, peso;
} Aresta;

typedef struct {
  Aresta lista[50];
  int n_vertices;
  int m_arestas;
} Adjacencias;

//função de debug
void printLista(int lista[], int tam){
    int i;
    for(i = 0; i < tam; i++)
        printf("%d ", lista[i]);
    printf("\n");
}

void print(Adjacencias adj){
    int i;
    
    printf("AGM\nu v w\n");

    for (i = 0; i < adj.m_arestas; i++)
        printf("%d %d %d\n", adj.lista[i].origem, adj.lista[i].destino, adj.lista[i].peso);
}

Adjacencias bubbleSort(Adjacencias adj){
    int i, j;
    Aresta aux;

    //printf("Bubblesort...\n");

    for(i = 0; i < adj.m_arestas; i++)
        for(j = 0; j < (adj.m_arestas - 1); j++){
            if(adj.lista[j].peso > adj.lista[j+1].peso){
                aux = adj.lista[j];
                adj.lista[j] = adj.lista[j+1];
                adj.lista[j+1] = aux;
            }
        }

    return adj;
}

//Union recebe o array com as componentes conexas a que cada vértice pertence, e une a componente do vértice de destino à componenete do vertice de origem
int *Union(int cluster[], int n, int origem, int destino){    
    int i;
    for(i=0; i<n; i++){
        if(cluster[i] == destino){
            cluster[i] = origem;
        }
    }
    return cluster;
}

int find(int cluster[], int v){
    return cluster[v];
}

Adjacencias kruskal(Adjacencias adj){
    int i, j = 0, origem, destino;
    int *cluster = malloc(sizeof(int) * adj.n_vertices);
    Adjacencias agm;
    agm.n_vertices = adj.n_vertices;
    agm.m_arestas = 0;

    //Passo 1: Ordenação das arestas em ordem crescente;
    adj = bubbleSort(adj);
    //print(adj); 

    //Passo 2: cada vértice é, inicialmente, uma arvore separada;
    for (i = 0; i < adj.n_vertices; i++)
        cluster[i] = i;

    //Passo 3: aplicação do Union-Find; 
    for (i=0; i < adj.m_arestas; i++){
        origem = find(cluster, adj.lista[i].origem);
        destino = find(cluster, adj.lista[i].destino);
        
        if( origem != destino){ //se origem e destino estiverem em componentes distintas, une-se a componente de destino à de origem.
            cluster = Union(cluster, adj.n_vertices, origem, destino);

            agm.lista[agm.m_arestas] = adj.lista[i]; //a aresta (origem,destino) é adicionada à AGM
            agm.m_arestas  += 1;
        }
    }

    //Passo 4: Retornar a AGM;
    return agm;
}


int main(){
    int n_vertices, m_arestas, i, u, v, w;
    Adjacencias adj;    

    scanf("%d%d", &n_vertices, &m_arestas);

    adj.n_vertices = n_vertices;
    adj.m_arestas = m_arestas;

    for(i = 0; i < m_arestas; i++){
        scanf("%d%d%d", &u, &v, &w);
        adj.lista[i].origem = u;     
        adj.lista[i].destino = v;     
        adj.lista[i].peso = w;    
    } 

    //print(adj); 
    adj = kruskal(adj);
    print(adj);
    //adj = bubbleSort(adj);
    //print(adj); 

    return 0;
}