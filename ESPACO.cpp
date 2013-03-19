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
Titulo do problema: A gerencia do espaco

Link: http://br.spoj.com/problems/ESPACO/

Solução: Estrutura de dados e gestão de recursos.
*/
#include <set>
#include <map>
#include <cstdio>
#include <cmath>
#include <string>
#include <list>

#define MEGA 0x400
#define GIGA 0x100000

using namespace std;

/*Estruturas de dados necessarias*/
typedef unsigned int uint;

static uint D, LD;
struct Disco{
	bool isf;
	uint tam, id;
	
	Disco(uint tam=0, uint id=0, bool isf=true)
	:tam(tam),id(id),isf(isf){}
};

typedef list<Disco>::iterator it;
typedef pair<uint,it> hd;
list<Disco> disco;
set<hd> vagas;
map<uint,it> pos;
map<string,it> file;

inline
void init_espaco(){
	vagas.clear(); disco.clear(); pos.clear(); file.clear();
}

inline
bool operator<(const it& a, const it& b){
	return a->id < b->id;
}

inline
void otimiza(){
	if(vagas.size()==0 || LD==0) return;
	if(vagas.size()==1){ 
		if(vagas.begin()->first==LD && vagas.begin()->second->id==D-LD)
			return;
	}
	
	vagas.clear();
	pos.clear();
	
	uint id;
	it i;
	
	id=0;
	for(i=disco.begin(); i!=disco.end();)
		if(i->isf){
			i=disco.erase(i);
		}else{
			i->id=id;
			id=id+i->tam;
			i++;
		}
		
	i=disco.insert(disco.end(),Disco(LD,D-LD));
	pos[D-LD]=i; 
	vagas.insert(hd(LD,i));
}

inline
void insere(const char* fname, uint T){
	
	set<hd>::iterator t=vagas.lower_bound(hd(T,disco.begin()));
	if(t==vagas.end()){
		otimiza();
		t=vagas.begin();
	}
	LD-=T;
	
	uint tam, id;
	it p;
	
	p=t->second;
	tam=p->tam; id=p->id;
	
	vagas.erase(t);
	pos.erase(id);
	
	if(T==tam){
		p->isf=false;
		file[fname]=p;
	}else{
		p->tam=tam-T; p->id=id+T; 
		pos[id+T]=p;
		
		vagas.insert(hd(tam-T,p));
		file[fname]=disco.insert(p,Disco(T,id,false));
	}
}

inline
void remover(const char* fname){
	map<string,it>::iterator t=file.find(fname);
	
	if(t!=file.end()){
		it p, ant, prox;
		
		p=t->second;
		LD+=p->tam;
		
		file.erase(t);
		
		p->isf=true;
		
		ant=prox=0;
		if(p!=disco.begin()){p--;ant=p;p++;}
		
		p++;
		if(p!=disco.end()) prox=p;
		p--;
		
		if(ant!=0)
			if(ant->isf){
				p->tam+=ant->tam;
				p->id=ant->id;
				
				vagas.erase(hd(ant->tam,ant));
				pos.erase(ant->id);
				disco.erase(ant);
			}
		
		if(prox!=0)
			if(prox->isf){
				p->tam+=prox->tam;
				
				vagas.erase(hd(prox->tam,prox));
				pos.erase(prox->id);
				disco.erase(prox);
			}
			
		pos[p->id]=p;
		vagas.insert(hd(p->tam,p));
	}
}

int main()
{
	uint N, T; 
	char c, cmd[20], fname[20];
	bool bc;
	it p; 
	
	while(scanf("%u\n",&N),N){	
		scanf("%u%cb\n",&D,&c);
		D*=(c=='G')?GIGA:(c=='M')?MEGA:1;
		bc=false;
		
		init_espaco();
		
		p=disco.insert(disco.begin(),Disco(D,0));
		pos[0]=p;
		vagas.insert(hd(D,p));
		
		LD=D;
		while(N--){
			scanf("%s",cmd);
			
			switch(cmd[0]){
				case 'i':
					scanf("%s %u%cb\n",fname,&T,&c);
					T*=(c=='G')?GIGA:(c=='M')?MEGA:1;
					if(LD>=T && !bc){
						insere(fname,T);
					}else bc=true;
				break;
				
				case 'r':
					scanf("%s\n",fname);
					if(!bc){
						remover(fname);
					}else bc=true;
				break;
				
				default:
					scanf("\n");
					if(!bc) otimiza();
					else bc=true;
			}
		}
		
		if(bc) printf("ERRO: disco cheio\n");
		else{
			uint dx, i, id, tam, te;
			uint esp[8], ini, fim;
			
			dx=D/8;
			for(i=0;i<8;i++) esp[i]=0;
			
			for(set<hd>::iterator t=vagas.begin(); t!= vagas.end(); t++){
				p=t->second; 
				id=p->id; tam=p->tam;
				ini=id/dx; fim=(id+tam)/dx;
				
				if(ini==fim) esp[ini]+=tam;
				else{
					te=dx-id%dx;
					esp[ini]+=te;
					te=tam-te;
					for(i=ini+1;i<fim;i++){ esp[i]+=dx; te-=dx;}
					esp[fim]+=te;
				}
			}
			
			for(i=0;i<8;i++)
				if(esp[i]*4>3*dx) printf("[ ]");
				else if(esp[i]*4>dx) printf("[-]");
				else printf("[#]");
			printf("\n");
		}
	}
	return 0;
}



