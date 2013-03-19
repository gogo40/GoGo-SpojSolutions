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
Titulo do problema: A rã saltadora preguiçosa

Link: http://br.spoj.com/problems/RAPREGUI/

Solução: A* 
*/

#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <math.h>
#include <queue>

#define INF 0x3f3f3f3f

using namespace std;

struct Key{
	int v;
};

int C, R, W;
bool vis[1000000];
bool lagoa[1000000];
int dist[1000000];
int F[1000000];
Key Q[1000000];

int dx[]={
		  -2,-2,-2,-2,-2,
		  -1,-1,-1,-1,-1, 
		   0, 0, 0, 0,
		   1, 1, 1, 1, 1,
		   2, 2, 2, 2, 2
		  };

int dy[]={
		  -2,-1, 0, 1, 2,
		  -2,-1, 0, 1, 2,
		  -2,-1, 1, 2,
		  -2,-1, 0, 1, 2,
		  -2,-1, 0, 1, 2
		  };

int cal[]={ 7,6,5,6,7,
			6,3,2,3,6,
			5,2,  2,5,
			6,3,2,3,6,
			7,6,5,6,7
			};

int Cf, Rf, Ct, Rt;
int C1, R1, C2, R2;
int u, v, w, i, k;
int ux, uy, vx, vy;
int wx, wy;
int dxy, dyz, duv, dvw;
int TAMBOOL, TAMINT;
Key P;
int pCF, pCT;
int id;
bool* pl;
int* pc;
int nEL;
int Dxt, Dyt;	

inline
int abs(int a)
{
	return (a<0)?-a:a;
}
inline
int max(int a, int b)
{
	return (a>b)?a:b;
}
inline 
int min(int a, int b)
{
	return (a<b)?a:b;
}
inline
bool operator<(const Key& a, const Key& b)
{
	if(F[a.v]<F[b.v]) return false;
	return true;
}

int main()
{
	for(;;){
		scanf("%d%d",&C,&R);
		
		if(!C) return 0;
		
		TAMINT=C*R;
		for(i=0;i<TAMINT;i++){
			vis[i]=lagoa[i]=true;
			dist[i]=INF;
		}
		
		scanf("%d%d%d%d",&Cf,&Rf,&Ct,&Rt);
		
		Cf--;Rf--;Ct--;Rt--;
		pCF=Rf*C+Cf;
		pCT=Rt*C+Ct;
		
		scanf("%d",&W);
		for(i=0;i<W;i++){
			scanf("%d%d%d%d",&C1,&R1,&C2,&R2);
			C1--;R1--;
			for(u=R1, id=R1*C;u<R2;u++,id=u*C)
				for(v=C1,id+=v, pl=&lagoa[id];v<C2;v++,id++,pl++)
					*pl=false;
		}
		
		u=pCF;
		F[u]=dist[u]=0;
		nEL=1;
		Q[0].v = u;
		
		while(nEL>0){
			
			u=Q[0].v;
			pop_heap(Q,Q+nEL);
			nEL--;
			dxy=dist[u];
			
			if(!vis[u]) continue;
			if( u==pCT) break;
			
			vis[u]=lagoa[u]=false;
			ux=u/C;
			uy=u%C;
			
			for(i=0;i<24;i++){
				vx=ux + dx[i]; 
				if(vx>-1 && vx<R){
					vy=uy+dy[i];
					if( vy>-1 && vy<C){
						v=vx*C+vy;
						duv=cal[i]+dxy;
						if(lagoa[v])
						if(dist[v] > duv){
							Dxt=abs(Rt-vx);
							Dyt=abs(Ct-vy);
							F[v]=(dist[v]=duv)+3*(min(Dyt,Dxt))+2*(abs(Dyt-Dxt));
							Q[nEL].v=v;
							nEL++;
							push_heap (Q,Q+nEL);
						}
					}
				}
			}
			
		}
		
		if(dist[pCT]<INF)
			printf("%d\n",dist[pCT]);
		else
			printf("impossible\n");
	}
	return 0;
}


