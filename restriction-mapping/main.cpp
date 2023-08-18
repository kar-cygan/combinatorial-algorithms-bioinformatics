// Restriction mapping

#include <iostream>
#include <cstdio>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> multiset, map;
int multiset_size, map_size;
int maxind; //max map index
int k; //number of cuts
int found; // solution found indicator


void generateInstances(int min, int max){
    srand(time(NULL));
    vector<int> cuts = {5,8,11,14};

    for(auto c : cuts){
        int num, sum, map_size = c+1;
        vector<int> map;

        for(int i=0; i<map_size; i++){
            num = rand() % (max-min+1) + min;
            map.push_back(num);
        }

        vector<int> multiset;
        for(int i=0; i < map_size-1; i++){
            sum = map[i];
            multiset.push_back(sum);
            for(int j=i+1; j < map_size; j++){
                sum += map[j];
                multiset.push_back(sum);
            }
        }
        multiset.push_back(map[map_size-1]);
        sort(multiset.begin(), multiset.end());

        cout << "\n\nMAP: ";
        for(auto m: map){
            cout << m << " ";
        }
        cout << "\nMULTISET: ";
        for(auto n: multiset){
            cout << n << " ";
        }
    }
}

int readInstance() {
    string file_path;

    cout << "Enter the full path to the text file:" << endl;
    cin >> file_path;

    ifstream file(file_path);

    if (file.is_open()) {
        while (!file.eof()) {
            string instance; int num;

            getline(file, instance);
            stringstream iss(instance);
            while (iss >> num)
                multiset.push_back(num);
        }
        multiset_size = multiset.size();
        file.close();
        return 1;
    }
    else{
        printf("Opening file error!\n");
        return 0;
    }
}

int findNumberOfCuts(){
    k = 0;
    while(2*multiset_size != (k+1)*(k+2))
        k++;

    return k;
}

bool checkCardinality(){
    int card = 0;
    int cuts = 1;
    while(card <= multiset_size){
        card = (cuts+2)*(cuts+1)/2;
        if(multiset_size == card)
            return true;
        cuts++;
    }
    return false;
}

int findElement(int element, vector<bool> &used){ 
    // find unused multiset element
    for(int i=0; i<multiset_size; i++) {
        if (element == multiset[i] && !used[i])
            return i;
    }
    return -1;
}

void search(int v, int ind, int *found){
    int index, sum;
    map[ind] = multiset[v];

    vector<bool> used(multiset_size);  //used multiset elements
    fill(used.begin(), used.end(), false);

    for(int i=0; i<=ind; i++) {    //all map elements set as used
        index = findElement(map[i], used);
        if(index != -1){
            used[index] = true;
            break;
        }
    }

    vector<int> sums;  //all possible sums of adjacent fragments
    for(int i=0; i < ind; i++){
        sum = map[i];
        for(int j=i+1; j <= ind; j++){
            sum += map[j];
            sums.push_back(sum);
        }
    }

    bool correct_sum = true;
    for(int i=sums.size()-1; i >= 0; i--){
        correct_sum = false;
        index = findElement(sums[i], used);
        if(index != -1) {
            used[index] = true;
            correct_sum = true;
        }
        if(!correct_sum)
            break;
    }

    if(correct_sum) {
        if (ind == maxind) {
            cout << "Found map:" << endl;
            for(auto num : map)
                cout << num << " ";
            cout << endl;
            *found = 1;
        }
        else {
            for(int i = 0; i < multiset_size; i++) {
                if(!used[i])
                    search(i, ind + 1, found);
                if (*found == 1)
                    break;
            }
        }
    }
}


int main(){

    //generateInstances(1,10);
    //generateInstances(1,35);

    int correct = readInstance();
    sort(multiset.begin(), multiset.end());

    if(correct && checkCardinality()) {
        clock_t begin, end;
        double time;

        k = findNumberOfCuts();
        map_size = k + 1;
        maxind = map_size - 1;
        map.resize(map_size);

        int first = multiset[multiset_size - 1] - multiset[multiset_size - 2];

        begin = clock();
        for (int i = 0; i < multiset_size; i++) {
            if (found == 0) {
                fill(map.begin(), map.end(), 0);
                map[0] = first; //pierwszy element mapy jest znany
                search(i, 1, &found);
            }
        }
        if (found == 0) {
            cout << "No solution - instance contains errors!" << endl;
        }
        end = clock();

        time = (double) (end - begin) / CLOCKS_PER_SEC;  //ms
        cout << "Runtime: ";
        if (time >= 10.00)
            printf("%.1lf s\n", time);
        else
            printf("%.3lf s\n", time);
    }
    else if (correct && !checkCardinality()){
        cout << "Incorrect multiset cardinality!" << endl;
    }

    return 0;
}
