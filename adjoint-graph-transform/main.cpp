// Adjoint Graph Transformation

#include <iostream>
#include <cstdio>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>

using namespace std;

struct Vertex{
    int v;
    int out;
    vector<int> next;
};
struct Graph {
    int n; //number of vertices
    vector<Vertex> vertices;
};
struct ArcsList{
    // list of arcs created after transformation with info about the vertex of the adjoint graph from which they were created
    int n; //number of vertices
    int a; //number of arcs
    vector<pair<int, pair<int, int>>> arcs; // arcs = [ [v1, (x,y)], [v2, (x,y)], ... ] where v1: x -> y
};

Graph convertArcsToGraph(ArcsList H){
    //converts the graph H represented as a set of arcs into a vector of next
    Graph graph;
    graph.n = H.n;

    for(int i=0; i < H.n; i++) {
        Vertex ver;
        ver.v = i+1;
        ver.out = 0;
        ver.next.push_back(0);
        graph.vertices.push_back(ver);

        for (int j = 0; j < H.a; j++) {
            if (ver.v == H.arcs[j].second.first){
                if(graph.vertices[i].next[0] == 0) {
                    graph.vertices[i].next[0] = H.arcs[j].second.second;
                }
                else {
                    graph.vertices[i].next.push_back(H.arcs[j].second.second);
                }
                graph.vertices[i].out++;
            }
        }
        sort(graph.vertices[i].next.begin(), graph.vertices[i].next.end());
    }
    return graph;
}

Graph readFromTextFile(string path) {
    //returns graph read from a text file
    ifstream file(path);
    Graph graph;

    if (file.is_open()) {
        string v, next;
        int n, num = 0;

        while (!file.eof()) {

            getline(file, v);
            getline(file, next);

            Vertex vertex;
            vertex.v = stoi(v);
            n++;

            stringstream iss(next);
            while (iss >> num)
                vertex.next.push_back(num);

            if(vertex.next[0] != 0)
                vertex.out = vertex.next.size();
            else
                vertex.out = 0;

            graph.vertices.push_back(vertex);
        }
        graph.n = n;
        file.close();
    }
    else{
        printf("Opening file error!\n");
        graph.n = 0;
    }
    return graph;
}

void writeToTextFile(string path, Graph graph){
    //writes graph to a text file
    ofstream file;

    file.open(path);
    if(file.is_open()){

        for(int i=0; i < graph.n; i++) {
            file << graph.vertices[i].v << "\n";

            if(graph.vertices[i].next[0] != 0){
                for(int j=0; j < graph.vertices[i].out; j++)
                    file << graph.vertices[i].next[j] << " ";
          }
          else{
              file << 0;
          }
          if(i != graph.n-1)
              file << "\n";
        }
        file.close();
    }
    else {
        printf("Opening file error!\n");
    }
}

bool checkOneGraph(Graph G){
    for(int i=0; i < G.n; i++){
        for(int j=0; j < G.vertices[i].out-1; j++){
            if(G.vertices[i].next[j] == G.vertices[i].next[j+1])
                return false;
        }
    }
    return true;
}

bool checkAdjoint(Graph G){
    //checks if the graph is adjoint
    int count;

    if(checkOneGraph(G)) {
        for (int i = 0; i < G.n - 1; i++) {
            if (G.vertices[i].out != 0) {

                for (int j = i + 1; j < G.n; j++) {
                    count = 0;

                    for (int k = 0; k < G.vertices[i].out; k++) {
                        for (int l = 0; l < G.vertices[j].out; l++) {
                            if (G.vertices[i].next[k] == G.vertices[j].next[l])
                                count++;
                        }
                    }
                    if (count != 0 && (count != G.vertices[i].out || count != G.vertices[j].out))
                        return false;
                }
            }
        }
        return true;
    }
    return false;
}

bool checkLine(Graph H){
    //checks if the adjoint graph of H - G is line
    return checkOneGraph(H);
}

ArcsList transform(Graph G){
    //transforms adjoint graph G into its original graph H
    ArcsList H;
    H.a = G.n;
    int x = 1;

    for(int i=0; i < G.n; i++) {  //transforming vertices into arcs
        H.arcs.push_back(make_pair(i+1, make_pair(x, x+1)));
        x += 2;
    }

    int prev, next;

    for(int i=0; i < G.n; i++){  //creating connections between arcs
        next = H.arcs[i].second.second;

        for(int j=0; j < G.vertices[i].out; j++) {
            prev = H.arcs[G.vertices[i].next[j] - 1].second.first;

            for (int k = 0; k < H.a; k++) {
                if (H.arcs[k].second.first == prev)
                    H.arcs[k].second.first = next;
                if (H.arcs[k].second.second == prev)
                    H.arcs[k].second.second = next;
            }
        }
    }

    vector<int> all_H_ver, unique_H_ver;

    for(int i=0; i < H.a; i++){
        all_H_ver.push_back(H.arcs[i].second.first);
        all_H_ver.push_back(H.arcs[i].second.second);
    }
    sort(all_H_ver.begin(), all_H_ver.end());
    unique_H_ver.push_back(all_H_ver[0]);
    int n_H = 1;

    for(int i=0; i < all_H_ver.size()-1; i++){
        if(all_H_ver[i+1] != all_H_ver[i]) {
            unique_H_ver.push_back(all_H_ver[i + 1]);
            n_H++;
        }
    }
    H.n = n_H;

    for(int i=0; i < G.n; i++) { //reindexing vertices in arcs (first index = 1)
        for (int j = 0; j < n_H; j++) {
            if (H.arcs[i].second.first == unique_H_ver[j])
                H.arcs[i].second.first = j+1;
            if (H.arcs[i].second.second == unique_H_ver[j])
                H.arcs[i].second.second = j+1;
        }
    }

    return H;
}

void printGraph(Graph graph){
    //presents graph in a readable form
    for(int i=0; i < graph.n; i++){
        printf("%-2d -->  ", graph.vertices[i].v);
        if(graph.vertices[i].next[0] != 0){
            for(int j=0; j < graph.vertices[i].out; j++)
                printf("%d ", graph.vertices[i].next[j]);
        }
        printf("\n");
    }
}

int main() {
    string file_path;

    cout << "Enter the full path to the text file:" << endl;
    cin >> file_path;

    Graph G = readFromTextFile(file_path);

    if(G.n) {
        cout << "READ GRAPH:\n";
        printGraph(G);

        cout << "\nRESULTS:\n";
        if (checkAdjoint(G)) {
            cout << "  adjoint: TRUE\n";
            ArcsList aH = transform(G);
            Graph H = convertArcsToGraph(aH);

            if (checkLine(H))
                cout << "  line: TRUE\n";
            else
                cout << "  line: FALSE\n";

            cout << "\nORIGINAL GRAPH:\n";
            for (int i = 0; i < aH.arcs.size(); i++)
                printf("%-3d %d --> %d\n", aH.arcs[i].first, aH.arcs[i].second.first, aH.arcs[i].second.second);

            printf("\n");
            //printGraph(H);

            size_t lastDot = file_path.find_last_of(".");
            string write_path = file_path.substr(0, lastDot) + "_result.txt";
            writeToTextFile(write_path, H);

        } else {
            cout << "  adjoint: FALSE\n";
            cout << "  line: FALSE\n";
        }
    }

    return 0;
}
