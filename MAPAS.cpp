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

Link: http://br.spoj.com/problems/MAPAS/

Solução: Aceleração de pesquisas.
*/

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <set>
#include <map>

const int INF = 0x3F3F3F3F;
const int NULO = -1;
const double EPS = 1e-13;

inline
int cmp(double x, double y = 0, double tol = EPS) {
    return (x <= y + tol) ? (x + tol < y) ? -1 : 0 : 1;
}

using namespace std;

struct Mapa{
	int I, X1, Y1, X2, Y2;
	int area;
	double Xc, Yc;
	
	Mapa(int I=-1, int X1=-10001,int Y1=-10001, int X2=0, int Y2=0,
		 int area=0, double Xc=0, double Yc=0)
		 :I(I),X1(X1),Y1(Y1),X2(X2),Y2(Y2),
		  area(area),Xc(Xc),Yc(Yc){}
};

bool operator<(const Mapa& a, const Mapa& b){
	if(a.area!=b.area) return a.area<b.area;
	return a.I<b.I;
}

map<int, map<int, map<int, map<int, set<Mapa> > > > > mapas;
typedef map<int, map<int, map<int, map<int, set<Mapa> > > > >::iterator it1;
typedef map<int, map<int, map<int, set<Mapa> > > >::iterator it2;
typedef map<int, map<int, set<Mapa> > >::iterator it3;
typedef map<int, set<Mapa> >::iterator it4;
typedef set<Mapa>::iterator it5;

int main(){
	int M, R; double dc;
	int I, X1, Y1, X2, Y2;
	int area, gogo40=0;
	
	while(scanf("%d%d",&M,&R),M){
		
		mapas.clear();
		gogo40++;
		printf("Teste %d\n",gogo40);
		
		while(M--){
			scanf("%d%d%d%d%d",&I,&X1,&Y1,&X2,&Y2);
			Mapa m(I,X1,Y1,X2,Y2,abs(X1-X2)*abs(Y1-Y2),(X1+X2)*0.5,(Y1+Y2)*0.5);
			mapas[X2][X1][Y2][Y1].insert(m);
		}
		
		while(R--){
			scanf("%d%d",&X1,&Y1);
			
			I=-1; area=-1; dc=-1;
			it1 i1;
			i1=mapas.lower_bound(X1);
			for(;i1!=mapas.end();i1++){
				it2 i2=i1->second.begin();
				
				for(;i2!=i1->second.end() && (i2->first<=X1);i2++){
					it3 i3=i2->second.lower_bound(Y1);
					
					for(;i3!=i2->second.end();i3++){
						it4 i4=i3->second.begin();
						
						for(;i4!=i3->second.end() && (i4->first<=Y1);i4++){
							it5 i5=i4->second.begin();
							for(;i5!=i4->second.end() && (area==-1 || area>=i5->area);i5++){
								Mapa m=*i5; double dcc, dx, dy;
								dx=X1-m.Xc;
								dy=Y1-m.Yc;
								dcc=sqrt(dx*dx+dy*dy);
								if(area==-1||area>m.area){
									area=m.area;
									dc=dcc;
									I=m.I;
								}else if(area==m.area){
									if(cmp(dc,dcc)>0){
										area=m.area;
										dc=dcc;
										I=m.I;
									}else if(cmp(dc,dcc)==0){
										if(I>m.I){
											area=m.area;
											dc=dcc;
											I=m.I;
										}
									}
								}
							}
						}	
					}	
				}
			}
			
			if(I==-1) printf("0\n");
			else printf("%d\n",I);
		}
		
		printf("\n");
	}
	
	return 0;
}



