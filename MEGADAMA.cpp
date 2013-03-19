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
Titulo do problema: Mega Damas

Link: http://br.spoj.com/problems/MEGADAMA/

Solução: AI, backtracking com podas.
*/

#include <stdio.h>
/* Movimentos*/

int dx[]={-1,-1, 1, 1};
int dy[]={-1, 1,-1, 1};

int MAX, N, M;
int n2, n1;
int pos[900][2];

int tab[30][30];

void calc(int xo, int yo, int n){
	if(n>MAX) MAX=n;
	if(MAX==n2) return;
	
	int x1, y1, x2, y2;
	int i;
	
	tab[xo][yo]=0;
	
	for(i=0;i<4;i++){
		x1=xo+dx[i]; y1=yo+dy[i];
		x2=x1+dx[i]; y2=y1+dy[i];
		if(x2>-1 && x2<N && y2>-1 && y2<M)
			if(tab[x1][y1]==2 && tab[x2][y2]==0){
				tab[x1][y1]=0; tab[x2][y2]=1;
				calc(x2,y2,n+1);
				tab[x1][y1]=2; tab[x2][y2]=0;
			}
	}
	tab[xo][yo]=1;
}

int main()
{
	int x, i, j;
		
	for(;;){
		scanf("%d%d\n",&N,&M);
		if(!(N||M)) return 0;
		
		n1=n2=0;
		for(i=0;i<N;i++)
			for(j=i&0x1;j<M;j+=2){
				scanf("%d",&x);
				tab[i][j]=x;
				if(x==2) n2++;
				else if(x==1){pos[n1][0]=i; pos[n1][1]=j; n1++; }
			}
			
		MAX=0;
		for(i=0;i<n1;i++)
			calc(pos[i][0],pos[i][1],0);
	
		printf("%d\n",MAX);
	}
	return 0;
}




