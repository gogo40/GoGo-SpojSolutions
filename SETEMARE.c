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
Titulo do problema: Sete mares

Link: http://br.spoj.com/problems/SETEMARE/

Solução: Backtracking com memoization.
*/
#include <stdio.h>

#define false 0
#define true 1
#define bool char

typedef struct{
	int x, y;
}pos;

char grade[10][9][9];
pos posE[10][10];
pos undoE[10][10];

static const int dx[]={ 1,1,1, 0, 0,-1,-1,-1,0};
static const int dy[]={-1,1,0,-1, 1,-1, 1, 0,0};


bool bckt(int h, int xp, int yp)
{
	if(h>9) return false;
	
	int ix, iy;
	int nE, p, i, j;
	int k, vx, vy;
	
	p=h+1; nE=0;
	for(i=0;i<9;i++)
		for(j=0;j<8;j++){
			grade[p][i][j]=grade[h][i][j];
			if(grade[h][i][j]=='E'){ grade[p][i][j]='.'; posE[h][nE].x=i; posE[h][nE].y=j; nE++;}
		}
	
	if(nE==0) return true;
	
	for(i=0;i<8;i++){
		vx=xp+dx[i], vy=yp+dy[i];
		if(vx>-1 && vx<9 && vy>-1 && vy<8)
			if(grade[h][vx][vy]=='.'){
				for(k=0;k<nE;k++){
					if(vx<posE[h][k].x) ix=posE[h][k].x-1;
					else if(vx>posE[h][k].x) ix=posE[h][k].x+1;
					else ix=posE[h][k].x;
					
					if(vy<posE[h][k].y) iy=posE[h][k].y-1;
					else if(vy>posE[h][k].y) iy=posE[h][k].y+1;
					else iy=posE[h][k].y;
					
					if(ix==vx && iy==vy) break;
					if(grade[p][ix][iy]=='.') grade[p][ix][iy]='E';
					else if(grade[p][ix][iy]=='E') grade[p][ix][iy]='#';
					undoE[h][k].x=ix, undoE[h][k].y=iy;
				}
				
				if(k==nE)
					if(bckt(p,vx,vy)) return true;
				
				for(j=0;j<k;j++){
					grade[p][posE[h][j].x][posE[h][j].y]='.';
					grade[p][undoE[h][j].x][undoE[h][j].y]=grade[h][undoE[h][j].x][undoE[h][j].y];
					if(grade[p][undoE[h][j].x][undoE[h][j].y]=='E')
						grade[p][undoE[h][j].x][undoE[h][j].y]='.';
				}
			}
	}
	
	return false;
}

int main()
{
	int gogo40,i, j, xp, yp;
	
	scanf("%d",&gogo40);
	while(gogo40--){
	
		for(i=0;i<9;i++) scanf("%s\n",grade[0][i]);
		
		for(i=0;i<9;i++)	
			for(j=0;j<8;j++)
				if(grade[0][i][j]=='S'){
					xp=i; yp=j;
					grade[0][xp][yp]='.';
					goto br;
				}
		
		br: 
		if(bckt(0,xp,yp)) printf("I'm the king of the Seven Seas!\n");
		else printf("Oh no! I'm a dead man!\n");
	}
	
	return 0;
}



