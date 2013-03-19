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
Titulo do problema: Registrador de deslocamento

Link: http://br.spoj.com/problems/REGISTRA/

Solução: Baby-step Giant-step e algebra (teoria de grupo, 
determinação de grupo gerador e calculo de inversa).
Referencia: http://en.wikipedia.org/wiki/Baby-step_giant-step
*/

#include <vector>
#include <stdio.h>

using namespace std;

typedef unsigned int uint;

#define ones(x) __builtin_popcount(x)

inline
uint operator*(const vector<uint>& A, const uint& s){
	uint t, N;
	N=A.size(); t=0;
	for(uint i=0, j=N-1;i<N;i++, j--) t|=((ones(A[i]&s)&0x1)<<j);
	return t;
}

inline 
vector<uint> transp(const vector<uint>& A){
	uint N, x;
	N=A.size();
	vector<uint> B(N);
	for(int i=N-1;i>-1;i--){
		x=0;
		for(uint j=0;j<N;j++) x|=((A[j]>>(i))&0x1)<<(N-j-1);
		B[N-i-1]=x;
	}
	return B;
}

inline
vector<uint> operator*(const vector<uint>& A, const vector<uint>& B){
	uint N, x;
	N=A.size();
	
	vector<uint> C(N), D;
	D=transp(B);
	
	for(uint k=0;k<N;k++){
		C[k]=0;
		for(uint i=0, j=N-1;i<N;i++, j--) C[k]|=((ones(A[k]&D[i])&0x1)<<j);
	}
	
	return C;
}

inline
void inv(vector<uint>& I, vector<uint>& A){
	uint N, x, t, m;
	N=A.size();
	
	vector<uint> B(N);
	
	for(uint i=0, j=N-1;i<N;i++, j--){
		I[i]=1u<<j;
		B[i]=A[i];
	}
	
	for(uint i=0, j=N-1;i<N;i++, j--){
		if( !((B[i]>>j) &0x1))
		for(uint k=i+1;k<N;k++)
			if((B[k]>>(N-k-1))){
				B[i]^=B[k];
				I[i]^=I[k];
				break;
			}
			
		for(uint k=i+1;k<N;k++)
			if((B[k]>>j)&0x1){
				B[k]^=B[i];
				I[k]^=I[i];
			}
	}
	
	for(int i=N-1, j=0;i>-1;i--, j++)
		for(int k=i-1;k>-1;k--)
			if((B[k]>>j) &0x1){
				B[k]^=B[i];
				I[k]^=I[i];
			}
}

vector<uint> pow(vector<uint>& A, uint N){
	if(N==0){
		uint s=A.size();
		vector<uint> I(s);
		for(uint i=0, j=s-1;i<s;i++,j--) I[i]=1u<<j;
		return I;
	}
	if(N==1) return A;
	
	vector<uint> t=pow(A,N/2);
	return t*t*pow(A,N%2);
}

int X[1<<17];
uint key[1<<17];
uint len;

void clear(){
	uint i;
	for(i=0;i<len;i++){ key[i]=0; X[i]=-1; }
}

uint hash(uint k){
	uint p;
	p=k%len;
	while(X[p]!=-1 && key[p]!=k) p=(p+1)%len;
	return p;
}

void insert(uint k, uint v){
	int p;
	p=hash(k);
	key[p]=k;
	X[p]=v;
}

int main()
{
	uint N, T;
	uint mini;
	bool ok;
	uint y, x;
	uint s, t, k, m;
	
	for(;;){
		scanf("%u %u",&N,&T);
		if(!(N||T)) return 0;
		
		vector<uint> A(N), invA(N), D(N);
		
		for(uint i=0, j=N-1;i<N;i++,j--){ A[i]=0; D[i]=(1u<<j); }
		
		for(uint i=0;i<T;i++){
			scanf("%u",&k);
			A[0]|=(1u<<(k));
		}
		A[0]|=1u;
		
		scanf("%x %x",&s,&t);
		
		for(uint i=1,j=N-1;i<N;i++,j--) A[i]|=(1u<<j);
		
		//computa A^-1;
		inv(invA,A);
		
		m= 1<<( (N+1)/2);
		len=m<<1;
		clear();
		
		x=s;
		for(uint i=0;i<m;i++){
			if( X[(y=hash(x))] ==-1){ X[y]=i; key[y]=x; }
			if(x==t){
				mini=i;
				ok=true;
				goto L1;
			}
			x=A*x;
		}
		D=pow(invA,m);
		
		ok=false;
		y=t;
		for(uint i=0;i<m;i++){
			if( X[(x=hash(y))]!=-1){
				mini=i*m+X[x];
				ok=true;
				break;
			}
			y=D*y;
		}
		
		L1:
		if(ok){
			printf("%u\n",mini);
		}else printf("*\n");
	}
	return 0;
}



