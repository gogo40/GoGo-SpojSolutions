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
Titulo do problema: Strange billboard

Link: http://www.spoj.com/problems/CERC07B/

Solução: Algebra e determinação de sistema linear associado.
*/

#include <vector>
#include <stdio.h>

using namespace std;

typedef unsigned int uint;

const int TCHUNCKS=32;//tamanho chuncks
const int NCHUNCKS=256/TCHUNCKS+1;//numero de chuncks

inline int ones(int& x) { return __builtin_popcount(x); }

struct word{

	int chuncks[NCHUNCKS];

	word(int x=0){
		for(int i=0;i<NCHUNCKS;i++) chuncks[i]=0;
		chuncks[0]=x;
	}

	word& operator=(const int& x){
		for(int i=0;i<NCHUNCKS;i++) chuncks[i]=0;
		chuncks[0]=x;
		return *this;
	}

	word& operator=(const word& x){
		for(int i=0;i<NCHUNCKS;i++) chuncks[i]=x.chuncks[i];
		return *this;
	}

	word& operator|=(const int& x){
		chuncks[0]|=x;
		return *this;
	}

	word& operator|=(const word& x){
		for(int i=0;i<NCHUNCKS;i++) chuncks[i]|=x.chuncks[i];
		return *this;
	}

	word& operator^=(const word& x){
		for(int i=0;i<NCHUNCKS;i++) chuncks[i]^=x.chuncks[i];
		return *this;
	}
	
	word& operator&=(const word& x){
		for(int i=0;i<NCHUNCKS;i++) chuncks[i]&=x.chuncks[i];
		return *this;
	}


	void insert(int v, int i){
		int p=i%TCHUNCKS;
		int t=i/TCHUNCKS;
		int q=chuncks[t];
		chuncks[t]^= ((q>>p)&0x1)<<p;
		chuncks[t]|=v<<p;
	}
	
	int ones(){
		int t=0;
		for(int i=0;i<NCHUNCKS;i++) t+=::ones(chuncks[i]);
		return t;
	}
	
	int get(int x){
		return (chuncks[x/TCHUNCKS]>>(x%TCHUNCKS))&0x1;
	}
};

word operator<<(const word& a, int x){
	word b;
	for(int i=0;i<NCHUNCKS;i++)
		for(int j=0;j<TCHUNCKS;j++){
			int y=(a.chuncks[i]>>j)&0x1;
			int p=j+x, q, t;
			q=p/TCHUNCKS;
			t=p%TCHUNCKS;
			if(q < NCHUNCKS){
				b.chuncks[q]|=(y<<(t));
			}
		}
	return b;
}

word operator>>(const word& a, int x){
	word b;
	for(int i=0;i<NCHUNCKS;i++)
		for(int j=0;j<TCHUNCKS;j++){
			int y=(a.chuncks[i]>>j)&0x1;
			int p=j-x+i*TCHUNCKS, q, t;
			q=p/TCHUNCKS;
			t=p%TCHUNCKS;
			if(q < NCHUNCKS && p>-1){
				b.chuncks[q]|=(y>>(t));
			}
		}
	return b;
}

bool operator==(const word& a, const word& b){
	for(int i=0;i<NCHUNCKS;i++) if(a.chuncks[i]!=b.chuncks[i]) return false;
	return true;
}

bool operator==(const word& a, int b){
	for(int i=1;i<NCHUNCKS;i++) if(a.chuncks[i]!=0) return false;
	return a.chuncks[0]==b;
}

bool operator!=(const word& a, const word& b){
	for(int i=0;i<NCHUNCKS;i++) if(a.chuncks[i]!=b.chuncks[i]) return true;
	return false;

}

bool operator!=(const word& a, int b){
	for(int i=1;i<NCHUNCKS;i++) if(a.chuncks[i]!=0) return true;
	return a.chuncks[0]!=b;
}


word operator&(const word& a, const word& b){
	word res;
	for(int i=0;i<NCHUNCKS;i++) res.chuncks[i]=a.chuncks[i]&b.chuncks[i];
	return res;
}

inline
word operator*(const vector<word>& A, const word& s){
	int N;
	word t;
	N=A.size(); t=0;
	for(uint i=0, j=N-1;i<N;i++, j--) t.insert( (A[i]&s).ones()&0x1,j);
	return t;
}
//Tamanho da folga

void print_problema(vector<word>& B, word& X, int mark){
	int N=B.size();
	printf("\nPROBLEMA:\n");
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++) printf("%d ",B[i].get(j));
		printf("=%d %c\n",X.get(i),i==mark?'<':' ');
	}
	printf("---------------------------\n");
}

inline
int solve(vector<word>& B, word& X){
	int N, x, t, m;
	N=B.size();
	
	for(int i=0;i<N;i++){	
		if(!(B[i].get(i)))
		for(int k=i+1;k<N;k++)
			if(B[k].get(i)){
				B[i]^=B[k];
				X.insert(X.get(k)^X.get(i),i);
				//printf("k=%d<<<\n",k);
				break;
			}
			
		for(int k=i+1;k<N;k++)
			if(B[k].get(i)){
				B[k]^=B[i];
				//printf("%d xor %d->",X.get(k),X.get(i));
				X.insert(X.get(k)^X.get(i),k);
				//printf("%d\n",X.get(k));
			}
	}
	int tf=0;
	for(int i=N-1;i>-1;i--){
		if(!B[i].get(i)) tf++;
		for(int k=i-1;k>-1;k--){
			if(B[k].get(i)){
				B[k]^=B[i];
				X.insert(X.get(k)^X.get(i),k);
			}
		}
	}
	
	for(int i=N-tf;i<N;i++) if(X.get(i)) return -1;
	int M=1<<(tf);
	
	word S;

#ifdef DEBUG
	word best;
#endif

	int TM;
	TM=N;
	
	for(int i=0;i<M;i++){

		S=0;

		for(int k=0;k<N-tf;k++){
			for(int j=0;j<tf;j++){
				if( ( (i>>j)&0x1) && B[k].get(N-j-1))
					S.insert(S.get(k)^0x1,k);
			}
			S.insert(S.get(k)^X.get(k),k);
		}

		for(int k=N-tf;k<N;k++) S.insert((i>>N-k-1)&0x1,k);

		int nz=0;

		for(int k=0;k<N;k++)
			if(S.get(k)) nz++;

		if(nz<TM){
			TM=nz; 

#ifdef DEBUG
			best=S;
#endif

		}

	}

#ifdef DEBUG
	X=best;
#endif

	return TM;
}


/*
M=numero de linhas
N=numero de colunas
x=linha
y=coluna
*/

inline int get(int x, int y, int M, int N){
	return x*N+y;
}

static const int NMOV=5;
static const int dx[]={0,1,0,-1,0};
static const int dy[]={0,0,1,0,-1};

bool valid(int x, int y, int M, int N){
	return (x>-1)&&(x<M)&&(y>-1)&&(y<N);
}

int main()
{
	int M, N;
	char c;
	for(;;){
		scanf("%d%d\n",&M,&N);
		if(!(M||N)) return 0;

		vector<word> P(M*N);
		word X;

		X=0;
		for(int i=0;i<M;i++)
			for(int j=0;j<N;j++){
				scanf(" %c\n ",&c);
				if(c=='X') X.insert(1,get(i,j,M,N));
			}
		
		
		//Criando problema associado
		for(int i=0;i<M;i++)
			for(int j=0;j<N;j++)
				for(int k=0;k<NMOV;k++){
					int p=i+dx[k], q=j+dy[k];
					if(valid(p,q,M,N)){
						P[get(i,j,M,N)].insert(1,get(p,q,M,N));
					}
				}
	
		int gl;
		gl=solve(P,X);
		
		
		if(gl>-1){
			printf("You have to tap %d tiles.\n",gl);
			
#ifdef DEBUG
			printf("_____________________________________________\n\n");
			for(int i=0;i<M;i++){
				for(int j=0;j<N;j++){
					printf(" %d",X.get(get(i,j,M,N)));
				}
				printf("\n");
			}
			printf("_____________________________________________\n\n");
#endif

		}else{
			printf("Damaged billboard.\n");
		}
	}
	return 0;
}


