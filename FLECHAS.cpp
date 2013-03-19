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
Titulo do problema: Flechas

Link: http://br.spoj.com/problems/FLECHAS/

Solução: AI, backtracking com podas, heurísticas.
*/
#include <stdio.h>

/*
|={1,0}
|={-1,0}
_={0,1}
_={0,-1}
\={1,1}
\={-1,-1}
/={1,-1}
/={-1,1}

\ | /
--*--
/ | \

*/

int N;
int X[20][20], Y[20][20];
int nc[20][20];
bool ok;

static int nx[100];
static int ny[100];
int np;

//8 direcoes
static const int dx[]={ 1,-1, 0, 0, 1,-1, 1,-1};
static const int dy[]={ 0, 0, 1,-1, 1,-1,-1, 1};

static const char* mask[]= { " S"," N"," E"," W","SE","NW","SW","NE","  "};

void print_sol(){
	printf("%s",mask[Y[0][0]]);
	for(int i=1;i<=N+1;i++) printf(" %s",mask[Y[0][i]]);
	printf("\n");
	for(int i=1;i<=N;i++){
		printf("%s",mask[Y[i][0]]);
		for(int j=1;j<=N;j++) printf("  %d",Y[i][j]);
		printf(" %s\n",mask[Y[i][N+1]]);
	}
	printf("%s",mask[Y[N+1][0]]);
	for(int i=1;i<=N+1;i++) printf(" %s",mask[Y[N+1][i]]);
	printf("\n");
}


void check()
{
	for(int i=1;i<=N;i++)
		for(int j=1;j<=N;j++)
			if(X[i][j]!=0) { return;}
	ok=true;
	Y[0][0]=Y[0][N+1]=Y[N+1][0]=Y[N+1][N+1]=8;
	for(int i=1;i<=N;i++){
		Y[i][0]=X[i][0];
		Y[i][N+1]=X[i][N+1];
		Y[0][i]=X[0][i];
		Y[N+1][i]=X[N+1][i];
	}
}

void solve(int x, int y, int p){
	int px, py;
	bool cont;
	
	if(ok) return;
	
	for(int i=1;i<=N;i++)
		for(int j=1;j<=N;j++){
			if(nc[i][j]<X[i][j] && X[i][j]>0) return;
		}
	
	for(int i=0;i<8;i++){
		px=x+dx[i]; py=y+dy[i];
		if(px>0 && px<=N && py>0 && py<=N){
			cont=true;
			X[x][y]=i;
			while(px>0&&px<=N&&py>0&&py<=N){
				X[px][py]--;
				if(X[px][py]<0) cont=false;
				px+=dx[i]; py+=dy[i];
			}
			if(p==np) check();
			
			if(cont && p<np && !ok){
				//Reduz o numero de candidatos em outras casas alinhadas a essa seta
				for(int j=0;j<8;j++){
					px=x+dx[j]; py=y+dy[j];
					while(px>0&&px<=N&&py>0&&py<=N){ 
						nc[px][py]--;
						px+=dx[j]; py+=dy[j];
					}
				}
				
				solve(x+nx[p],y+ny[p],p+1);
				
				for(int j=0;j<8;j++){
					px=x+dx[j]; py=y+dy[j];
					while(px>0&&px<=N&&py>0&&py<=N){ 
						nc[px][py]++;
						px+=dx[j]; py+=dy[j];
					}
				}
				
			}else if(ok) return;
			
			px=x+dx[i]; py=y+dy[i];
			while(px>0&&px<=N&&py>0&&py<=N){
				X[px][py]++;
				px+=dx[i]; py+=dy[i];
			}
			X[x][y]=8;
		}
	}
}

int main()
{
	int gogo40;
	gogo40=1;
	while(scanf("%d",&N),N){ 
		
		X[0][0]=X[0][N+1]=X[N+1][0]=X[N+1][N+1]=8;
		
		for(int i=1;i<=N;i++){
			X[i][0]=X[i][N+1]=X[0][i]=X[N+1][i]=8;
			for(int j=1;j<=N;j++){
				scanf("%d",&X[i][j]);
				Y[i][j]=X[i][j];
				nc[i][j]=8;
			}
		}
		
		for(int i=1;i<=N;i++) nc[i][i]=6;
		
		for(int i=N, j=1;j<=N;j++,i--) nc[j][i]=6;
		
		if(N%2) nc[N/2+1][N/2+1]=4;
		
		for(int i=0;i<N;i++){ nx[i]=0; ny[i]=1;}
		nx[N-1]=1; ny[N-1]=1;
		
		for(int i=N;i<2*N;i++){ nx[i]=1; ny[i]=0; }
		nx[2*N-1]=1; ny[2*N-1]=-1;
		
		for(int i=2*N;i<3*N;i++){ nx[i]=0; ny[i]=-1; }
		nx[3*N-1]=-1; ny[3*N-1]=-1;
		
		for(int i=3*N;i<4*N;i++){ nx[i]=-1; ny[i]=0; }
		np=4*N-1;
		
		ok=false;
		
		solve(0,1,0);
		
		printf("Instancia #%d:\n",gogo40);
		if(ok) print_sol();
		else printf("sem solucao\n");
		printf("\n");
		
		gogo40++;
	}
	return 0;
}




