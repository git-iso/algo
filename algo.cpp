#include <iostream>
#include <iterator>
#include <vector>
#include <cmath>
#include <limits>
#include <numeric>
#include <algorithm>

using namespace std;

vector<vector<float>> normalize(const vector<vector<float>>&matrix){

    int rows = matrix.size();
    int columns = matrix[0].size();
    vector<vector<float>> nMatrix(rows, vector<float>(columns));

    for (int j = 0; j < columns; j++){ // Find Euclidean norm for columns

        double nColumn = 0;

        for (auto row: matrix){

            nColumn += row[j] * row[j];

        }

        nColumn = sqrt(nColumn);

        for (int i = 0; i < rows; i++){

            nMatrix[i][j] = matrix[i][j] / nColumn;

        }

    }

    return nMatrix;

}

vector<vector<float>> normalizeAHP(const vector<vector<float>>&matrix){

    int rows = matrix.size();
    int columns = matrix[0].size();
   
    vector<vector<float>> nMatrix(rows, vector<float>(columns));

    for (int j = 0; j < columns; j++){ // Find Euclidean norm for columns

        double nColumn = 0;

        for (auto row: matrix){

            nColumn += row[j];

        }

        for (int i = 0; i < rows; i++){

            nMatrix[i][j] = matrix[i][j] / nColumn;

        }

    }

    return nMatrix;

}


vector<float> computeWeights(const vector<vector<float>>&matrix, const int &m){

    int rows = matrix.size();
    int columns = matrix[0].size();
    vector<float> wMatrix(columns);

    for (int i = 0; i < rows; i++){
        
        for (int j = 0; j < columns; j++){

            wMatrix[j] = matrix[i][j] / m;

        }
    }

    return wMatrix;


}

vector<vector<float>> weightMatrix(const vector<vector<float>>&matrix, const vector<float>&weights){

    int rows = matrix.size();
    int columns = matrix[0].size();
    vector<vector<float>> wMatrix(rows, vector<float>(columns));

    for (int i = 0; i < rows; i++){
        
        for (int j = 0; j < columns; j++){

            wMatrix[i][j] = matrix[i][j] * weights[j];

        }
    }

    return wMatrix;

}

vector<float> positiveIdeal(const vector<vector<float>>&matrix, const vector<float>&weights){

    int columns = matrix[0].size();
    vector<float> ideal(columns, numeric_limits<float>::min());

    for (auto row: matrix){

        for (int j = 0; j < columns; j++){

            ideal[j] = max(ideal[j],row[j]);

        }

    }

    return ideal;

}

vector<float> negativeIdeal(const vector<vector<float>>&matrix, const vector<float>&weights){

    int columns = matrix[0].size();
    vector<float> ideal(columns, numeric_limits<float>::max());

    for (auto row: matrix){

        for (int j = 0; j < columns; j++){

            ideal[j] = min(ideal[j],row[j]);

        }

    }

    return ideal;

}

vector<float> euclideanDistance(const vector<vector<float>>&matrix, const vector<float> &ideal){

    int rows = matrix.size();
    vector<float> distances(rows, 0);

    for (int i = 0; i < rows; i++){

        for (int j = 0; j < ideal.size(); j++){

            distances[i] += (matrix[i][j] - ideal[j]) * (matrix[i][j] - ideal[j]);

        }

        distances[i] = sqrt(distances[i]);
        
    }

    return distances;

}

vector<float> relativeProxmity(const vector<float> &eDistancePositiveIdeal, const vector<float> &eDistanceNegativeIdeal){

    int size = eDistancePositiveIdeal.size();
    vector<float> coefficents(size);

    for (int i = 0; i < size; i++){

        coefficents[i] = eDistanceNegativeIdeal[i] / (eDistancePositiveIdeal[i] + eDistanceNegativeIdeal[i]);

    }

    return coefficents;

}

int idealNetwork(const vector<float>&proximity){

    return distance(proximity.begin(), max_element(proximity.begin(), proximity.end()));

}

int main(){

    int n = 3; // # of networks
    int m = 6; // # of attributes

    // 1: Create decision matrix (rssi, dist, sinr, speed, load , bandwith)
    vector<vector<float>> dMatrix = {

        {9, 7, 1, 2, 5, 2},
        {11, 3, 5, 7, 0, 3},
        {6, 5, 4, 6, 15, 8},
       
    };

    // 2: Normalize decision matrix
    vector<vector<float>> nMatrix = normalize(dMatrix);

    // AHP
    // 3: Create pairwise comparison matrix from Saaty's scale (6 attributes = 6x6)
    vector<vector<float>> ahpMatrix = {

        {1, 3, 7, 4, 2, 9},
        {1/(float)3, 1, 6, 5, 3, 3},
        {1/(float)7, 1/(float)6, 1, 4, 8, 9},
        {1/(float)4, 1/(float)5, 1/(float)4, 1, 2 ,7 },
        {1/(float)2, 1/(float)3, 1/(float)8, 1/(float)2, 1, 2},
        {1/(float)9, 1/(float)3, 1/(float)9, 1/(float)7, 1/(float)2, 1}

    };

    // 4: Normalize pairwise matrix
    vector<vector<float>> ahpNMatrix = normalizeAHP(ahpMatrix);

    // 5: Compute weights
    vector<float> weights = computeWeights(ahpNMatrix, m);

    // TOPSIS
    // 6: Created weighted normalized decision matrix
    vector<vector<float>> wMatrix = weightMatrix(nMatrix, weights);

    // 7: Determine positive and negative ideal
    vector<float> pIdeal = positiveIdeal(wMatrix, weights);
    vector<float> nIdeal = negativeIdeal(wMatrix, weights);

    // 8: Compute Euclidean distance from network to positive/negative ideal
    vector<float> edPositiveIdeal = euclideanDistance(wMatrix, pIdeal);
    vector<float> edNegativeIdeal = euclideanDistance(wMatrix, nIdeal);
    
    // 9: Compute relative proximity to ideal
    vector<float> rProximity = relativeProxmity(edPositiveIdeal, edNegativeIdeal);

    // 10: Select ideal network
    int bestNetwork = idealNetwork(rProximity);

    // 11: Display proximities and best solution

    cout << "Relative proxmities: ";

    for (auto coeff: rProximity){

        cout << coeff << " ";

    }

    cout << endl;
    cout << "Best Network: " << bestNetwork + 1 << endl;

}