// bellmanford

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
        aresta[k][0] = numberArray[x]-1;
        aresta[k][1] = numberArray[x+1]-1;
        aresta[k][2] = numberArray[x+2];
        k++;
    }
    
}


int lerArquivos(){
    FILE *arq;

    
    int arrayVerticeAresta[60];
    arq = fopen("entradaford.txt", "rt");

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


int solucaoCompleta(){
    for(int i=0;i<qntVertices;i++){
        printf("%d:%d ",i+1,d[i]);
    }
}

int solucaoSimples(int vFinal){
    return d[vFinal-1];
}

int bellmanford(int vInicial, int vFinal, int solCompleta){

    
    for(int dst = 0; dst<maximoMatriz;dst++){
        d[dst] = 9999; // definidno como infinito 
    }

    if(solCompleta){
        d[0] = 0; // source

    }
    else{
        d[vInicial] = 0;
    }
    
    int u,v,pesoAresta;

    int temCicloNegativo = 1;

    
    for(int i=0;i<qntVertices-1;i++){
        for(int k=0;k<qntArestas;k++){
            // trocar isso aqui para o nome que o professor deu para eles
            u = aresta[k][0] , v = aresta[k][1], pesoAresta = aresta[k][2] ;
    
            if(d[u]+pesoAresta < d[v]){
                d[v] = d[u] + pesoAresta;
    
            }
        }
    }
    for(int k=0;k<qntArestas;k++){
        // trocar isso aqui pro nome q o professor deu
        u = aresta[k][0] , v = aresta[k][1], pesoAresta = aresta[k][2];
        if(d[u]+pesoAresta < d[v]){
            temCicloNegativo = 0;
        }
    }

    if(temCicloNegativo){
        if(solucaoCompleta){
            solucaoCompleta();
        }
        else{
            solucaoSimples(vFinal);
        }
    }
    else{
        printf("Tem ciclo negativo");
    }
    
}
int main(){
    lerArquivos();
    bellmanford(0,5,1);
}