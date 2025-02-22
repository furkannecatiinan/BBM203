#include "RealmShapers.h"
#include <cmath>
#include <algorithm>
#include <queue>


ShaperTree::ShaperTree()
{
}

ShaperTree::~ShaperTree() {
    for (RealmShaper* shaper : realmShapers) {
        delete shaper;
    }
    realmShapers.clear();
}


void ShaperTree::initializeTree(std::vector<RealmShaper*> shapers) {
    for (RealmShaper* shaper : shapers) {
        insert(shaper);
    }
}


int ShaperTree::getSize() {
    return realmShapers.size();
}


std::vector<RealmShaper *> ShaperTree::getTree()
{
    return realmShapers;
}

bool ShaperTree::isValidIndex(int index)
{
    // Dizinin geçerli olup olmadığını kontrol et
    // Geçerli ise true, değilse false döndür
    return (index >= 0 && index < realmShapers.size());
}


void ShaperTree::insert(RealmShaper* shaper) {
    realmShapers.push_back(shaper);
}


int ShaperTree::remove(RealmShaper* shaper) {
    int index = findIndex(shaper);

    if (index == -1) {
        std::cerr << "[Remove] Player not found: " << shaper->getName() << std::endl;
        return -1;
    }

    std::string shaperName = shaper->getName();

    // Önce o index'teki shaper'ı delete et
    delete realmShapers[index];

    // Elemanları sola kaydır
    for (int i = index; i < (int)realmShapers.size() - 1; i++) {
        realmShapers[i] = realmShapers[i + 1];
    }

    // Son elemanı çıkart
    realmShapers.pop_back();

    std::cout << "[Remove] " << shaperName << " removed from tree" << std::endl;
    return index;
}



int ShaperTree::findIndex(RealmShaper *shaper) {
    for (size_t i = 0; i < realmShapers.size(); ++i) {
        if (*realmShapers[i] == *shaper) {
            return i;
        }
    }
    return -1; // Bulunamadıysa
}


int ShaperTree::getDepth(RealmShaper *shaper) {
    int index = findIndex(shaper);
    if (index == -1) return -1; // Bulunamadıysa
    return static_cast<int>(std::floor(std::log2(index + 1)));
}


int ShaperTree::getDepth() {
    if (realmShapers.empty()) return 0;
    return static_cast<int>(std::floor(std::log2(realmShapers.size())));
}


RealmShaper* ShaperTree::duel(RealmShaper* challenger, bool result) {
    int challengerIndex = findIndex(challenger);
    int parentIndex = (challengerIndex - 1) / 2; // Binary tree parent index

    if (parentIndex < 0 || parentIndex >= realmShapers.size()) {
        std::cerr << "[Duel] Invalid parent index for challenger: " << challenger->getName() << std::endl;
        return nullptr;
    }

    RealmShaper* parent = realmShapers[parentIndex];
    RealmShaper* winner = result ? challenger : parent;
    RealmShaper* loser = result ? parent : challenger;

    // Honour Points Updates
    winner->gainHonour();
    loser->loseHonour();

    if (loser->getHonour() <= 0) {
        remove(loser);
        std::cout << "[Duel] " << loser->getName() << " lost all honour, deleted from tree" << std::endl;
    }

    if (result) {
        std::swap(realmShapers[challengerIndex], realmShapers[parentIndex]);
        std::cout << "[Duel] " << challenger->getName() << " won the duel against " << parent->getName() << std::endl;
    }

    return winner;
}


RealmShaper* ShaperTree::getParent(RealmShaper *shaper) {
    int index = findIndex(shaper);
    if (index <= 0) return nullptr; // Kök düğümün ebeveyni yok
    int parentIndex = (index - 1) / 2;
    return isValidIndex(parentIndex) ? realmShapers[parentIndex] : nullptr;
}


void ShaperTree::replace(RealmShaper *player_low, RealmShaper *player_high) {
    int lowIndex = findIndex(player_low);
    int highIndex = findIndex(player_high);

    if (lowIndex == -1 || highIndex == -1) {
        std::cerr << "[Replace] Player(s) not found!" << std::endl;
        return;
    }

    std::swap(realmShapers[lowIndex], realmShapers[highIndex]);
}


RealmShaper* ShaperTree::findPlayer(RealmShaper shaper) {
    for (RealmShaper* player : realmShapers) {
        if (*player == shaper) {
            return player;
        }
    }
    return nullptr; // Bulunamadıysa
}


// Find shaper by name
RealmShaper* ShaperTree::findPlayer(std::string name) {
    for (RealmShaper* player : realmShapers) {
        if (player->getName() == name) {
            return player;
        }
    }
    return nullptr; // Bulunamadıysa
}


void ShaperTree::inOrderTraversalHelper(int index, std::vector<std::string> &result) {
    if (!isValidIndex(index)) return;

    int left = 2 * index + 1;
    int right = 2 * index + 2;

    inOrderTraversalHelper(left, result);
    result.push_back(realmShapers[index]->getName());
    inOrderTraversalHelper(right, result);
}

std::vector<std::string> ShaperTree::inOrderTraversal(int index) {
    std::vector<std::string> result;
    inOrderTraversalHelper(index, result);
    return result;
}


std::vector<std::string> ShaperTree::preOrderTraversal(int index) {
    std::vector<std::string> result;

    if (!isValidIndex(index)) return result;

    result.push_back(realmShapers[index]->getName());
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    std::vector<std::string> leftTraversal = preOrderTraversal(left);
    std::vector<std::string> rightTraversal = preOrderTraversal(right);

    result.insert(result.end(), leftTraversal.begin(), leftTraversal.end());
    result.insert(result.end(), rightTraversal.begin(), rightTraversal.end());

    return result;
}



void ShaperTree::postOrderTraversalHelper(int index, std::vector<std::string> &result) {
    if (!isValidIndex(index)) return;

    int left = 2 * index + 1;
    int right = 2 * index + 2;

    postOrderTraversalHelper(left, result);
    postOrderTraversalHelper(right, result);
    result.push_back(realmShapers[index]->getName());
}

std::vector<std::string> ShaperTree::postOrderTraversal(int index) {
    std::vector<std::string> result;
    postOrderTraversalHelper(index, result);
    return result;
}


void ShaperTree::preOrderTraversal(int index, std::ofstream &outFile)
{
    if (!outFile.is_open()) {
        std::cerr << "[Error] Output file is not open." << std::endl;
        return;
    }

    if (!isValidIndex(index)) {
        return;
    }

    // Mevcut düğümün adını dosyaya yaz
    outFile << realmShapers[index]->getName() << std::endl;

    preOrderTraversal(2 * index + 1, outFile); // Sol alt düğüm
    preOrderTraversal(2 * index + 2, outFile); // Sağ alt düğüm
}

void ShaperTree::breadthFirstTraversal(std::ofstream &outFile) {
    if (realmShapers.empty()) return;

    std::queue<int> queue;
    queue.push(0);

    while (!queue.empty()) {
        int index = queue.front();
        queue.pop();

        outFile << realmShapers[index]->getName() << std::endl;

        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (isValidIndex(left)) queue.push(left);
        if (isValidIndex(right)) queue.push(right);
    }
}


void ShaperTree::displayTree()
{
    std::cout << "[Shaper Tree]" << std::endl;
    printTree(0, 0, "");
}

// Helper function to print tree with indentation
void ShaperTree::printTree(int index, int level, const std::string &prefix)
{
    if (!isValidIndex(index))
        return;

    std::cout << prefix << (level > 0 ? "   └---- " : "") << *realmShapers[index] << std::endl;

    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (isValidIndex(left) || isValidIndex(right))
    {
        printTree(left, level + 1, prefix + (level > 0 ? "   │   " : ""));
        printTree(right, level + 1, prefix + (level > 0 ? "   │   " : ""));
    }
}


void ShaperTree::writeShapersToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "[Error] Cannot open file: " << filename << std::endl;
        return;
    }

    for (RealmShaper* shaper : realmShapers) {
        file << shaper->getName() << std::endl;
    }

    file.close();
    std::cout << "[Output] Shapers have been written to " << filename << " according to rankings." << std::endl;
}
void ShaperTree::writeToFile(const std::string &filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "[Error] Cannot open file: " << filename << std::endl;
        return;
    }

    std::vector<std::string> traversalResult = preOrderTraversal(0);
    for (const std::string &name : traversalResult) {
        file << name << std::endl;
        std::cout << "[Debug] Writing to file: " << name << std::endl;
    }

    file.close();
    std::cout << "[Output] Tree has been written to " << filename << " in pre-order." << std::endl;
}
