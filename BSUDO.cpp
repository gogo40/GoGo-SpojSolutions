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
Titulo do problema: SUDOKU

Link: http://br.spoj.com/problems/BSUDO/

Solução: AI, backtracking com podas.
*/
#include <stdio.h>

#define INF 500

using namespace std;


/*vetor de palavras com tamanho menor que 32 bits**/
typedef long long lint;
typedef unsigned long long ulint;

struct V{
	//tam_word=9  NMAX=20;
	//n_words=64/9 numero de palavras por chunck
	
	int n_words, nbits, MASK;
	
	ulint v[6];
	ulint mask;
	
	V(int n_words=5,int nbits=10, int MASK=1023)
	:n_words(n_words),nbits(nbits),mask(MASK){
		for(int i=0;i<6;i++) v[i]=0;
	}
	
	V(const V& b){
		n_words=b.n_words; nbits=b.nbits;
		MASK=b.MASK; mask=b.mask;
		for(int i=0;i<6;i++) v[i]=b.v[i];
	}
	
	inline
	void insert(int p, ulint V){
		int i;
		
		i=p/n_words;
		p=p%n_words;
		
		p*=nbits;
		
		v[i]=(v[i]^(v[i] & (mask<<p))) | (V<<p);
	}
	
	inline
	int get(int p){
		int i;
		
		i=p/n_words;
		p=p%n_words;
		p*=nbits;
		
		return (int)((v[i]>>p)&mask);
	}
	
	V& operator=(const V& b){
		n_words=b.n_words; nbits=b.nbits;
		MASK=b.MASK;  mask=b.mask;
		for(int i=0;i<6;i++) v[i]=b.v[i];
		return *this;
	}

};

/****************************/


//Armazena estado anterior
struct st{
	V s, pl, pc, ps;//estado anterior
	int le, x, y;//ultima escolha e posicao onde ocorreu
	
	st():s(16,4,15),pl(),pc(),ps(),le(1),x(0),y(0){}
	
	st(const V& s, const V& pl, const V& pc, const V& ps, int le=1, int x=0, int y=0)
	:s(s),pl(pl),pc(pc),ps(ps),le(le),x(x),y(y){}
	
	st& operator=(const st& a){
		s=a.s; pl=a.pl; pc=a.pc; ps=a.ps;
		le=a.le; x=a.x; y=a.y;
		return *this;
	}
};


static st Q[81];
int ip;
static int hel[10];
static int lg[1<<10];
static int ones[1<<10];
static const ulint inf=~(0LL);

//FUNCAO PRO UVA AL 3304

inline
void print(V& sol)
{
	int i, j;
	for(i=0;i<9;i++){
		for(j=0;j<9;j++) printf("%d",sol.get(i*9+j));
		printf("\n");
	}
}

//FUNCAO PRO SPOJ CHALLENGE 1414
/*inline
void print(V& sol)
{
	printf("Y\n");
	for(int i=0;i<81;i++) printf("%d",sol.get(i));
	printf("\n");
}
*/

bool solve(V& s){
	V podeLin, podeCol, podeSet;
	int i, j, x, y, a, b, set;
	int px, py, mi, l, ns;
	bool ok;
	
	for(i=0;i<2;i++) podeCol.v[i]=podeSet.v[i]=podeLin.v[i]=inf;
	
	ok=true;
	ip=0;
	int np=0;
	for(;;){
	
		//Verifica se j� resolvi o sudoku s
		//print(s);
		init:
		np++;
		if(!ok)
			for(;;){
				ip--;
				
				if(ip==-1) return false;
				else{
					s=Q[ip].s;
					podeLin=Q[ip].pl; podeCol=Q[ip].pc; podeSet=Q[ip].ps;
					px=i=Q[ip].x; py=j=Q[ip].y;
					
					a=(i/3); b=(j/3); set=a*3+b;
					y=podeLin.get(i); y&=podeCol.get(j); y&=podeSet.get(set); 
					
					for(i=Q[ip].le+1;i<10;i++)
						if((y>>i)&0x1){
							Q[ip]=st(s,podeLin,podeCol,podeSet,i,px,py);
							ip++;
							s.insert(px*9+py,i);
							goto cont;
						}
				}
			}
		
		cont:
		
		//Realiza travas  e procura posi��es que tem unicas op��es
		ns=0;
		for(i=0;i<9;i++)
			for(j=0;j<9;j++){
				x=s.get(i*9+j);
				a=(i/3); b=(j/3); set=a*3+b;
					
				if(x>0){
					ns++;
					//Atualiza linha
					y=podeLin.get(i); y&=hel[x]; podeLin.insert(i,y);
					//Atualiza coluna
					y=podeCol.get(j); y&=hel[x]; podeCol.insert(j,y);
					//Atualiza setor
					y=podeSet.get(set); y&=hel[x]; podeSet.insert(set,y);
				}
			}
		
		
		if(ns==81){ print(s);  return true; }
		
		
		mi=INF; ok=false;
		for(i=0;i<9;i++)
			for(j=0;j<9;j++){	
				x=s.get(i*9+j);
				a=(i/3); b=(j/3); set=a*3+b;
					
				if(x==0){
					y=podeLin.get(i); y&=podeCol.get(j); y&=podeSet.get(set); 
					x=lg[y];
					if(x>0){
						s.insert(i*9+j,x);
						y=podeLin.get(i); y&=hel[x]; podeLin.insert(i,y);
						y=podeCol.get(j); y&=hel[x]; podeCol.insert(j,y);
						y=podeSet.get(set); y&=hel[x]; podeSet.insert(set,y);
						ok=true;
					}else if(y==0)  goto init; 
					else{
						x=ones[y]; 
						if(x<mi){ mi=x; l=y; px=i; py=j; }
					}
				}
			}
		
		
		if(!ok){
			i=px; j=py; a=(i/3); b=(j/3); set=a*3+b;
			y=podeLin.get(i); y&=podeCol.get(j); y&=podeSet.get(set); 
			
			for(i=1;i<10;i++)
				if((y>>i)&0x1){
					Q[ip]=st(s,podeLin,podeCol,podeSet,i,px,py);
					ip++;
					s.insert(px*9+py,i);
					ok=true;
					break;
				}
		}
	}
}

//*********************************************************//
//MAIN
//*********************************************************//

//FUNCAO PRO UVA AL 3304
/*
int main()
{
	int N, x, i, j, len, m;
	V s(16,4,15);
	
	len=1<<10; m=len-1;
	for(i=0;i<len;i++) {
		lg[i]=-1;
		x=0;
		for(j=0;j<10;j++)
			if(i>>j & 0x1)x++;
		ones[i]=x;
	}
	
	for(i=0;i<10;i++)  { 
		lg[1<<i]=i;
		hel[i]=m-(1<<i);
		if(i>0) hel[i]--;
	}
	
	
	scanf("%d",&N);
	while(N--){
		for(i=0;i<9;i++)
			for(j=0;j<9;j++){
				scanf("%1d",&x);
				s.insert(i*9+j,x);
			}
		
		if(!solve(s)) printf("impossible\n\n");
	}
	return 0;
}
*/

/*
81/16=
*/
//spoj challenge 1414
/*
int main()
{
	int N, x, i, j, len, m;
	V s(16,4,15);
	
	len=1<<10; m=len-1;
	for(i=0;i<len;i++) {
		lg[i]=-1;
		x=0;
		for(j=0;j<10;j++)
			if(i>>j & 0x1)x++;
		ones[i]=x;
	}
	
	for(i=0;i<10;i++)  { 
		lg[1<<i]=i;
		hel[i]=m-(1<<i);
		if(i>0) hel[i]--;
	}

	char in[82];
	scanf("%d\n",&N);
	while(N--){
		scanf("%s",in);
		for(int i=0;i<81;i++) 
		if(in[i]=='.') s.insert(i,0);
		else s.insert(i,in[i]-'0');
		if(!solve(s)) printf("N\n");
	}
	return 0;
}
*/

//FUNCAO PRO UVA AL 4245
bool find_error(V& s){
	for(int n=1;n<10;n++){
		int noc;
		for(int i=0;i<9;i++){
			noc=0;
			for(int j=0;j<9;j++)
				if(s.get(i*9+j)==n) noc++;
			if(noc>1) return false;
		}
		
		for(int i=0;i<9;i++){
			noc=0;
			for(int j=0;j<9;j++)
				if(s.get(j*9+i)==n) noc++;
			if(noc>1) return false;
		}
		
		for(int i=0;i<3;i++){
			int lm=3*(i+1);
			int auxI=3*i;
			
			for(int j=0;j<3;j++){
				int lp=3*(j+1);
				noc=0;
				
				for(int m=auxI;m<lm;m++)
					for(int p=3*j;p<lp;p++){
						int x=s.get(m*9+p);
						if(x==n) noc++;
					}
				if(noc>1) return false;
			}	
		}
		
	}
	return true;
}

int main()
{
	int N, x, i, j, len, m,c;
	V s(16,4,15);
	
	len=1<<10; m=len-1;
	for(i=0;i<len;i++) {
		lg[i]=-1;
		x=0;
		for(j=0;j<10;j++)
			if(i>>j & 0x1)x++;
		ones[i]=x;
	}
	
	for(i=0;i<10;i++)  { 
		lg[1<<i]=i;
		hel[i]=m-(1<<i);
		if(i>0) hel[i]--;
	}
	
	c=1;
	scanf("%d",&N);
	while(N--){
		for(i=0;i<9;i++)
			for(j=0;j<9;j++){
				scanf("%1d",&x);
				s.insert(i*9+j,x);
			}
		
		if(!find_error(s)) printf("Could not complete this grid.\n");
		else
		 if(!solve(s)) printf("Could not complete this grid.\n");
		c++;
	}
	return 0;
}



