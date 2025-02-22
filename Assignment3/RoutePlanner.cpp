#include "RoutePlanner.h"
#include <iostream>
#include <fstream>

const std::string cities[81] = {
    "Adana", "Adiyaman", "Afyon", "Agri", "Amasya", "Ankara", "Antalya", "Artvin", "Aydin", "Balikesir", "Bilecik", 
    "Bingol", "Bitlis", "Bolu", "Burdur", "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir", "Edirne", 
    "Elazig", "Erzincan", "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay", "Isparta", 
    "Mersin", "Istanbul", "Izmir", "Kars", "Kastamonu", "Kayseri", "Kirklareli", "Kirsehir", "Kocaeli", "Konya", "Kutahya", 
    "Malatya", "Manisa", "Kaharamanmaras", "Mardin", "Mugla", "Mus", "Nevsehir", "Nigde", "Ordu", "Rize", "Sakarya", 
    "Samsun", "Siirt", "Sinop", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Urfa", "Usak", "Van", "Yozgat", 
    "Zonguldak", "Aksaray", "Bayburt", "Karaman", "Kirikkale", "Batman", "Sirnak", "Bartin", "Ardahan", "Igdir", 
    "Yalova", "Karabuk", "Kilis", "Osmaniye", "Duzce" 
};

RoutePlanner::RoutePlanner(const std::string& distance_data, const std::string& priority_data, const std::string& restricted_data, int maxDistance)
    : maxDistance(maxDistance), totalDistanceCovered(0), numPriorityProvinces(0), numWeatherRestrictedProvinces(0) {
    map.loadDistanceData(distance_data);
    map.resetVisited();
    loadPriorityProvinces(priority_data);
    loadWeatherRestrictedProvinces(restricted_data);
}

void RoutePlanner::loadPriorityProvinces(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    std::string line;
    while (getline(file, line)) {
        size_t openParen = line.find('(');
        size_t closeParen = line.find(')');

        if (openParen != std::string::npos && closeParen != std::string::npos) {
            std::string indexStr = line.substr(openParen + 1, closeParen - openParen - 1);
            int provinceIndex = std::stoi(indexStr);
            priorityProvinces.push_back(provinceIndex);
            numPriorityProvinces++;
        }
    }
    file.close();
}


void RoutePlanner::loadWeatherRestrictedProvinces(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    std::string line;
    while (getline(file, line)) {
        size_t openParen = line.find('(');
        size_t closeParen = line.find(')');

        if (openParen != std::string::npos && closeParen != std::string::npos) {
            std::string indexStr = line.substr(openParen + 1, closeParen - openParen - 1);
            int provinceIndex = std::stoi(indexStr);
            weatherRestrictedProvinces.push_back(provinceIndex);
            numWeatherRestrictedProvinces++;

        }
    }
    file.close();
}
bool RoutePlanner::isPriorityProvince(int province) const {
    for (int priority : priorityProvinces) {
        if (priority == province) {
            return true;
        }
    }
    return false;
}


bool RoutePlanner::isWeatherRestricted(int province) const {
    for (int restricted : weatherRestrictedProvinces) {
        if (restricted == province) {
            return true;
        }
    }
    return false;
}

// Begins the route exploration from the starting point
void RoutePlanner::exploreRoute(int startingCity) {
    map.markAsVisited(startingCity);
    stack.push(startingCity);
    route.push_back(startingCity);
    currentTravelDistance = 0;

    while (!stack.isEmpty()) {
        int province = stack.peek();
        bool hasUnvisitedNeighbor = false;

        enqueueNeighbors(province);

        while (!queue.isEmpty()) {
            int nextProvince = queue.dequeue();

            if (!map.isVisited(nextProvince)) {
                int distanceToNext = map.getDistance(province, nextProvince);

                if (currentTravelDistance + distanceToNext > 250) {
                    currentTravelDistance = 0;
                }

                currentTravelDistance += distanceToNext;
                totalDistanceCovered += distanceToNext;
                map.markAsVisited(nextProvince);
                stack.push(nextProvince);
                route.push_back(nextProvince);
                hasUnvisitedNeighbor = true;
                break;
            }
        }

        if (!hasUnvisitedNeighbor) {
            stack.pop();
            if (!stack.isEmpty()) {
                int previousProvince = stack.peek();
                currentTravelDistance -= map.getDistance(previousProvince, province);
            }
        }
    }
}




void RoutePlanner::exploreFromProvince(int province) {
    enqueueNeighbors(province);

    while (!queue.isEmpty()) {
        int nextProvince = queue.dequeue();
        if (!map.isVisited(nextProvince)) {
            int distanceToNext = map.getDistance(province, nextProvince);
            if (currentTravelDistance + distanceToNext > 250) {
                //std::cout << "Yakıt yenileme gerekiyor. " << cities[province] << std::endl;
                currentTravelDistance = 0;
            }

            currentTravelDistance += distanceToNext;
            totalDistanceCovered += distanceToNext;
            map.markAsVisited(nextProvince);
            stack.push(nextProvince);
            route.push_back(nextProvince);
            exploreFromProvince(nextProvince);
        }
    }

    backtrack();
}


void RoutePlanner::enqueueNeighbors(int province) {
    for (int neighbor = 0; neighbor < 81; ++neighbor) {
        int distance = map.getDistance(province, neighbor);

        if (distance == -1 || distance > 250) {
            continue;
        }

        if (!map.isVisited(neighbor)) {
            if (isWeatherRestricted(neighbor)) {
                std::cout << "Province " << cities[neighbor] << " is weather-restricted. Skipping." << std::endl;
                continue;
            }

            if (isPriorityProvince(neighbor)) {
                queue.enqueuePriority(neighbor);
            } else {
                queue.enqueue(neighbor);
            }
        }
    }
}



void RoutePlanner::backtrack() {
    if (!stack.isEmpty()) {
        stack.pop();  //geri dön
    }
}

bool RoutePlanner::isExplorationComplete() const {
    return stack.isEmpty() && queue.isEmpty();
}


void RoutePlanner::displayResults() const {
    std::cout << "----------------------------\n";
    std::cout << "Journey Completed!\n";
    std::cout << "----------------------------\n";
    std::cout << "Total Number of Provinces Visited: " << map.countVisitedProvinces() << std::endl;
    std::cout << "Total Distance Covered: " << totalDistanceCovered << " km" << std::endl;

    std::cout << "Route Taken:\n";
    for (size_t i = 0; i < route.size(); ++i) {
        std::cout << cities[route[i]];
        if (i < route.size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << "\n";

    std::cout << "Priority Provinces Status:\n";
    int visitedCount = 0;
    for (int province : priorityProvinces) {
        bool visited = map.isVisited(province);
        std::cout << "- " << cities[province] << " " << (visited ? "(Visited)" : "(Not Visited)") << "\n";
        if (visited) {
            visitedCount++;
        }
    }

    std::cout << "Total Priority Provinces Visited: " << visitedCount << " out of " << numPriorityProvinces << "\n";

    if (visitedCount == numPriorityProvinces) {
        std::cout << "Success: All priority provinces were visited.\n";
    } else {
        std::cout << "Warning: Not all priority provinces were visited.\n";
    }
}
