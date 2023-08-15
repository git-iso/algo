#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <numeric>
#include <algorithm>
#include <bits/stdc++.h>
#include "ahp.cpp"
#include "topsis.cpp"

using namespace std;

int main(){

    AHP a; // AHP class
    TOPSIS t; // TOPSIS class
    ifstream file("networks.txt");
    string text; // Line from input files
    char c; // Char from input files
    int n;// # of networks
    int m; // # of attributes
    float num; // Entry value

    getline(file,text); // Get # of networks
    n = stoi(text);

    getline(file,text); // Get # of attributes
    m  = stoi(text);

    // 1: Create decision matrix (rssi, dist, sinr, speed, load , bandwith)
    vector<vector<float>> dMatrix(n, vector<float> (m));

    for (int i = 0; i < n; i++){
       
        for (int j = 0; j < m; j++){

            getline(file, text, ',');   
            num = stof(text);
            dMatrix[i].insert(dMatrix[i].begin() + j, num);
           

        }

    }

    dMatrix.resize(n);

    for (int i = 0; i < n; i++){

        dMatrix[i].resize(m);

    }

    file.close();

    // 2: Normalize decision matrix
    vector<vector<float>> nMatrix = a.normalize(dMatrix);

    // AHP
    // 3: Create pairwise comparison matrix from Saaty's scale (6 attributes = 6x6)
    ifstream file2("ahp.txt");
    vector<vector<float>> ahpMatrix(m, vector<float> (m));

    for (int i = 0; i < m; i++){
       
        for (int j = 0; j < m; j++){

            getline(file2, text, ',');   
            num = stof(text);
            ahpMatrix[i].insert(ahpMatrix[i].begin() + j, num);
           

        }

    }

    ahpMatrix.resize(m);

    for (int i = 0; i < m; i++){

        ahpMatrix[i].resize(m);

    }

    file2.close();

    // 4: Normalize pairwise matrix
    vector<vector<float>> ahpNMatrix = a.normalizeAHP(ahpMatrix);

    // 5: Compute weights
    vector<float> weights = a.computeWeights(ahpNMatrix, m);

    // TOPSIS
    // 6: Created weighted normalized decision matrix
    vector<vector<float>> wMatrix = t.weightMatrix(nMatrix, weights);

    // 7: Determine positive and negative ideal
    vector<float> pIdeal = t.positiveIdeal(wMatrix, weights);
    vector<float> nIdeal = t.negativeIdeal(wMatrix, weights);

    // 8: Compute Euclidean distance from network to positive/negative ideal
    vector<float> edPositiveIdeal = t.euclideanDistance(wMatrix, pIdeal);
    vector<float> edNegativeIdeal = t.euclideanDistance(wMatrix, nIdeal);
    
    // 9: Compute relative proximity to ideal
    vector<float> rProximity = t.relativeProxmity(edPositiveIdeal, edNegativeIdeal);

    // 10: Select ideal network
    int bestNetwork = t.idealNetwork(rProximity);

    // 11: Display proximities and best solution

    cout << "Relative proxmities: ";

    for (auto coeff: rProximity){

        cout << coeff << " ";

    }

    cout << endl;
    cout << "Best Network: " << bestNetwork + 1 << endl;

}