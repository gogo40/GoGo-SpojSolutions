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

Link: http://br.spoj.com/problems/MOEDAS/

Solução: Problema da mochila.
*/

#include <algorithm>
#include <iostream>
#include <cstring>

using namespace std;

int pval[50002];
int val, nmoedas;
int vmoedas[102];

int main(){
	
	while ((cin >> val) && val){

		cin >> nmoedas;
		memset(pval,0,sizeof(pval));

		for (int i = 0; i < nmoedas; i++){
			cin >> vmoedas[i];
			pval[vmoedas[i]] = 1;
		}

		for (int i = 1; i <= val; i++){
			if (pval[i])
				for (int a = 0; a < nmoedas; a++){
					if (i + vmoedas[a] > val) break;
					if (pval[i + vmoedas[a]] == 0) pval[i + vmoedas[a]] = pval[i] + 1;
					else pval[i + vmoedas[a]] = min(pval[i + vmoedas[a]],pval[i] + 1);
			}
		}

		if (pval[val]) cout << pval[val];
		else cout << "Impossivel";
		cout << "\n";

		//fill(&pval[0],&pval[val+1],0);
	 // fill(&vmoedas[0],&vmoedas[nmoedas],0);
	}

	return 0;
}


