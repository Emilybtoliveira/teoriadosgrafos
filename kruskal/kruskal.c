// Kruskal's algorithm - minimum spanning tree
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h> 

typedef struct{
  int origem, destino, peso;
} Aresta;

typedef struct {
  Aresta lista[50];
  int n_vertices;
  int m_arestas;
} Adjacencias;


#define maximoArestas 40

int qntVertices;
int qntArestas;
int numberArray[maximoArestas];

// ------ função de debug -----
void printLista(int lista[], int tam){
    int i;
    for(i = 0; i < tam; i++)
        printf("%d ", lista[i]);
    printf("\n");
}
// ------ fim da função de debug -----

// ------ funçoes de leitura de entrada -----
Adjacencias setListaAdj(int hasPeso){
    Adjacencias adj;  
    int i = 0;  

    adj.n_vertices = qntVertices;
    adj.m_arestas = qntArestas;

    if(hasPeso){
        // 1,5,5 se hasPeso = true, entao definir [1][5] = 5
         
        for (int x=0;x<(qntArestas*3);x+=3){
            int valor1 = numberArray[x];
            int valor2 = numberArray[x+1];
            int valor3 = numberArray[x+2];

            adj.lista[i].origem = valor1;     
            adj.lista[i].destino = valor2;     
            adj.lista[i].peso = valor3; 

            i += 1; 
            /* matrizAdjNova[valor1][valor2] = valor3;
            matrizAdjNova[valor2][valor1] = valor3; */
        }
    }
    else{
        // 1,5,5 se hasPeso = false, entao [1][5] = 1, o prox 5 é do outro
        for(int x=0;x<(qntArestas*2);x+=2){
           
            int valor1 = numberArray[x];
            int valor2 = numberArray[x+1];

            adj.lista[i].origem = valor1;     
            adj.lista[i].destino = valor2;     
            adj.lista[i].peso = 1; 

            i += 1;
           /*  matrizAdjNova[valor1][valor2] = 1; 
            matrizAdjNova[valor2][valor1] = 1; */
        }
    }  

    return adj;  
}

Adjacencias lerArquivos(char nomeArquivo[]){
    FILE *arq;
    
    int arrayVerticeAresta[maximoArestas];
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
    
    Adjacencias adj = setListaAdj(hasPeso);

    fclose(arq);

    return adj;
}

void escreverArquivoResultado(Adjacencias adj, char nomeArquivo[]){
    FILE *arq;

    arq = fopen(nomeArquivo, "w");

    int i;
    fprintf(arq, "AGM\n(u,v) w\n\n");   
    for (i=0; i<adj.m_arestas; i++){
        fprintf(arq, "(%d,%d) %d\n", adj.lista[i].origem, adj.lista[i].destino, adj.lista[i].peso);        
    }
    fclose(arq);
}
// ------ fim das funçoes de leitura de entrada -----

// ------ funções de Kruskal -----
void print(Adjacencias adj){
    int i;
    
    printf("AGM\n(u,v) w\n");

    for (i = 0; i < adj.m_arestas; i++)
        printf("(%d,%d) %d\n", adj.lista[i].origem, adj.lista[i].destino, adj.lista[i].peso);
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
// ------ fim das funções de Kruskal -----

int main(int argc, char **argv){
    Adjacencias adj; 
    int opt;
    int numInicial = -1;
    int numFinal = -1;     
   
    while( (opt = getopt(argc, argv, "ho:f:si:l:")) > 0 ) {
        switch ( opt ) {
            case 'h': ;
                printf("-h : Guia de ajuda do algoritmo de Kruskal\n");
                printf("-o <arquivo> : redireciona a saida para o arquivo\n");
                printf("-f <arquivo> : indica o arquivo que contém o grafo de entrada\n");
                printf("-s : mostra a solução\n");
                break;
            case 'o': ;
                char *nomeArquivoSaida = optarg;
                adj = kruskal(adj);
                escreverArquivoResultado(adj, nomeArquivoSaida);   
                break ;
            case 'f': ;
                char *nomeArquivoEntrada = optarg;
                adj = lerArquivos(nomeArquivoEntrada);
                break ;
            case 's': ;
                adj = kruskal(adj);
                print(adj);
                break ;            
            default:
                printf("-h para help");
        }
    }
   
    return 0;
}