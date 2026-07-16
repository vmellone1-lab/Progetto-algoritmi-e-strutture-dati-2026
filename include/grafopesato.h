#ifndef GRAFOPESATO_H
#define GRAFOPESATO_H

#pragma once

#include<iostream>
#include<vector>

using namespace std;

class GrafoPesato {
    public:
        GrafoPesato();
        GrafoPesato(int v, int e);
        void V_set(int v);
        void E_set(int e);
        void new_edge(int x, int y, int w);
        void add_weight(int x, int y);
        void stampa_adj();
        vector<int> dijkstra(int source);
        int get_V();
        int get_E();
        vector<vector<pair<int,int>>> get_adj();
        vector<int> costi_minimax(int u);
        GrafoPesato cc_massima();
    private:
        int V, E;
        static const int MAXN = INT16_MAX;
        vector<vector<pair<int, int>>> adj;
};

#endif