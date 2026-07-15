#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include"include\grafopesato.h"

using namespace std;

int main() {
    GrafoPesato G;
    ifstream fin("19980101.as-rel.txt");
    string riga;
    string x;
    string y;

    while(getline(fin, riga)) { //qua creo il grafo settando i pesi a zero
        if(riga.empty() || riga.front() == '#') {
            continue;
        }
     
        stringstream ss(riga);
        getline(ss, x, '|');
        getline(ss, y, '|');
        G.new_edge(stoi(x), stoi(y), 0); 
    }

}




