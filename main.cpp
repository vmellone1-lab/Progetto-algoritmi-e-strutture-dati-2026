#include<iostream>
#include<vector>
#include<climits>
#include<string>
#include<fstream>
#include<sstream>
#include"include\grafopesato.h"

using namespace std;
auto const INF = INT_MAX;

int main() {

    GrafoPesato G;
    ifstream fin1("19980101.as-rel.txt");
    string riga;
    string x;
    string y;

    while(getline(fin1, riga)) { //qua creo il grafo settando i pesi a zero
        if(riga.empty() || riga.front() == '#') {
            continue;
        }

        stringstream ss1(riga);
        getline(ss1, x, '|');
        getline(ss1, y, '|');
        G.new_edge(stoi(x), stoi(y), 0);
    }

    ifstream fin2("19980101.all-paths");
  
    if(! fin2.is_open()) {
        cout << "errore di apertura "  << endl;
    }

    string u;
    
    while(getline(fin2, riga)) {
        stringstream ss2(riga);
        getline(ss2, y, ' '); //y contiene la prima parte che non mi serve
        getline(ss2, x, ' '); //così x contiene il cammino BGP
        stringstream sstemp(x);       
        vector<int> P;
        while(getline(sstemp, u, '|')) {
            P.push_back(stoi(u));
        }
        for(int i = 0; i < P.size() - 1; i++ ) {
            G.add_weight(P[i], P[i+1]);
        }
    }

    auto T = G.cc_massima();
    G = T.first;
    auto hash = T.second;
}