/*
SPOJ SOLUTION
Copyright (c) 2011-2013 Péricles Lopes Machado (gogo40)

  Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:
The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

/*
Titulo do problema: Demonstração de Honestidade

Link: http://br.spoj.com/problems/HONESTID/

Solução: Aplicar o algoritmo de intersecção de matróides
para encontrar a maior árvore de cobertura colorida no 
grafo do problema.
*/

#include <stdio.h>
#include <string.h>
 
 
#define INF 0x3f3f3f3f
 
 
typedef struct{
	int u, v, c;
}ar;
 
 
static int G[101][10001];
static int grau[101];
 
static int comp[101];
 
 
static unsigned short DI[10003][10003];
static int grauI[10003];
static int distI[10003];
static int pi[10003];
static int V[10003];
 
int n, m, k, nI;
static ar E[10003];
static int corPode[201];
static int I[10003];
static int J[10003];
static int cI[10003];
int s, t;
 
static int Q[10003], ini, fim;
 
void dfs(int u, int nc)
{
	int i, v;
	
	comp[u]=nc;
	for(i=0;i<grau[u];i++){
		v=G[u][i];
		if(I[v]){
			if(!comp[E[G[u][i]].u])
				dfs(E[G[u][i]].u,nc);
			
			if(!comp[E[G[u][i]].v])
				dfs(E[G[u][i]].v,nc);
		}
	}
}
 
/*Encontra componentes conexas no grafo restrito a I, ou seja, o grafo com somente arestas permitidas*/
void find_comp()
{
	int i;
	for(i=0;i<=n;i++) comp[i]=0;
	for(i=1;i<=n;i++)
		if(!comp[i])
			dfs(i,i);
}
 
int main()
{
	int gogo40=1, i, ok, x, y, k;
	int u, v, c, du, ns, nt;
	
	s=10000; t=10001;
	while(scanf("%d %d %d",&n,&m,&k)==3){
		
		for(i=0;i<=n;i++) grau[i]=0;
		for(i=0;i<=k;i++) corPode[i]=1;
		
		for(i=0;i<m;i++){
			scanf("%d %d %d",&u,&v,&c);
			E[i].u=u, E[i].v=v, E[i].c=c;
			G[u][grau[u]]=i;
			G[v][grau[v]]=i;
			grau[u]++; grau[v]++;
			I[i]=J[i]=0;
		}
		
		ok=0;
		if(k>=n-1){
			
			nI=0;
			for(;;){
				
				/*Inicializa��o*/	
				nI=grauI[s]=grauI[t]=V[s]=V[t]=0;
				pi[s]=pi[t]=-1;  
				distI[t]=INF;
				
				for(i=0;i<=k;i++) corPode[i]=1;
				
				for(i=0;i<m;i++){ 
					I[i]=J[i];  
					V[i]=J[i]=grauI[i]=0;
					pi[i]=-1; 
					distI[i]=INF;
					if(I[i]) cI[nI++]=i, corPode[E[i].c]=0;
				}
				
				if(nI==n-1) break;
				
				/*Construo digrafo D(I)*/
				/////////////////////////////////////////////////////////
				/*
				Construo X1 e X2       
				x in S\I 
				pertence a X1 se I+x in I1 
				e x  pertence a X2 se I+x in I2
				*/
				find_comp();
				ns=nt=0;
				for(x=0;x<m;x++)
					if(!I[x]){
						ok=0;
					    if(corPode[E[x].c]){
							ok++;
							ns++;
							/*x pertence a X1*/
							DI[s][grauI[s]++]=x;
						}
						
						if(comp[E[x].u]!=comp[E[x].v]){
							ok++;
							nt++;
							/*x pertence a X2*/
							DI[x][grauI[x]++]=t;
						}
						if(ok==2) break;
					}
					
				if(ns==0 || nt==0) break;
				///////////////////////////////////////////////////////////////////////////////
				//Se x est� em X1 e X2, ent�o o menor caminho entre os dois conjuntos eh x
				if(ok==2) pi[t]=x, pi[x]=s;
				else{
					//Se n�o tenho elemento na intersec��o de X1 e X2, tenho de procurar uma sequencia aumentante
					for(i=0;i<nI;i++){
						y=cI[i];
						I[y]=0;
						find_comp();
						I[y]=corPode[E[y].c]=1;
						
						for(x=0;x<m;x++)
							if(!I[x]){
								/*(y,x) com y dentro de I e x fora existe se I-y+x estiver em I1*/
								if(corPode[E[x].c]){
									DI[y][grauI[y]++]=x;
								}
								
								/*(x,y) com y dentro de I e x fora existe se I-y+x estiver em I2*/
								if(comp[E[x].u]!=comp[E[x].v]){
									DI[x][grauI[x]++]=y;
								}	
							}
						
						corPode[E[y].c]=0;
					}
					
					/*Procuro menor caminho de X1 para X2*/
					ini=fim=0;
					Q[fim++]=s;
					distI[s]=0;
					while(ini<fim){
						u=Q[ini++];
						du=distI[u]+1;
						if(u==t) break;
						for(i=0;i<grauI[u];i++){
							v=DI[u][i];
							if(distI[v]>du){
								distI[v]=du;
								pi[v]=u;
								Q[fim++]=v;
							}
						}
					}
					/*Se n�o houver mais caminhos entre X1 e X2, pare, j� atingimos o conjunto maximal*/
					if(distI[t]>=INF) break;
				}
				
				/////////////////////////////////
				//I = (I - V (P) ) U(V(P) - I)
				//Aumento o conjunto com a diferenca simetrica do caminho aumentante e I
				for(x=pi[t];x!=s;x=pi[x]){
					V[x]=1;
					if(!I[x]) J[x]=1;
				}
				
				for(i=0;i<nI;i++){
					x=cI[i];
					if(!V[x]) J[x]=1;
				}
				
			}
			
			
			ok=nI;
		
		}
		
		
		printf("Instancia %d\n",gogo40++);
		if(ok==n-1) printf("sim\n\n");		
		else printf("nao\n\n");
	}
	return 0;
}
 
 
