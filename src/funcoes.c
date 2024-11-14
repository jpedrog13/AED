#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "funcoes.h"

typedef int Item;
typedef double Item2;
#define key(A) (A)
#define eq(A,B) (!less(A,B) && !less(B,A))
#define less(A, B) (key(A) < key(B))
#define exch(A, B) {Item t = A; A = B; B = t; }
#define exch2(A, B) {Item2 t = A; A = B; B = t; }

#define MAX 99999999999999999 /*valor infinito*/

struct graph{
	int V; /*número de vértices*/
	int A; /*número de arestas*/
	double **m; /*matriz adjacências*/
};

typedef struct rotas{
	int p1;
	int p2;
	double c;
}Rotas;

/***********************************************************************
* Função: inicializa
 
Recebe um inteiro com o número de vértices e cria uma matriz VxV com
valor infinito em todas as casas.
***********************************************************************/
Graph *inicializa(int V){
	int i, j;
	Graph *G;
	
	G = (Graph *) malloc(sizeof(Graph));
	
	G->V = V;
	G->A = 0;
	
	G->m = (double**)malloc(V * sizeof(double*));
	
	for(i=0; i<V; i++){
		G->m[i] = (double*)malloc(V * sizeof(double));
	}
	
	for(i=0; i<V; i++){
		for(j=0; j<V; j++){
			G->m[i][j] = MAX;
		}
	}
	
	return G;
}


/***********************************************************************
 * Função: insereA

Recebe o grafo contento a matriz de adjacências, 2 inteiros que 
representam a aresta a adicionar à matriz e 1 double com o valor do 
custo da aresta, que será introduzido na matriz.
***********************************************************************/
void insereA(Graph *G, int i, int j, double c){
	
	if(G->m[i-1][j-1]==MAX)
		G->A++;
	
	G->m[i-1][j-1]=c;
	G->m[j-1][i-1]=c;
}


/***********************************************************************
 * Função: backbone
 
Calcula o backbone utilizando o algoritmo de Prim. Recebe o grafo com a
matriz de adjacências, e duas tabelas, uma de inteiros (st) onde serão
guardadas as ligações do backbone, e uma de doubles (val) com o custo de
cada ligação.
***********************************************************************/
void backbone(Graph *G, int st[], double val[]){
	
	int fr[G->V], v, w, min, i;
	
	for(v=0; v<G->V; v++){
		st[v]=-1;
		fr[v]=v;
		val[v]=MAX;
	}
	
	val[G->V]=MAX;
	
	for(i=0; i<G->V; i++){ /*garante análise de vértices que não fazem 
						   parte da árvore do vértice 1 (pois este é o 
						   primeiro a ser analisado)*/
						   
		if(st[i]==i || st[i]==-1){ /*verifica se o vértice pertence à
								   árvore de um vértice anterior*/
								   
			min=i; st[i]=i;
			
			for(v=0; min!=G->V; st[min] = fr[min]){
				v=min;
				for(w=0, min=G->V; w<G->V; w++){
					if(st[w] == -1){
						if(G->m[v][w]<val[w]){
							val[w] = G->m[v][w];
							fr[w] = v;	
						}
						if(val[w]<val[min])
							min=w;				
					}
				}
			}		
		}
	}
}


/***********************************************************************
 * Função: caminho														
 
Testa se existe um caminho entre uma aresta, definida pelos vertices i e
j. Recebe tabela com a search tree e 2 inteiros com os vértices.
Retorna 1 se encontrou caminho, caso contrário retorna -1.
***********************************************************************/
int caminho(int st[], int i, int j){
	
	while (i != st[i]) 
		i = st[i];
	
	while (j != st[j])
		j = st[j];
	
	if (i != j)
		return 1;
	else
		return -1;
}


/***********************************************************************
 * Função: caminho2														
 
Testa quantos pares de vértices da tabela U formam um novo caminho. 
Recebe a tabela U, a tabela com a search tree, e um inteiro que define o
tamanho da tabela U.
Retorna o número de caminhos novos encontrados.
***********************************************************************/
int caminho2(Rotas *U, int st[], int l){
	int r=0, i, j, k;
	
	for(k=0; k<l; k++){
		
		i=U[k].p1;
		j=U[k].p2;
		
		while (i != st[i]) 
			i = st[i];
	
		while (j != st[j])
			j = st[j];
		
		if (i!=j){

			st[i]=j;
						
			r++;
		}
		else
			U[k].c=MAX;
	}
	return r;
}


/***********************************************************************
 * Função: quicksort
 
Organiza vetor de arestas por ordem crescente de custo
***********************************************************************/
void quicksort(Rotas *a, int l, int r){
	
	int i, j, k, p, q;
	double v;
	
	if (r <= l) return;
	
	v = a[r].c; i = l-1; j = r; p = l-1; q = r;
	
	for (;;) {
		while (less(a[++i].c, v));
		
		while (less(v, a[--j].c)) 
			if (j == l)
				break;
		
		if (i >= j)
			break;
		
		exch2(a[i].c, a[j].c);
		exch(a[i].p1, a[j].p1);
		exch(a[i].p2, a[j].p2);
		
		if (eq(a[i].c,v)) {
			p++;
			exch2(a[p].c,a[i].c);
			exch(a[p].p1,a[i].p1);
			exch(a[p].p2,a[i].p2);
		}
		if (eq(v,a[j].c)) {
			q--;
			exch2(a[q].c,a[j].c);
			exch(a[q].p1,a[j].p1);
			exch(a[q].p2,a[j].p2);
		}
	}
	exch2(a[i].c, a[r].c);
	exch(a[i].p1, a[r].p1);
	exch(a[i].p2, a[r].p2);
	
	j = i-1; 
	i = i+1;

	for (k = l ; k < p; k++, j--){
		exch2(a[k].c, a[j].c);
		exch(a[k].p1, a[j].p1);
		exch(a[k].p2, a[j].p2);
	}
	
	for (k = r-1; k > q; k--, i++){
		exch2(a[k].c, a[i].c);
		exch(a[k].p1, a[i].p1);
		exch(a[k].p2, a[i].p2);
	}
	
	quicksort(a, l, j); quicksort(a, i, r);
}

/***********************************************************************
 * Função: quicksort2
 
Organiza vetor de arestas por ordem crescente da posição p1
***********************************************************************/
void quicksort2(Rotas *a, int l, int r){
	
	int i, j, k, p, q;
	int v;
	
	if (r <= l) return;
	
	v = a[r].p1; i = l-1; j = r; p = l-1; q = r;
	
	for (;;) {
		while (less(a[++i].p1, v));
		
		while (less(v, a[--j].p1)) 
			if (j == l)
				break;
		
		if (i >= j)
			break;
		
		exch2(a[i].c, a[j].c);
		exch(a[i].p1, a[j].p1);
		exch(a[i].p2, a[j].p2);
		
		if (eq(a[i].p1,v)) {
			p++;
			exch2(a[p].c,a[i].c);
			exch(a[p].p1,a[i].p1);
			exch(a[p].p2,a[i].p2);
		}
		if (eq(v,a[j].p1)) {
			q--;
			exch2(a[q].c,a[j].c);
			exch(a[q].p1,a[j].p1);
			exch(a[q].p2,a[j].p2);
		}
	}
	exch2(a[i].c, a[r].c);
	exch(a[i].p1, a[r].p1);
	exch(a[i].p2, a[r].p2);
	
	j = i-1; 
	i = i+1;

	for (k = l ; k < p; k++, j--){
		exch2(a[k].c, a[j].c);
		exch(a[k].p1, a[j].p1);
		exch(a[k].p2, a[j].p2);
	}
	
	for (k = r-1; k > q; k--, i++){
		exch2(a[k].c, a[i].c);
		exch(a[k].p1, a[i].p1);
		exch(a[k].p2, a[i].p2);
	}
	
	quicksort2(a, l, j); quicksort2(a, i, r);
}


/***********************************************************************
 * Função: escreve
 
Escreve backbone no ficheiro de saída. Recebe o grafo contendo a matriz 
de adjacências, a tabela com os vertices a serem escritos, a tabela com
o valor dos vértices e um ponteiro para o ficheiro de saída.
***********************************************************************/
void escreve(Graph *G, FILE *f, int st[], double val[]){
	int i, j;
	
	for(i=0; i<G->V-1; i++){ /*garante que o ficheiro de saída é 
							 escrito de forma ordenada*/
							 
		for(j=i+1; j<G->V; j++){
			
			if(st[i]<i){ /*para o vértice da posição i, o valor do 
						 vértice j a que está ligado é menor que o seu.
						 Isto significa que o seu valor já foi escrito
						 quando foi analisada a posição do vértice j,
						 portanto só se analizam os vértices seguintes
						 escrevendo os que estejam ligados a i*/
						 
				if(st[j]==i && val[j]!=MAX && val[j]!=0)
					fprintf(f, "%d %d %.2lf\n", i+1, j+1, val[j]);

			}
			else{ /*para o vértice da posição i, o valor do vértice j a 
				  que está ligado é maior ou igual ao seu. Escreve os
				  vértices seguintes que estão ligados a i. Para o caso
				  em que o veŕtice j tem valor maior i, escreve o valor 
				  da sua ligação (val[i]) na posição de j*/
				  
				if(j==st[i] && val[i]!=MAX && val[i]!=0)
					fprintf(f, "%d %d %.2lf\n", i+1, j+1, val[i]);			
				if(st[j]==i && val[j]!=MAX && val[j]!=0)
					fprintf(f, "%d %d %.2lf\n", i+1, j+1, val[j]);
			}
		}
	}
}


/***********************************************************************
 * Função: A1

Resolve problemas do tipo A1. Recebe o grafo com a matriz de adjacências
e um ponteiro para o ficheiro de saída onde vai escrever a resposta.
***********************************************************************/
void A1(Graph *G, FILE *f){
	
	int st[G->V], a = 0, i;
	double c = 0, val[G->V+1];
	
	backbone(G, st, val); /*cria o backbone*/
	
	/*calcula o custo total*/
	for(i=0; i<G->V; i++){ 
		if(st[i]!=i && st[i]!=-1){ /*garante que a ligação existe*/
			c=c+val[i];
			a++; /*numero arestas backbone*/
		}
	}
	
	fprintf(f, "%d %.2lf\n", a, c);
	
	escreve(G, f, st, val);
	
	fprintf(f, "\n");
}


/***********************************************************************
 * Função: B1

Resolve problemas do tipo B1. Recebe o grafo com a matriz de adjacências
e um ponteiro para o ficheiro de saída onde vai escrever a resposta.
Recebe ainda dois inteiros que definem a aresta a ser retirada.
***********************************************************************/
void B1(Graph *G, FILE *f, int v1, int v2){
	
	int st[G->V], a=0, r=-1, d, i, j, k, v=0, cnt=0, caso=0;
	double c = 0, val[G->V+1];
	Rotas *U;
	
	backbone(G, st, val); /*cálculo backbone original*/
	
	/*calcula o custo total do backbone*/
	for(i=0; i<G->V; i++){ 
		if(st[i]!=i && st[i]!=-1){
			c=c+val[i];
			a++; /*numero de arestas do backbone original*/
		}
	}
	
	/*verifica se aresta pertence ao grafo*/
	if(v1>0 && v1<=G->V && v2>0 && v2<=G->V){
		if(G->m[v1-1][v2-1]==MAX || G->m[v1-1][v2-1]<=0){
			fprintf(f, "%d %.2lf %d\n", a, c, r);
			escreve(G, f, st, val);
			fprintf(f, "\n");
			return;
		}
	}
	else{
		fprintf(f, "%d %.2lf %d\n", a, c, r);
		escreve(G, f, st, val);
		fprintf(f, "\n");
		return;
	}
	r=0;
	
	/*verifica se aresta pertence ao backbone original, e se pertencer
	remove-a*/
	if(r==0){	
		if(st[v1-1]==v2-1){
			r=1;
			st[v1-1]=v1-1;
			caso = 1;
		}	
		if(st[v2-1]==v1-1){
			r=1;
			st[v2-1]=v2-1;
			caso = 2;
		}
	}
	if(caso==0){
		fprintf(f, "%d %.2lf %d\n", a, c, r);
		escreve(G, f, st, val);
		fprintf(f, "\n");
		return;
	}
	
	d = G->A - a; /*número de rotas que não pertencem ao backbone*/
	
	/*aloca memoria para a estrutura U que guarda todas as rotas
	alternativas*/		
	U = (Rotas *) malloc(d * sizeof(Rotas));
	
	/*remove aresta do grafo*/
	G->m[v1-1][v2-1] = MAX;
	G->m[v2-1][v1-1] = MAX;
	G->A--;
	
	/*descobre rota de menor custo que repoe ligação, se existir*/
	if(r==1){
		r=-1;
		
		/*adiciona a U as arestas que nao pertencem ao backbone*/
		for(i=0; i<G->V; i++){
			if(cnt>d)
				break;
			
			for(j=i+1; j<G->V; j++){
				if(G->m[i][j]!=MAX){	
					v=0;
					
					/*verifica se pertence ao backbone*/
					for(k=0; k<G->V; k++){						
						if((st[k]==i && k==j)||(st[k]==j && k==i)){
							v = 1;							
							break;
						}						
					}
					if(v!=1){
						U[cnt].c = G->m[i][j];
						U[cnt].p1 = i;
						U[cnt].p2 = j;

						cnt++; /*conta nº arestas em U*/						
					}
				}	
			}
		}
		
		/*organiza U*/ 
		quicksort(U, 0, d-1);
		
		for(i=0; i<d; i++){
			
			r = caminho(st, U[i].p1, U[i].p2);
			
			/*guarda aresta de menor custo*/
			if(r==1)
				break;
		}
	}
	fprintf(f, "%d %.2lf %d\n", a, c, r);
	
	/*insere aresta removida para escrever backbone original*/
	if(caso==1)
		st[v1-1]=v2-1;	
	else if(caso==2)
		st[v2-1]=v1-1;
	
	escreve(G, f, st, val);
	
	/*escreve aresta da rota alternativa*/
	if(r==1){
		fprintf(f, "%d %d %.2lf\n", U[i].p1+1, U[i].p2+1, U[i].c);
	}
	
	fprintf(f, "\n");
	
	/*liberta estrutura U*/
	free(U);
}


/***********************************************************************
 * Função: C1

Resolve problemas do tipo C1. Recebe o grafo com a matriz de adjacências
e um ponteiro para o ficheiro de saída onde vai escrever a resposta.
Recebe ainda dois inteiros que definem a aresta a ser retirada.
***********************************************************************/
void C1(Graph *G, FILE *f, int v1, int v2){
	
	int st[G->V], st2[G->V], a = 0, a2 = 0, r=-1, i;
	double c = 0, c2 = 0, val[G->V+1], val2[G->V+1];
	
	backbone(G, st, val); /*cálculo backbone original*/
	
	/*calcula o custo total do backbone*/
	for(i=0; i<G->V; i++){ 
		if(st[i]!=i && st[i]!=-1){
			c=c+val[i];
			a++; /*numero de arestas do backbone original*/
		}
	}
		
	/*verifica se aresta pertence ao grafo*/
	if(v1>0 && v1<=G->V && v2>0 && v2<=G->V){
		if(G->m[v1-1][v2-1]==MAX || G->m[v1-1][v2-1]<=0){
			fprintf(f, "%d %.2lf %d\n", a, c, r);
			escreve(G, f, st, val);
			fprintf(f, "\n");
			return;
		}
	}
	else{
		fprintf(f, "%d %.2lf %d\n", a, c, r);
		escreve(G, f, st, val);
		fprintf(f, "\n");
		return;
	}
		
	for(i=0; i<G->V; i++){ /*verifica se aresta pertence ao backbone
						   original, guarda em r essa informação*/
						   
		if(st[v1-1]==v2-1 || st[v2-1]==v1-1){
			r=1;
		}
		else{
			fprintf(f, "%d %.2lf %d\n", a, c, r);
			escreve(G, f, st, val);
			fprintf(f, "\n");
			return;
		}	
	}
	
	fprintf(f, "%d %.2lf ", a, c);
	
	/*remove aresta*/
	G->m[v1-1][v2-1] = MAX;
	G->m[v2-1][v1-1] = MAX;
	G->A--;
	
	if(r==1) /*calcula segundo backbone se aresta pertence ao original*/
		backbone(G, st2, val2);
	
	if(r==1){ /*calcula custo do segundo backbone se a aresta pertencer
			  ao original*/
			  
		for(i=0; i<G->V; i++){
			if(st2[i]!=i && st2[i]!=-1){
				c2=c2+val2[i];
				a2++; /*numero arestas backbone secundario*/
			}
		}
		fprintf(f, "%d %.2lf\n", a2, c2);
	}
	else{
		fprintf(f, "%d\n", r);
	}
	
	escreve(G, f, st, val);
	
	if(r==-1){ /*caso a aresta nao pertença ao original faz return, pois
			   não há um segundo backbone a escrever*/
			   
		fprintf(f, "\n");
		return;
	}
	
	/*escreve backbone secundario*/
	escreve(G, f, st2, val2);
	
	fprintf(f, "\n");
}


/***********************************************************************
 * Função: D1

Resolve problemas do tipo D1. Recebe o grafo com a matriz de adjacências
e um ponteiro para o ficheiro de saída onde vai escrever a resposta.
Recebe ainda um inteiro que define o vértice a ser retirado.
***********************************************************************/
void D1(Graph *G, FILE *f, int v1){
	int a=0, r=-1, d, i, j, k, v=0, cnt=0, cnt2=0, ret=0;
	int st[G->V], stbb[G->V];
	double c = 0, val[G->V+1], valbb[G->V+1];
	Rotas *U;
	
	backbone(G, st, val); /*cálculo backbone original*/
	
	/*calcula o custo total do backbone*/
	for(i=0; i<G->V; i++){
		if(st[i]!=i && st[i]!=-1){
			c=c+val[i];
			a++; /*numero de arestas do backbone original*/
		}
	}
	
	if(v1<0 || v1>G->V){
		r=-1;
		fprintf(f, "%d %.2lf %d\n", a, c, r);
		escreve(G, f, st, val);
		fprintf(f, "\n");
		return;
	}
	
	/*guarda st do backbone*/
	valbb[G->V+1]=val[G->V+1];
	for(i=0;i<G->V;i++){
		stbb[i]=st[i];
		valbb[i]=val[i];
	}
	
	/*remove vértive*/
	if(st[v1-1]!=v1-1){
				
		/*remove aresta do grafo*/
		G->m[v1-1][st[v1-1]] = MAX;
		G->m[st[v1-1]][v1-1] = MAX;
		G->A--;
		a--;
		
		st[v1-1]=v1-1;
		val[v1-1]=MAX;
		ret++;
	}
	
	for(i=0; i<G->V; i++)
		if(st[i]==v1-1 && i!=v1-1){
						
			/*remove aresta do grafo*/
			G->m[v1-1][i] = MAX;
			G->m[i][v1-1] = MAX;
			G->A--;
			a--;
			
			st[i]=i;
			val[i]=MAX;
			
			ret++;
	}
	
	/*verifica se o vertice pertence ao grafo*/
	if(ret==0){
		r=-1;
		fprintf(f, "%d %.2lf %d\n", a, c, r);
		escreve(G, f, st, val);
		return;
	}
	
	d = G->A - a; /*número de rotas que não pertencem ao backbone*/
	
	/*aloca memoria para a estrutura U que guarda todas as rotas
	alternativas*/		
	U = (Rotas *) malloc(d * sizeof(Rotas));
	
	/*adiciona a U as arestas que nao pertencem ao backbone*/
	for(i=0; i<G->V; i++){
		if(cnt>d)
			break;
			
		for(j=i+1; j<G->V; j++){
			if(G->m[i][j]!=MAX){	
				v=0;
					
				/*verifica se pertence ao backbone*/
				for(k=0; k<G->V; k++){						
					if((st[k]==i && k==j)||(st[k]==j && k==i)){
													
						v = 1;							
						break;
					}						
				}
				
				/*garante que nenhum dos vertices é o que foi
				retirado*/
				if(v!=1 && i!=v1-1 && j!=v1-1){
					U[cnt].c = G->m[i][j];
					U[cnt].p1 = i;
					U[cnt].p2 = j;
					
					cnt++; /*conta nº arestas em U*/						
				}
			}	
		}
	}
		
	/*organiza U*/
	quicksort(U, 0, cnt-1);
				
	r = caminho2(U, st, cnt);
	
	a = a + ret;
			
	fprintf(f, "%d %.2lf %d\n", a, c, r);
	
	escreve(G, f, stbb, valbb);
	
	quicksort2(U, 0, cnt-1);
	
	/*ordena rotas alternativas*/
	if(r>1){
		r=0;
		for(i=0; i<cnt; i++){
			
			while(U[i].p1==U[i+1].p1){
				i++;
				cnt2++;
			}
			for(j=r; j<cnt2; j++){
				int min = j;
				for(k=j+r;k<=cnt2;k++){
					if (less(U[k].p2, U[min].p2))
						min = k;
						
					//exch(U[j].p1, U[min].p1)
					exch(U[j].p2, U[min].p2)
				}
			}
			r++;
			cnt2=r;
		}
	}
	
	/*escreve arestas das rotas alternativas*/
	if(r!=0){
		for(i=0; i<cnt; i++){
			if(U[i].c!=MAX && U[i].c!=0){
				fprintf(f, "%d %d %.2lf\n", U[i].p1+1, U[i].p2+1, U[i].c);
			}
		}
	}
	
	fprintf(f, "\n");
	
	/*liberta estrutura U*/
	free(U);
}


/***********************************************************************
 * Função: E1

Resolve problemas do tipo E1. Recebe o grafo com a matriz de adjacências
e um ponteiro para o ficheiro de saída onde vai escrever a resposta.
***********************************************************************/
void E1(Graph *G, FILE *f){
	int a=0, r=-1, d, i, j, k, v=0, cnt=0, cnt2=0, caso;
	int st[G->V], *bbv1, *bbv2;
	double c = 0, *bbc, val[G->V+1];
	Rotas *U;
	
	backbone(G, st, val); /*cálculo backbone original*/
	
	/*calcula o custo total do backbone*/
	for(i=0; i<G->V; i++){ 
		if(st[i]!=i && st[i]!=-1){
			c=c+val[i];
			a++; /*numero de arestas do backbone original*/
		}
	}
	
	/*aloca memória para tabelas que guardam valores das arestas do
	backbone*/
	bbv1 = (int *) malloc(a * sizeof(int));
	bbv2 = (int *) malloc(a * sizeof(int));
	bbc = (double *) malloc(a * sizeof(double));
	
	fprintf(f, "%d %.2lf\n", a, c);
	
	d = G->A - a; /*número de rotas que não pertencem ao backbone*/
	
	/*aloca memoria para a estrutura U que guarda todas as rotas
	alternativas*/		
	U = (Rotas *) malloc(d * sizeof(Rotas));
	
	/*adiciona a U as arestas que nao pertencem ao backbone*/
	for(i=0; i<G->V; i++){		
		for(j=i+1; j<G->V; j++){
			if(G->m[i][j]!=MAX){	
				v=0;
					
				/*verifica se pertence ao backbone*/
				for(k=0; k<G->V; k++){
					
					/*guarda arestas pertencentes ao backbone*/				
					if((st[k]==i && k==j)||(st[k]==j && k==i)){
						bbv1[cnt] = i;
						bbv2[cnt] = j;
						bbc[cnt] = G->m[i][j];
						v = 1;
						cnt++;						
						break;
					}						
				}
				
				/*guarda aresta que nao pertencem ao backbone*/
				if(v!=1){
					U[cnt2].c = G->m[i][j];
					U[cnt2].p1 = i;
					U[cnt2].p2 = j;

					cnt2++; /*conta nº arestas em U*/						
				}
			}	
		}
	}
	
	/*organiza U*/ 
	quicksort(U, 0, d-1);
		
	for(i=0; i<a; i++){
		r=-1;
		
		fprintf(f, "%d %d %.2lf ", bbv1[i]+1, bbv2[i]+1, bbc[i]);
		
		/*remove aresta do backbone*/
		if(st[bbv1[i]]==bbv2[i]){
			st[bbv1[i]]=bbv1[i];
			caso=1;
		}
		else{
			st[bbv2[i]]=bbv2[i];
			caso=2;
		}
		for(j=0; j<d; j++){
				
			r = caminho(st, U[j].p1, U[j].p2);
			
			/*escreve a rota alternativa*/
			if(r==1){
				fprintf(f, "%d %d %.2lf\n", U[j].p1+1, U[j].p2+1, U[j].c);
				break;				
			}
		}
		if(r==-1)
			fprintf(f, "%d\n", r);
		
		/*volta a adicionar aresta ao backbone*/
		if(caso==1)
			st[bbv1[i]]=bbv2[i];
		else
			st[bbv2[i]]=bbv1[i];

	}
	fprintf(f, "\n");
	
	/*liberta estrutura U e arrays bb*/
	free(U);
	free(bbv1);
	free(bbv2);
	free(bbc);
}


/***********************************************************************
 * Função: liberta

Liberta a memória da matriz de adjacências.
***********************************************************************/
void liberta(Graph *G){
	int i;
	for(i=0; i<G->V; i++)
		free(G->m[i]);
	free(G->m);
	free(G);
}

