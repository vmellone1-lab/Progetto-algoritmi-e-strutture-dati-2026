#include "grafopesato.h"
#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;
auto const INF = INT_MAX;

GrafoPesato::GrafoPesato() : V(0), E(0) {
    adj.resize(1);
}

GrafoPesato::GrafoPesato(int v, int e) : V(v), E(e) {
    if (V > MAXN) {
        cout << "Troppo grande!" << endl;
        exit(1);
    }
}

void GrafoPesato::V_set(int v) {
    V = v;
    if (V > MAXN) {
        cout << "Troppo grande!" << endl;
        exit(1);
    }
}

void GrafoPesato::E_set(int e) {
    E = e;
}

void GrafoPesato::new_edge(int x, int y, int w) {
    if(adj.size() <= x || adj.size() <= y ) {
        if(x > MAXN || y > MAXN) {
            cout << "troppo grande!" << endl ; 
            exit(1);
        }
        if(x < y) {
            adj.resize(y+1);
        }
        else {
            adj.resize(x+1);
        }
    }
    if(adj[x].empty()) V++;
    if(adj[y].empty()) V++;
    
    adj[x].push_back({y, w});
    adj[y].push_back({x, w});
    E++;
}

void GrafoPesato::stampa_adj() {
    for (int i = 1; i < adj.size(); i++) {
        if(adj[i].empty()) continue; 
        cout << i << ": ";
        for (auto edge : adj[i]) {
            auto u = edge.first;
            auto w = edge.second;
            cout << "(" << u << ", " << w << ") ";
        }
        cout << endl;
    }
}

vector<int> GrafoPesato::dijkstra(int source) {
    vector<int> dist(V, INF);
    dist[source] = 0;
    priority_queue<pair<int, int>> pq;
    pq.push({0, 0});
    vector<bool> visited(V);

    while(! pq.empty()) {
        auto v = (pq.top()).second;
        pq.pop();
        if (visited[v]) continue;
        visited[v] = true;
        for (auto edge : adj[v]) {
            auto u = edge.first;
            auto w = edge.second;
            if (dist[v] + w < dist[u]) {
                dist[u] = dist[v] + w;
                pq.push({-dist[u], u});
            }
        }
    }
    
    return dist;
}

void GrafoPesato::add_weight(int x, int y) {
    for(int i = 0; i < (adj[x]).size() ; i++ ) {
        if(adj[x][i].first == y){ 
            adj[x][i].second += 1;
            break;
        }
    }
    for(int i = 0; i < (adj[y]).size() ; i++ ) {
        if(adj[y][i].first == x){ 
            adj[y][i].second += 1;
            break;
        }
    }
}

int GrafoPesato::get_V() {
    return V;
}

int GrafoPesato::get_E() {
    return E;
}

vector<vector<pair<int,int>>> GrafoPesato::get_adj(){
    return adj;
}

vector<int> GrafoPesato::costi_minimax(int u) {
    vector<int> dist(adj.size(), INF);
    dist[u] = 0;
    priority_queue<pair<int,int>> pq;
    pq.push({0, u});
    vector<bool> visited(adj.size());

    while(! pq.empty()) {
        auto x = (pq.top()).second;
        pq.pop();
        if (visited[x]) continue;
        visited[x] = true;
        for (auto edge : adj[x]) {
            auto y = edge.first;
            if(visited[y]) continue;
            auto w = edge.second;
            int M;
            if(w < dist[x]) {
                M = dist[x];
            }
            else M = w;
            if(M < dist[y] ){
                dist[y] = M;
                pq.push({-dist[y], y});
            }
        }
    }
    return dist;
}