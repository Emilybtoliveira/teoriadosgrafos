#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h> 

#define maximoMatriz 50
#define vInFim 3

int qntVertices;
int qntArestas;
int numberArray[maximoMatriz];
int aresta[maximoMatriz][vInFim];
int d[maximoMatriz];


int setMatrizAdj(int hasPeso){
    
    int k =0;
    for (int x=0;x<((qntArestas*3));x+=3){
        // aqui é uma array de arestas mesmo, nao é a matriz de adjacencia
        // so reusei o mesmo codigo do floyd
        aresta[k][0] = numberArray[x]-1;
        aresta[k][1] = numberArray[x+1]-1;
        aresta[k][2] = numberArray[x+2];
        k++;
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


void solucaoCompleta(){
    for(int i=0;i<qntVertices;i++){
        printf("%d:%d ",i+1,d[i]);
    }
}

void solucaoSimples(int vFinal){
    printf("%d\n", d[vFinal-1]);
}

void printarNoArquivo(char *nomeArquivoSaida, int solCompleta, int vFinal){
    if(solCompleta){
         FILE *arq;

        arq = fopen(nomeArquivoSaida, "w");
        for(int i=0;i<qntVertices;i++){
            fprintf(arq, "%d:%d ", i+1,d[i]);
        }
        fclose(arq);
        
    }else{
        FILE *arq;

        arq = fopen(nomeArquivoSaida, "w");
        fprintf(arq,"%d",d[vFinal-1]);
        fclose(arq);
    }
}

int bellmanford(int vInicial, int vFinal, int solCompleta,int temArquivoSaida, char *nomeArquivoSaida, char *nomeArquivoEntrada){

    lerArquivos(nomeArquivoEntrada);
    for(int dst = 0; dst<maximoMatriz;dst++){
        d[dst] = 9999; // definidno como infinito 
    }

    if(solCompleta){
        d[0] = 0; // nao vai especificar a origem

    }
    else{
        d[vInicial] = 0; // especifica a origem
    }
    
    

    int temCicloNegativo = 1;

    
    for(int i=0;i<qntVertices-1;i++){
        for(int k=0;k<qntArestas;k++){
            int u = aresta[k][0];
            int v = aresta[k][1];
            int pesoAresta = aresta[k][2];
            if(d[u]+pesoAresta < d[v]){
                d[v] = d[u] + pesoAresta;
            }
        }
    }
    for(int k=0;k<qntArestas;k++){
        int u = aresta[k][0];
        int v = aresta[k][1];
        int pesoAresta = aresta[k][2];
        if(d[u]+pesoAresta < d[v]){
            temCicloNegativo = 0;
        }
    }
    if(temArquivoSaida==0){
        if(temCicloNegativo){
            if(solCompleta){
                solucaoCompleta();
            }
            else{
                solucaoSimples(vFinal);
            }
        }
        else{
            printf("Tem ciclo negativo");
        }

    }else{
        printarNoArquivo(nomeArquivoSaida,solCompleta,vFinal);
    }
    
}
int main(int argc, char **argv) {
    int opt;
    int mostrarSolucao = 0;
    int numInicial = 0;
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
    if(numFinal==-1){
        printf("informar vertice final com -l x");
    }
    bellmanford(numInicial,numFinal,mostrarSolucao,temArquivoSaida,nomeArquivoSaida,nomeArquivoEntrada);

    
}