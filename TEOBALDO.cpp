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
Titulo do problema: Teobaldo

Link: http://br.spoj.com/problems/TEOBALDO/

Solução: Contagem de rotas em grafos.
*/
#include <cstdio>
using namespace std;

const int MAX_V = 101;

int C, L, S, E, D;
int va[MAX_V], vd[MAX_V];// vetor anterior, vetor posterior (anterior e posterior ao produto matricial)
int g[MAX_V][MAX_V];//Matriz de adjac�ncia

//Zera toda a Matriz de adjacencia com 0
void Inic()
{
    for(int i=1; i<=C; i++){
   for(int j=i; j<=C; j++){
       g[i][j] = g[j][i] = 0;
   }
    }
}

bool Processamento()
{
    for(int c=1; c<=C; c++)
      va[c] = 0;

    va[S] = 1;

    for(int d=1; d<=D; d++){//itera D dias

   //Inicio - Produto Matricial
   for(int c=1; c<=C; c++){
       int soma = 0;
       for(int c2=1; c2<=C; c2++)
		if(va[c2]&&g[c2][c]){
		soma= 1;
		break;
       }

       vd[c] = soma;
   }
   //Fim - Produto Matricial

   //Prepata o vetor va[] para a pr�xima itera��o
   for(int c=1; c<=C; c++)
     va[c] = vd[c];
    }

    if( va[E]>0 ) return true;
    return false;
}

int main()
{
    for(;;){
   scanf("%d %d", &C, &L);
   if(C==0 && L==0) return 0;

   Inic();

   for(int l=1; l<=L; l++){
       int A, B;
       scanf("%d %d", &A, &B);
       
       g[A][B] = g[B][A] = 1;
   }

   scanf("%d %d %d", &S, &E, &D);

   bool ok = Processamento();

   if(ok) printf("Yes, Teobaldo can travel.\n");
   else printf("No, Teobaldo can not travel.\n");
    }
}


