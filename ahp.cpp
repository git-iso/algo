#include <iostream>
#include <iterator>
#include <vector>
#include <cmath>
#include <limits>
#include <numeric>
#include <algorithm>

using namespace std;

class AHP {

public:
    // Calculate Euclidean norm
    vector<vector<float>> normalize(const vector<vector<float>>&matrix){

        int rows = matrix.size();
        int columns = matrix[0].size();
        vector<vector<float>> nMatrix(rows, vector<float>(columns));

        for (int j = 0; j < columns; j++){ 

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

    //Pairwise AHP normalization
    vector<vector<float>> normalizeAHP(const vector<vector<float>>&matrix){

        int rows = matrix.size();
        int columns = matrix[0].size();
    
        vector<vector<float>> nMatrix(rows, vector<float>(columns));

        for (int j = 0; j < columns; j++){ // Find norm for columns

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

    //AHP weights calculation
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

};