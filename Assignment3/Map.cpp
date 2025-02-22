#include "Map.h"
#include <fstream>
#include <iostream>
#include <sstream>
Map::Map() {
    for (int i = 0; i<MAX_SIZE; i++){
        for(int j=0; j<MAX_SIZE; j++){
            if(i == j){
                distanceMatrix[i][j] = 0;
            }
            else{
                distanceMatrix[i][j] = -1;
            }
        }
        visited[i] = false;
    }
}

// Loads distance data from a file and fills the distanceMatrix
void Map::loadDistanceData(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }
    std::string line;
    int row = 0;
    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        int col = 0;
        while (getline(ss, cell, ',')) {
            distanceMatrix[row][col] = std::stoi(cell);
            col++;
        }
        row++;
    }
    file.close();
}


bool Map::isWithinRange(int provinceA, int provinceB, int maxDistance) const {
    if (distanceMatrix[provinceA][provinceB] == -1){
    return false;}

    if(distanceMatrix[provinceA][provinceB] <= maxDistance){
        return true;
    }
    return false;
}

void Map::markAsVisited(int province) {
    visited[province] = true;
}

bool Map::isVisited(int province) const {
    return visited[province];
}

void Map::resetVisited() {
    for(int i=0; i<MAX_SIZE; i++){
        visited[i] = false;
    }
}

int Map::countVisitedProvinces() const {
    int count = 0;
    for(int i=0; i<MAX_SIZE; i++){
        if(visited[i] == true){
            count++;
        }
    }
    return count;
}

int Map::getDistance(int provinceA, int provinceB) const {
    return distanceMatrix[provinceA][provinceB];
}