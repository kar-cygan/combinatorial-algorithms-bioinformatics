// Motif Search

#include <iostream>
#include <cstdio>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <cmath>

using namespace std;

int INST = 5;  //number of sequences in an instance

struct Nucleobase{
    char base;
    int pos;
    int qual;
};

struct Sequence{
    int num;
    int len;
    string id;
    vector<Nucleobase> seq;
};

struct Vertex{
    string lmer;
    int seq;
    int pos;
    vector<Vertex> neighbors;
};

struct Graph{
    int n;
    vector<Vertex> vertices;
};

string generateMotif(int n){
    // Generates a random motif of length n
    srand(time(NULL));
    char bases[] = {'A', 'C', 'G', 'T'};

    string motif = "";
    for(int i=0; i<n; i++) {
        int x = rand() % 4;
        motif += bases[x];
    }
    return motif;
}

bool checkParameters(int threshold, int l){
    // Checks whether the parameters passed by the user are within the range

    if(threshold < 0 || threshold > 40){
        cout << "The quality threshold must be an integer between 0 and 40!\n" << endl;
        return false;
    }
    if(l < 4 || l > 9){
        cout << "Length of the substring must be an integer between 4 and 9!\n" << endl;
        return false;
    }
    return true;
}

vector <Sequence> readFromFiles(string fasta_path, string qual_path) {
    // Returns a vector of sequences read from FASTA and QUAL files
    vector<Sequence> sequences;
    ifstream fasta_file(fasta_path);
    ifstream qual_file(qual_path);

    if (fasta_file.is_open()) {
        int i = 1;

        while (!fasta_file.eof()) {
            Sequence seq;
            string f_line, id, len, chain;

            getline(fasta_file, f_line);
            istringstream iss(f_line);
            getline(iss, id, ' ');
            getline(iss, len, ' ');

            seq.id = id;
            seq.len = stoi((len.substr(len.find('=')+1)));
            seq.num = i;

            int lines = ceil(seq.len / 60.0);  //FASTA file has 60 chars per line
            while(lines){
                string buff;
                getline(fasta_file, buff);
                chain += buff;
                lines--;
            }

            for(int j=0; j<chain.length(); j++) {
                Nucleobase nuc;
                nuc.base = chain[j];
                nuc.pos = j+1;
                seq.seq.push_back(nuc);
            }

            sequences.push_back(seq);
            i++;
        }
        fasta_file.close();
    }
    else
        printf("Opening FASTA file error!\n");

    if (qual_file.is_open()) {
        int i = 0;

        while (!qual_file.eof()) {
            string f_line, quals;
            int num;

            getline(qual_file, f_line);
            int lines = ceil(sequences[i].len / 60.0); //60 values per line
            int j=0;
            while(lines) {
                getline(qual_file, quals);
                stringstream iss(quals);

                while (iss >> num){
                    sequences[i].seq[j].qual = num;
                    j++;
                }
                lines--;
            }
            i++;
        }
        qual_file.close();
    }
    else
        printf("Opening QUAL file error!\n");

    return sequences;
}

void checkQualityThreshold(int threshold, vector <Sequence> &seqs){
    // Removes from the sequence positions with quality below the threshold

    for(int i=0; i<INST; i++){
        vector <Nucleobase> new_seq;

        for(int j=0; j<seqs[i].len; j++){
            if(seqs[i].seq[j].qual >= threshold){
                new_seq.push_back(seqs[i].seq[j]);
            }
        }
        seqs[i].seq = new_seq;
    }
}

Graph createVertices(int l, vector <Sequence> &seqs){
    // Creates graph vertices from l-mers of instance sequence
    Graph graph;
    int n = 0;

    for(int i=0; i<INST; i++){
        for(int j=0; j < seqs[i].seq.size()-l+1; j++){
            Vertex v;
            n++;
            string subseq;
            auto it = seqs[i].seq.begin()+j;
            for (it; it != seqs[i].seq.begin()+j+l; ++it){
                subseq += it->base;
            }
            v.seq = seqs[i].num;
            v.lmer = subseq;
            v.pos = seqs[i].seq[j].pos;
            graph.vertices.push_back(v);
        }
    }
    graph.n = n;
    return graph;
}

void createEdges(int l, Graph &graph){
    // Connects the vertices of the graph by edges, if they correspond to the same subsequences 
    // in different sequences, and the difference in positions is no more than ten times the 
    // length of the subsequence (l)

    for(int i=0; i<graph.n-1; i++){
        for(int j=i+1; j<graph.n; j++){
            if(graph.vertices[i].seq != graph.vertices[j].seq
            && graph.vertices[i].lmer == graph.vertices[j].lmer
            && abs(graph.vertices[i].pos - graph.vertices[j].pos) <= 10 * l) {
                graph.vertices[i].neighbors.push_back(graph.vertices[j]);
                graph.vertices[j].neighbors.push_back(graph.vertices[i]);
            }
        }
    }
}

vector<Vertex> findClique(Graph &graph){
    // Returns a structure similar to a clique (star structure)
    vector<Vertex> clique;
    vector<bool> table(5);

    for(auto vertex : graph.vertices){
        if(vertex.neighbors.size() >= 4){
            clique.push_back(vertex);

            fill(table.begin(), table.end(), false);
            table[vertex.seq-1] = true;

            for(auto neighbor : vertex.neighbors){
                if(!table[neighbor.seq-1]){
                    table[neighbor.seq-1] = true;
                    clique.push_back(neighbor);
                }
            }

            if(clique.size() == 5)
                return clique;
            else{
                clique.clear();
                break;
            }
        }
    }
    return clique;
}

void showResult(vector<Vertex> &clique){
    if(!clique.empty()) {
        cout << "\nFound motif: " << clique[0].lmer << endl;
        for (auto v : clique) {
            cout << "sequence " << v.seq << " position " << v.pos << ": " << v.lmer << endl;
        }
    }
    else
        cout << "\nNo motif found for the given parameters!" << endl;
}

int main() {

    string file_path;
    cout << "Enter the full path to the FASTA instance file:" << endl;
    cin >> file_path;

    size_t lastDot = file_path.find_last_of(".");
    string fasta_path = file_path.substr(0, lastDot) + ".fasta";
    string qual_path = file_path.substr(0, lastDot) + ".qual";

    string t_param, l_param;
    bool is_correct = false;
    while(!is_correct){
        cout << "Input parameters:" << endl;
        cout << " Threshold [1-40]:"; cin >> t_param;
        cout << " Subsequence length [4-9]: "; cin >> l_param;

        is_correct = checkParameters(stoi(t_param), stoi(l_param));
    }
    vector<Sequence> seqs; Graph G; vector<Vertex> clique;
    int threshold = stoi(t_param);
    int l = stoi(l_param);

    seqs = readFromFiles(fasta_path, qual_path);
    checkQualityThreshold(threshold, seqs);
    G = createVertices(l, seqs);
    createEdges(l, G);
    clique = findClique(G);
    showResult(clique);

    return 0;
}
