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
Titulo do problema: O cubo

Link: http://br.spoj.com/problems/CUBO/

Solução: BFS no grafo de estados do problema
*/

#include <stdio.h>
#include <string.h>
#include <map>
#include <queue>

#define INF 0x7fffffff

struct val{
	int db, dj;
	val(int db=INF, int dj=INF): db(db),dj(dj){};
	val(const val& d): db(d.db), dj(d.dj){};
};

struct key{
	int bx, by;
	int jx, jy;
	val d;
	
	key(int bx=0, int by=0,
		int jx=0, int jy=0, const val& d=0):
		bx(bx), by(by),
		jx(jx), jy(jy), d(d)
		{}
};


using namespace std;

int r, c;
int mat[21][21];

static const int dx[]={ 0, 0,-1, 1};
static const int dy[]={-1, 1, 0, 0};

val D[21][21];
int DJ[21][21];
int vis[21][21][21][21];
static key Q[21*21*21*21];
int begin, end;

int Qx[21*21];
int Qy[21*21];
int ini, fim;

int bfs(int sx, int sy, int tx, int ty)
{
	for(int x=0;x<r;x++)
		for(int y=0;y<c;y++)
			DJ[x][y]=INF;
	
	ini=fim=0;
	Qx[fim]=sx;
	Qy[fim]=sy;
	fim++;
	DJ[sx][sy]=0;
	while(ini<fim){
		int ux=Qx[ini], uy=Qy[ini], du=DJ[ux][uy]+1;
		ini++;
		if(ux==tx && uy==ty) break;
		
		for(int i=0;i<4;i++){
			int vx=ux+dx[i], vy=uy+dy[i];
			
			if(vx>-1 && vx<r && vy>-1 && vy<c)
				if(mat[vx][vy])
				if(du<DJ[vx][vy]){
					DJ[vx][vy]=du;
					Qx[fim]=vx;
					Qy[fim]=vy;
					fim++;
				}
		}
	}
	
	return DJ[tx][ty];
}

int main()
{
	int jxo, jyo;
	int bxo, byo;
	int tx, ty, gogo40=1;
	char a;
	
	while(scanf("\n%d %d\n",&r,&c),r||c){
		
		tx=ty=jxo=jyo=bxo=byo=INF;
		
		for(int i=0;i<r;i++){
			for(int j=0;j<c;j++){
				a=getc(stdin);
				switch(a){
					case 'T': mat[tx=i][ty=j]=1; break;
					case 'S': mat[jxo=i][jyo=j]=1; break;
					case 'B': mat[bxo=i][byo=j]=0; break;
					case '.': mat[i][j]=1; break;
					case '#': mat[i][j]=0; break;
					default: while(1); break;
				}
				D[i][j]=val(INF,INF);
				for(int k=0;k<r;k++)
					for(int l=0;l<c;l++)
						vis[i][j][k][l]=0;
			}
			a=getc(stdin);
		}
		
		if(tx<INF && ty<INF && jxo<INF && jyo<INF && bxo<INF && byo < INF){
			begin=end=0;
			Q[end++]=key(bxo,byo,jxo,jyo,val(0,0));
			while(begin<end){
				key u=Q[begin];
				begin++;
				
				int dbu=u.d.db;
				int dju=u.d.dj;
				
				mat[u.bx][u.by]=0;
				
				for(int i=0;i<4;i++){
					int vx=u.bx+dx[i], vy=u.by+dy[i];
					int vpx=u.bx-dx[i], vpy=u.by-dy[i];
					
					if( 
						vx>-1  && vx<r  && vy>-1  && vy<c &&
						vpx>-1 && vpx<r && vpy>-1 && vpy<c
						){
						if(mat[vx][vy] && mat[vpx][vpy]){
							int dt=bfs(u.jx,u.jy,vpx,vpy);
							if(dt<INF){
								int djv=dju+dt+1;
								val t=val(dbu+1,djv);
								if(!vis[u.bx][u.by][vpx][vpy]){
									vis[u.bx][u.by][vpx][vpy]=1;
									if(t.db<D[vx][vy].db ||(t.db==D[vx][vy].db && t.dj<D[vx][vy].dj)) D[vx][vy]=t;
									Q[end++]=(key(vx,vy,u.bx,u.by,t));
								}
							}	
						}
					}
				}
				
				mat[u.bx][u.by]=1;
			}
		}else while(1);
		
		printf("Instancia %d\n",gogo40++);
		if(D[tx][ty].db<INF && D[tx][ty].dj<INF) printf("%d %d\n\n",D[tx][ty].dj, D[tx][ty].db);
		else printf("Impossivel\n\n");
	}
	return 0;
}



