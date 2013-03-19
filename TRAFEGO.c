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
Titulo do problema: Tráfego

Link: http://br.spoj.com/problems/TRAFEGO/

Solução: Aplicar BFS no grafo de estados que representa o problema.
*/

#include <stdio.h>
#include <stdlib.h>

#define TAM  9957
#define V1   9865613 
#define V2  10099591

#define at(i,j) ((i)*6+(j))

typedef unsigned long long ulint;

typedef struct{
	ulint LH, HH;
}State;

void insert(State* s,int pos, int v)
{
	if(pos<18){
		pos*=3;
		ulint a=s->LH & (((ulint)0x7)<< pos );
		s->LH^=a; s->LH|=(((ulint)v)<<pos);
		return;
	}
	pos-=18; pos*=3;
	ulint a=s->HH & (((ulint)0x7)<< pos );
	s->HH^=a; s->HH|=(((ulint)v)<<pos);
}

int get(State* s, int pos)
{
	if(pos<18){
		pos*=3;
		return (int) ( ((s->LH)>>pos )& 0x7);
	}
	pos-=18;
	pos*=3;
	return (int) ( ((s->HH)>>pos) & 0x7);
}


int equal(State* a, State* b)
{
	return (a->LH==b->LH )&&(a->HH==b->HH ); 
}

void set(State* a, State* b)
{
	a->LH=b->LH;
	a->HH=b->HH;
}

static State* key[TAM];
static int d[TAM];
static int vis[TAM];
static int Q[TAM], ini, fim;

int hash(State* p)
{
	int i, k;
	i=( (p->LH)%V1 + (p->HH)%V2)%TAM;
	k=0;
	for(;;){
		if(key[i]==NULL){
			key[i]=(State*)malloc(sizeof(State));
			key[i]->LH=p->LH;
			key[i]->HH=p->HH;
			d[i]=vis[i]=0;
			break;
		}
		if(equal(key[i],p)) break;
		i=(i+1)%TAM;
		k++;
		if(k==TAM) break;
	}
	if(k==TAM) exit(-1);
	return i;
}

void print(State u)
{
	int i, j;
	printf("STATE 0:\n");
	for(i=0;i<6;i++){
		for(j=0;j<6;j++)
			printf("%c",get(&u,at(i,j))+'0');
		printf("\n");
	}
}

int main()
{
	State u, v; int marc[6][6];
	int i, j, n, m, gogo40=1, x, y, k, ans, pu;
	int t, pv, du;
	
	for(i=0;i<TAM;i++) key[i]=NULL;
	
	scanf("%d",&n);
	while(gogo40<=n){
		
		for(i=0;i<TAM;i++){
			d[i]=vis[i]=0;
			if(key[i]!=NULL){  free(key[i]); key[i]=NULL; d[i]=vis[i]=0; }
		}
		u.LH=u.HH=0;
		
		scanf("%d%d",&x,&y);
		insert(&u,at(x,y),1);
		insert(&u,at(x,y+1),1);
		
		
		scanf("%d",&m);
		for(i=0;i<m;i++){
			scanf("%d%d",&x,&y);
			insert(&u,at(x,y),2);
			insert(&u,at(x+1,y),2);
		}
		
		scanf("%d",&m);
		for(i=0;i<m;i++){
			scanf("%d%d",&x,&y);
			insert(&u,at(x,y),3);
			insert(&u,at(x+1,y),3);
			insert(&u,at(x+2,y),3);
		}
		
		scanf("%d",&m);
		for(i=0;i<m;i++){
			scanf("%d%d",&x,&y);
			insert(&u,at(x,y),4);
			insert(&u,at(x,y+1),4);
		}
		
		scanf("%d",&m);
		for(i=0;i<m;i++){
			scanf("%d%d",&x,&y);
			insert(&u,at(x,y),5);
			insert(&u,at(x,y+1),5);
			insert(&u,at(x,y+2),5);
		}
		
		
		ini=fim=0;
		Q[fim]=hash(&u); fim++;
		ans=-1;
		
		while(ini<fim){
			pu=Q[ini];
			set(&u,key[pu]); ini++;
			
			//printf("du:%d\n",d[pu]);
			
			if(get(&u,at(2,5))==1) { if(ans==-1 || ans>d[pu])ans=d[pu]; }
			//print(u);
			vis[pu]=1;
			/*Procuro estados adjascentes*/
			for(i=0;i<6;i++)
				for(j=0;j<6;j++)
					marc[i][j]=0;
			
			du=d[pu]+1;
			if(du>=ans && ans!=-1) continue;
			for(i=0;i<6;i++)
				for(j=0;j<6;j++)
					if(!marc[i][j] && (t=get(&u,at(i,j)))!=0){
						set(&v,&u);
						
						switch(t){
							case 1:
								marc[i][j]=marc[i][j+1]=1;
								insert(&v,at(i,j),0);
								insert(&v,at(i,j+1),0);
								for(k=j+2;k<6;k++){
									if(get(&u,at(i,k))==0){
										insert(&v,at(i,k),1);
										insert(&v,at(i,k-1),1);
										pv=hash(&v);
										if(d[pv]==0||d[pv]>du){
											d[pv]=du;
											Q[fim]=pv;
											fim++;
										}
										insert(&v,at(i,k),0);
										insert(&v,at(i,k-1),0);
									}else break;
								}
								
								for(k=j-1;k>-1;k--){
									if(get(&u,at(i,k))==0){
										insert(&v,at(i,k),1);
										insert(&v,at(i,k+1),1);
										pv=hash(&v);
										if(d[pv]==0||d[pv]>du){
											d[pv]=du;
											Q[fim]=pv;
											fim++;
										}
										insert(&v,at(i,k),0);
										insert(&v,at(i,k+1),0);
									}else break;
								}
							break;
							
							case 2:
								marc[i][j]=marc[i+1][j]=1;
								insert(&v,at(i,j),0);
								insert(&v,at(i+1,j),0);
								for(k=i+2;k<6;k++){
									if(get(&u,at(k,j))==0){
										insert(&v,at(k,j),2);
										insert(&v,at(k-1,j),2);
										pv=hash(&v);
										if(d[pv]==0||d[pv]>du){
											d[pv]=du;
											Q[fim]=pv;
											fim++;
										}
										insert(&v,at(k,j),0);
										insert(&v,at(k-1,j),0);
									}else break;
								}
								
								for(k=i-1;k>-1;k--){
									if(get(&u,at(k,j))==0){
										insert(&v,at(k,j),2);
										insert(&v,at(k+1,j),2);
										pv=hash(&v);
										if(d[pv]==0||d[pv]>du){
											d[pv]=du;
											Q[fim]=pv;
											fim++;
										}
										insert(&v,at(k,j),0);
										insert(&v,at(k+1,j),0);
									}else break;
								}
							break;
							
							case 3:
								marc[i][j]=marc[i+1][j]=marc[i+2][j]=1;
								insert(&v,at(i,j),0);
								insert(&v,at(i+1,j),0);
								insert(&v,at(i+2,j),0);
								for(k=i+3;k<6;k++){
									if(get(&u,at(k,j))==0){
										insert(&v,at(k,j),3);
										insert(&v,at(k-1,j),3);
										insert(&v,at(k-2,j),3);
										pv=hash(&v);
										if(d[pv]==0||d[pv]>du){
											d[pv]=du;
											Q[fim]=pv;
											fim++;
										}
										insert(&v,at(k,j),0);
										insert(&v,at(k-1,j),0);
										insert(&v,at(k-2,j),0);
									}else break;
								}
								
								for(k=i-1;k>-1;k--){
									if(get(&u,at(k,j))==0){
										insert(&v,at(k,j),3);
										insert(&v,at(k+1,j),3);
										insert(&v,at(k+2,j),3);
										pv=hash(&v);
										if(d[pv]==0||d[pv]>du){
											d[pv]=du;
											Q[fim]=pv;
											fim++;
										}
										insert(&v,at(k,j),0);
										insert(&v,at(k+1,j),0);
										insert(&v,at(k+2,j),0);
									}else break;
								}
							break;
							
							case 4:
								marc[i][j]=marc[i][j+1]=1;
								insert(&v,at(i,j),0);
								insert(&v,at(i,j+1),0);
								for(k=j+2;k<6;k++){
									if(get(&u,at(i,k))==0){
										insert(&v,at(i,k),4);
										insert(&v,at(i,k-1),4);
										pv=hash(&v);
										if(d[pv]==0||d[pv]>du){
											d[pv]=du;
											Q[fim]=pv;
											fim++;
										}
										insert(&v,at(i,k),0);
										insert(&v,at(i,k-1),0);
									}else break;
								}
								
								for(k=j-1;k>-1;k--){
									if(get(&u,at(i,k))==0){
										insert(&v,at(i,k),4);
										insert(&v,at(i,k+1),4);
										pv=hash(&v);
										if(d[pv]==0||d[pv]>du){
											d[pv]=du;
											Q[fim]=pv;
											fim++;
										}
										insert(&v,at(i,k),0);
										insert(&v,at(i,k+1),0);
									}else break;
								}
							break;
							
							case 5:
								marc[i][j]=marc[i][j+1]=marc[i][j+2]=1;
								insert(&v,at(i,j),0);
								insert(&v,at(i,j+1),0);
								insert(&v,at(i,j+2),0);
								for(k=j+3;k<6;k++){
									if(get(&u,at(i,k))==0){
										insert(&v,at(i,k),5);
										insert(&v,at(i,k-1),5);
										insert(&v,at(i,k-2),5);
										pv=hash(&v);
										if(d[pv]==0||d[pv]>du){
											d[pv]=du;
											Q[fim]=pv;
											fim++;
										}
										insert(&v,at(i,k),0);
										insert(&v,at(i,k-1),0);
										insert(&v,at(i,k-2),0);
									}else break;
								}
								
								for(k=j-1;k>-1;k--){
									if(get(&u,at(i,k))==0){
										insert(&v,at(i,k),5);
										insert(&v,at(i,k+1),5);
										insert(&v,at(i,k+2),5);
										pv=hash(&v);
										if(d[pv]==0||d[pv]>du){
											d[pv]=du;
											Q[fim]=pv;
											fim++;
										}
										insert(&v,at(i,k),0);
										insert(&v,at(i,k+1),0);
										insert(&v,at(i,k+2),0);
									}else break;
								}
							break;
						}
					}
		}
		
		printf("The minimal number of moves to solve puzzle %d is %d.\n",gogo40,ans);
		gogo40++;
		
	}
	
	return 0;
}





