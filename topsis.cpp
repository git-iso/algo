#include <iostream>
#include <iterator>
#include <vector>
#include <cmath>
#include <limits>
#include <numeric>
#include <algorithm>

using namespace std;

class TOPSIS {

public:

    //TOPSIS weights calculation
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

    //Calculate positive ideals
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

    //Calculate negative ideals
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

    //Find Euclidean Distance between positive and negative ideals
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

    //Find relativep proximityo of networks to positive and negative ideals
    vector<float> relativeProxmity(const vector<float> &eDistancePositiveIdeal, const vector<float> &eDistanceNegativeIdeal){

        int size = eDistancePositiveIdeal.size();
        vector<float> coefficents(size);

        for (int i = 0; i < size; i++){

            coefficents[i] = eDistanceNegativeIdeal[i] / (eDistancePositiveIdeal[i] + eDistanceNegativeIdeal[i]);

        }

        return coefficents;

    }

    //Find ideal network
    int idealNetwork(const vector<float>&proximity){

        return distance(proximity.begin(), max_element(proximity.begin(), proximity.end()));

    }
};