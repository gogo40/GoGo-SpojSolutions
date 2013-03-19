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
Titulo do problema: GORE, PAZ VERDE HIPOCRISIA MUNDIAL

Link: http://br.spoj.com/problems/GORE/

Solução: Fenwick tree e line sweep.
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int id, px, py;
}point;

static int x[200000];
static int x2[200000];
int nft, nft2;

void buildFenwickTree(int t){
	int i;
	nft2=nft=t;
	for(i=0;i<=nft;i++) x2[i]=x[i]=0;
}

int sum(int i, int j) {
	if (i == 0) {
		int S = 0;
		for (j; j >= 0; j = (j & (j + 1)) - 1) S += x[j];
		return S;
	} else return sum(0, j) - sum(0, i-1);
}
	
void add(int k, int a) {
	for (; k < nft; k |= k+1) x[k] += a;
}

int sum2(int i, int j) {
	if (i == 0) {
		int S = 0;
		for (j; j >= 0; j = (j & (j + 1)) - 1) S += x2[j];
		return S;
	} else return sum2(0, j) - sum2(0, i-1);
}

void add2(int k, int a) {
	for (; k < nft; k |= k+1) x2[k] += a;
}
	

int cmpx(const void* a, const void* b){
	point* pa=(point*)a;
	point* pb=(point*)b;
	return pa->px-pb->px;
}

static point pnts [100000];
static int sol[100000];


int main () {
	int gogo40, x, y, maxY, i, j;
	int left, right, s, t;
	int n, D;
	
	scanf("%d",&gogo40);
	while(gogo40--){
		scanf("%d%d", &n,&D);
		
		maxY=-3000000;
		for (i=0;i<n;i++){
			scanf("%d %d", &x, &y);
			pnts[i].px=x-y;
			pnts[i].py=y+x;
			
			if(pnts[i].py>maxY)maxY=pnts[i].py;
			
			pnts[i].id=i;
			sol[i]=0;
		}
		
		buildFenwickTree(maxY+1);
		
		qsort(pnts,n,sizeof(point),cmpx);
		
		left=0; right=n-1;
		add(pnts[left].py,1); add2(pnts[right].py,1);
		
		for(i=1, j=n-2;i<n;i++, j--){
			
			while(pnts[i].px-pnts[left].px > D && left<i) {
				add(pnts[left].py,-1);
				left++;
			}
			
			s=(pnts[i].py-D<0)?(0):(pnts[i].py-D);
			t=(pnts[i].py+D>maxY)?maxY:(pnts[i].py+D);
			sol[pnts[i].id]+=sum(s,t);
			add(pnts[i].py,1);
			
			while(pnts[right].px-pnts[j].px>D && right>j){
				add2(pnts[right].py,-1);
				right--;
			}
			
			s=(pnts[j].py-D<0)?(0):(pnts[j].py-D);
			t=(pnts[j].py+D>maxY)?maxY:(pnts[j].py+D);
			sol[pnts[j].id]+=sum2(s,t);
			
			add2(pnts[j].py,1);
		}
		
		
		for(i=0;i<n;i++) printf("%d ",sol[i]);
		printf("\n");
	}
    return 0;
}



