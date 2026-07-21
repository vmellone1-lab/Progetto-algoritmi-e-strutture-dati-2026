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
    ifstream fin2("19980101.all-paths");
    if(! fin1.is_open()) {
        cout << "errore di apertura "  << endl;
    }
    if(! fin2.is_open()) {
        cout << "errore di apertura "  << endl;
    }
    string riga;
    string x;
    string y;

    vector<pair<bool, double>> tempi(10, {0,0});
    auto inizio = chrono::high_resolution_clock::now();

    while(getline(fin1, riga)) { //qua creo il grafo settando i pesi a zero
        if(riga.empty() || riga.front() == '#') {
            continue;
        }

        stringstream ss1(riga);
        getline(ss1, x, '|');
        getline(ss1, y, '|');
        G.new_edge(stoi(x), stoi(y), 0);
    }

    string s;
    vector<pair<int,vector<int>>> obs;

    while(getline(fin2, riga)) {
        stringstream ss2(riga);
        getline(ss2, y, ' '); //y contiene la prima parte che non mi serve
        getline(ss2, x, ' '); //così x contiene il cammino BGP
        stringstream sstemp(x);       
        vector<int> P;
        while(getline(sstemp, s, '|')) {
            P.push_back(stoi(s));
        }
        obs.push_back({0, P});
        for(int i = 0; i +1 < P.size(); i++ ) {
            G.add_weight(P[i], P[i+1]);
        }
    }
    
    auto fine = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> durata = fine - inizio;
    tempi[0] = {1, durata.count()};
    
    for(int i = 0; i < obs.size(); i++) {
        obs[i].first = G.costo(obs[i].second);
    }


    inizio = chrono::high_resolution_clock::now();
    auto T = G.cc_massima();
    fine = chrono::high_resolution_clock::now();
    durata = fine - inizio;
    tempi[1] = {1, durata.count()};

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
            inizio = chrono::high_resolution_clock::now(); 
            out << "Il costo minimax ottimo tra " << u << " e " << v << " é " << G.costi_minimax(hash.first[u])[hash.first[v]] << endl;
            fine = chrono::high_resolution_clock::now();
            durata = fine - inizio;
            tempi[2] = {1, durata.count()};
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
            cout << "Inserire dimensione del sottografo" << endl;
            cin >> dim;
            GrafoPesato E = G;
            E.ridimensiona(dim);
            int u = 1;
            int v = 1;
            cout << "Inserire il nodo di partenza: " << endl;
            cin >> u;
            cout << "Inserire il nodo di arrivo: " << endl;
            cin >> v;
            if(u > dim) {
                cout << "Il nodo " << u << "non si trova nel sottografo" << endl;
                break;
            }
            if(v > dim) {
                cout << "Il nodo " << v << "non si trova nel sottografo" << endl;
                break;
            }
            int costo = E.costi_minimax(u)[v];
            vector<bool> visitato(dim);
            inizio = chrono::high_resolution_clock::now();
            out << "Il numero di cammini minimax ottimi tra " << u << " e " << v << " é " << E.conta_minimax(u, v, visitato, costo)  << endl;
            fine = chrono::high_resolution_clock::now();
            durata = fine - inizio;
            tempi[3] = {1, durata.count()};
            break;
        }
        case 6: {
            out << "Tempi di esecuzione: " << endl;
            out << "Tempo di lettura del grafo: " ;
            if(tempi[0].first == 0) out << "non eseguito" << endl;
            else out << tempi[0].second << " millisecondi" << endl;
            out <<  "Tempo di costruzione componente connessa massima: ";
            if(tempi[1].first == 0) out << "non eseguito" << endl;
            else out << tempi[1].second << " millisecondi" << endl;
            out << "Tempo di ricerca dell'ultimo costo minimax: ";
            if(tempi[2].first == 0) out << "non eseguito" << endl;
            else out << tempi[2].second << " millisecondi" << endl;
            out << "Tempo di ricerca ultimo numero di cammini minimax: ";
            if(tempi[3].first == 0) out << "non eseguito" << endl;
            else out << tempi[3].second << " millisecondi" << endl;
            break;
        }
        default: {
            scelta = 0;
            break;
        }
    }
    }
}
