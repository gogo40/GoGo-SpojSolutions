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
Titulo do problema: JUKEBOX

Link: http://br.spoj.com/problems/JUKEBOX/

Solução: KMP, HASHING e BUSCA EXAUSTIVA.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INF 1000000

typedef struct{
	char musica[31], artista[31];
	int len_m, len_a, i_a;
}Playlist;

char id[13][31];
int marc[13];
int key[13];
int tam;

int hash(const char* v, int len){
	int p, i, pv;
	pv=0;
	for(i=0;i<len;i++) pv+=(v[i]);
	pv%=13;
	p=pv;
	for(;;){
		if(key[p]==-1){
			strcpy(id[p],v);
			key[p]=pv;
			marc[p]=tam;
			tam++;
			return p;
		}else if(key[p]==pv){
			if(!strcmp(id[p],v)) return p;
		}
		p=(p+1)%13;
	}
}


int lengsm[30][450];
char gsm[30][450][31];
int ngsm[30];

int lengsa[30][450];
char gsa[30][450][31];
int ngsa[30];

int  libA[13];
int ans, n;
Playlist P[31];
char match[31];
/*1  custo deu usar o artista se ele for permitido
    0 custo deu n�o usar o artista se ele for permitido
*/
int calc[31][2], A;

int sum0[13];
int diff[13];

int kmpNext[32];

void prekmp(char *x, int m) {
   int i, j;

   i = 0;
   j = kmpNext[0] = -1;
   while (i < m) {
      while (j > -1 && x[i] != x[j])
         j = kmpNext[j];
      i++;
      j++;
      if (x[i] == x[j])
         kmpNext[i] = kmpNext[j];
      else
         kmpNext[i] = j;
   }
}

int kmp(char *y, int n, char* x, int m) {
   int i, j;

   if(n<m) return 0;
   i = j = 0;
   while (j < n) {
      while (i > -1 && x[i] != y[j])
         i = kmpNext[i];
      i++; j++;
      if (i >= m) {
         return 1;
         i = kmpNext[i];
      }
   }
   
   return 0;
}
/*Verifica que golden strings s�o uteis de verdade com base nos artistas liberados*/
void find_min()
{
	int i, j, k, len;
	int lmin, sum, s;
	
	for(i=0;i<tam;i++) sum0[i]=0, diff[i]=INF;
	
	A=0;
	
	for(i=0;i<n;i++){
		lmin=INF;
		for(j=0;j<ngsm[i];j++){
			len=lengsm[i][j];
			
			prekmp(gsm[i][j],len);
			for(k=0;k<n;k++){
				if(k!=i){
					if(libA[P[k].i_a])
					if(kmp(P[k].artista,P[k].len_a,gsm[i][j],len)) break;
				}
			}
			if(k==n) break;
		}
		if(j<ngsm[i]) lmin=len;
		calc[i][0]=lmin;
		
		if(libA[P[i].i_a]){
			for(j=0;j<ngsa[i];j++){
				len=lengsa[i][j];
				
				prekmp(gsa[i][j],len);
				for(k=0;k<n;k++){
					if(k!=i){
						if(libA[P[k].i_a] && P[k].i_a!=P[i].i_a)
						if(kmp(P[k].artista,P[k].len_a,gsa[i][j],len)) break;
					}
				}
				if(k==n) break;
			}
			if(j<ngsa[i])
				if(lmin>len) lmin=len;
		}
		A+=lmin;
		if(lmin==INF) break;
		if(A>=ans) break;
		calc[i][1]=lmin;
		sum0[P[i].i_a]+=calc[i][0];
		if(calc[i][1]-calc[i][0]<diff[P[i].i_a]) diff[P[i].i_a]=calc[i][1]-calc[i][0];	
	}
	
	if(i==n){
		sum=0;
		for(i=0;i<tam;i++){
			if(libA[i]){
				s=sum0[i]+diff[i];
				if(s<INF) sum+=s;
				else{  sum=INF; break;}
			}else{
				s=sum0[i];
				if(s<INF) sum+=s;
				else {sum=INF; break;}
			}
			if(sum>=ans) break;
		}	
		if(sum<ans){ ans=sum; }		
	}
}

/*Testa todas configura��es de artistas liberados*/
void bckt(int io)
{
	find_min();
	for(;io<tam;io++){
		libA[io]=1;
		bckt(io+1);
		libA[io]=0;
	}
	
}

/*Encontra todas golden strings*/
void find_gs()
{
	int i, len, j, lim, p, l, k;
	
	for(i=0;i<n;i++){
		
		for(len=1;len<=P[i].len_m;len++){
			lim=P[i].len_m-len+1;
			for(j=0;j<lim;j++){
				for(p=0,l=j;p<len;p++, l++)
					match[p]=P[i].musica[l];
				match[p]='\0';
				
				prekmp(match,len);
				for(k=0;k<n;k++){
					if(k!=i){
						if(kmp(P[k].musica,P[k].len_m,match,len)) break;
					}
				}
				
				if(k==n){
					strcpy(gsm[i][ngsm[i]],match);
					lengsm[i][ngsm[i]]=len;
					ngsm[i]++;
				}
			}
		}
		
		for(len=1;len<=P[i].len_a;len++){
			lim=P[i].len_a-len+1;
			for(j=0;j<lim;j++){
				for(p=0,l=j;p<len;p++, l++)
					match[p]=P[i].artista[l];
				match[p]='\0';
				
				prekmp(match,len);
				for(k=0;k<n;k++){
					if(k!=i){
						if(kmp(P[k].musica,P[k].len_m,match,len)) break;
					}
				}
				
				if(k==n){
					strcpy(gsa[i][ngsa[i]],match);
					lengsa[i][ngsa[i]]=len;
					ngsa[i]++;
				}
			}
		}
		
	}
}

int main()
{
	int i;
	
	while(scanf("%d\n",&n),n){
		tam=0;
		for(i=0;i<13;i++) { libA[i]=0; key[i]=id[i][0]=-1; }
		for(i=0;i<n;i++){
			scanf("%s\n",P[i].musica); P[i].len_m=strlen(P[i].musica);
			scanf("%s\n",P[i].artista); P[i].len_a=strlen(P[i].artista);
			P[i].i_a=marc[hash(P[i].artista,P[i].len_a)];
			ngsa[i]=ngsm[i]=0;
		}
		ans=INF;
		find_gs();
		bckt(0);
		printf("%d\n",ans);
	}

	return 0;
}




