#include<iostream>
#include<vector>
#include<climits>
#include<string>
#include<fstream>
#include<sstream>
#include<chrono>
#include"include\grafopesato.h"

using namespace std;
auto const INF = INT_MAX;

int main() {

    ofstream out("output.txt");
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
    vector<pair<int,vector<int>>> obs;

    while(getline(fin2, riga)) {
        stringstream ss2(riga);
        getline(ss2, y, ' '); //y contiene la prima parte che non mi serve
        getline(ss2, x, ' '); //così x contiene il cammino BGP
        stringstream sstemp(x);       
        vector<int> P;
        while(getline(sstemp, u, '|')) {
            P.push_back(stoi(u));
        }
        obs.push_back({0, P});
        for(int i = 0; i +1 < P.size(); i++ ) {
            G.add_weight(P[i], P[i+1]);
        }
    }
    for(int i = 0; i < obs.size(); i++) {
        obs[i].first = G.costo(obs[i].second);
    }

    auto T = G.cc_massima();
    G = T.first;
    auto hash = T.second;
    auto adj = G.get_adj();
    
    vector<int> freq(1);
    for(int i = 0; i < adj.size(); i++) {
        if(adj[i].empty()) continue;
        for(auto j : adj[i]) {            
            if(i < j.first) {
                if(freq.size() <= j.second) freq.resize(j.second + 1, 0);
                freq[j.second]++;
            }
        }
    }

    int scelta  = -1;

    while(scelta) {

        out.close();

        cout << "-----MENU-----"<< endl;
        cout << "1) Tabella di adiacenza con numero di archi e nodi" << endl;
        cout << "2) Costo del cammino minimax ottimo tra due nodi" << endl;
        cout << "3) Distribuzione delle frequenze sugli archi" << endl;
        cout << "4) Numero di cammini BGP osservati di costo minimax " << endl;
        cout << "5) Numero di cammini di costo minimax ottimo di un sottografo" << endl;
        cout << "6) Tempi di esecuzione degli algoritmi" << endl;
        cout << "0) Interrompere l'esecuzione del programma" << endl;

        cin >> scelta;

        out.open("output.txt", ios::trunc);
        
        switch (scelta) {
        case 1 : {
            out << "Il numero di nodi é: " << G.get_V() << endl;
            out << "Il numero di archi é: " << G.get_E() << endl;
            int s = 0;
            cout << "Serve anche la lista? " << endl;
            cin >> s;
            if(s) G.stampa_adj(hash.second, out);
            break;
        }
        case 2: {
            int u = 1;
            int v = 1;
            cout << "Inserire il nodo di partenza: " << endl;
            cin >> u;
            cout << "Inserire il nodo di arrivo: " << endl;
            cin >> v;
            if(hash.first[u] == -1) {
                cout << "Il nodo " << u << "non si trova nella componente connessa" << endl;
                break;
            }
            if(hash.first[v] == -1) {
                cout << "Il nodo " << v << "non si trova nella componente connessa" << endl;
                break;
            } 
            out << "Il costo minimax ottimo tra " << u << " e " << v << " é " << G.costi_minimax(hash.first[u])[hash.first[v]] << endl;
            break;
        }
        case 3: {
            for(int i = 1; i < freq.size(); i++) {
                out << i << ": " ;
                for(int j = 0; j < freq[i]; j++) {
                    out << "/" ; 
                }
                out << endl;
            }
            break;
        }
        case 4: {
            int u = 1;
            int v = 1;
            cout << "Inserire il nodo di partenza: " << endl;
            cin >> u;
            cout << "Inserire il nodo di arrivo: " << endl;
            cin >> v;
            if(hash.first[u] == -1) {
                cout << "Il nodo " << u << "non si trova nella componente connessa" << endl;
                break;
            }
            if(hash.first[v] == -1) {
                cout << "Il nodo " << v << "non si trova nella componente connessa" << endl;
                break;
            }
            u = hash.first[u];
            v = hash.first[v];
            vector<int> costi = G.costi_minimax(u);
            int contatore = 0;
            for(auto i : obs) {
                if(i.second[0] == u && i.second.back() == v && i.first == costi[v]) {
                    contatore++;
                }
            }
            out << "Sono stati osservati " << contatore << " cammini BGP di costo ottimo minimax: " << costi[v] << endl;
            break;
        }
        case 5: {
            int dim = 1;
            cout << "Inserire dimensione del sottografo (si consiglia non più di 100)" << endl;
            cin >> dim;
            GrafoPesato E = G;
            E.ridimensiona(dim);
            int u = 1;
            int v = 1;
            cout << "Inserire il nodo di partenza: " << endl;
            cin >> u;
            cout << "Inserire il nodo di arrivo: " << endl;
            cin >> v;
            if(hash.first[u] == -1 || hash.first[u] > dim) {
                cout << "Il nodo " << u << "non si trova nella componente connessa" << endl;
                break;
            }
            if(hash.first[v] == -1 || hash.first[v] > dim) {
                cout << "Il nodo " << v << "non si trova nella componente connessa" << endl;
                break;
            }
            int costo = E.costi_minimax(hash.first[u])[hash.first[v]];
            vector<bool> visitato(dim);
            cout << "Il numero di cammini minimax ottimi tra " << u << " e " << v << " é " << E.conta_minimax(hash.first[u], hash.first[v], visitato, costo)  << endl;
            break;
        }
        default: {
            scelta = 0;
            break;
        }
    }
    }
}
