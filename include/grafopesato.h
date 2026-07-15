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
        void change_weight(int x, int y, int w); //da implementare 
        void stampa_adj();
        vector<int> dijkstra(int source);

    private:
        int V, E;
        static const int MAXN = 10000;
        vector<vector<pair<int, int>>> adj;
};

#endif