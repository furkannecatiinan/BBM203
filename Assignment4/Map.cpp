#include "Map.h"

Map::Map()
{
    this->root = nullptr;
}
Map::~Map()
{
    std::queue<MapNode*> nodes;
    if (root) {
        nodes.push(root);
    }

    while (!nodes.empty()) {
        MapNode* current = nodes.front();
        nodes.pop();

        if (current->left) {
            nodes.push(current->left);
        }
        if (current->right) {
            nodes.push(current->right);
        }

        delete current->isle;
        delete current;
    }
}


void Map::initializeMap(std::vector<Isle *> isles)
{
    for (auto isle : isles) {
        insert(isle);
    }
    populateWithItems();
}

MapNode* Map::rotateRight(MapNode* current) {
    if (!current || !current->left) {
        std::cerr << "[Right Rotation] Called on invalid node!" << std::endl;
        return current;
    }

    MapNode* newRoot = current->left;
    current->left = newRoot->right;
    newRoot->right = current;


    current->height = std::max(height(current->left), height(current->right)) + 1;
    newRoot->height = std::max(height(newRoot->left), height(newRoot->right)) + 1;

    return newRoot;
}

MapNode* Map::rotateLeft(MapNode* current) {
    if (!current || !current->right) {
        std::cerr << "[Left Rotation] Called on invalid node!" << std::endl;
        return current;
    }

    MapNode* newRoot = current->right;
    current->right = newRoot->left;
    newRoot->left = current;


    current->height = std::max(height(current->left), height(current->right)) + 1;
    newRoot->height = std::max(height(newRoot->left), height(newRoot->right)) + 1;

    return newRoot;
}



int Map::calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth) {
    if (totalShaperTreeHeight == 0) {
        std::cerr << "[Error] Division by zero in calculateMinMapDepthAccess!" << std::endl;
        return totalMapDepth;
    }
    return (playerDepth * totalMapDepth) / totalShaperTreeHeight;
}



int Map::height(MapNode* node) {
    if (!node) return 0;
    return node->height;
}


MapNode* Map::insert(MapNode* node, Isle* isle) {
    if (!node) {
        MapNode* newNode = new MapNode(isle);
        //std::cout << "[DEBUG] Inserting new Isle: " << isle->getName() << std::endl;
        return newNode;
    }

    if (*isle < *node->isle) {
        //std::cout << "[DEBUG] Traverse Left: Current Node = " << node->isle->getName()
                 // << ", Inserting = " << isle->getName() << std::endl;
        node->left = insert(node->left, isle);
    } else if (*isle > *node->isle) {
        //std::cout << "[DEBUG] Traverse Right: Current Node = " << node->isle->getName()
                //  << ", Inserting = " << isle->getName() << std::endl;
        node->right = insert(node->right, isle);
    }

    node->height = std::max(height(node->left), height(node->right)) + 1;
    return balance(node);
}


void Map::insert(Isle *isle)
{
    root = insert((root), isle);

    // you might need to insert some checks / functions here depending on your implementation
}

// Soldaki en sağdaki düğümü bulan fonksiyon
MapNode* Map::findMax(MapNode* node) {
    while (node->right != nullptr) {
        node = node->right;
    }
    return node;
}



MapNode* Map::remove(MapNode* node, Isle* isle) {
    if (!node) return nullptr;

    // BST mantığıyla düğümü bulma
    if (*isle < *node->isle) {
        node->left = remove(node->left, isle);
    } else if (*isle > *node->isle) {
        node->right = remove(node->right, isle);
    } else {
        // Silinecek düğüm bulundu
        if (!node->left || !node->right) {

            MapNode* temp = node->left ? node->left : node->right;
            delete node;
            return temp;
        }

        // İki çocuk varsa:
        // Artık sağdaki min yerine soldaki max bulunacak
        MapNode* temp = findMax(node->left);

        *(node->isle) = *(temp->isle);

        // Eski veriyi içeren düğümü sol alt ağaçtan sil
        node->left = remove(node->left, temp->isle);
    }

    // Yükseklik güncelle
    node->height = std::max(height(node->left), height(node->right)) + 1;

    // AVL dengesini sağla
    return balance(node);
}

void Map::remove(Isle *isle) {
    root = remove(root, isle);
}




void Map::populateWithItems() {
    int count = 0;

    std::cout << "[DEBUG] Starting Pre-Order Item Distribution" << std::endl;
    preOrderItemDrop(root, count);
    std::cout << "[DEBUG] Completed Pre-Order Item Distribution" << std::endl;

    count = 0;
    std::cout << "[DEBUG] Starting Post-Order Item Distribution" << std::endl;
    postOrderItemDrop(root, count);
    std::cout << "[DEBUG] Completed Post-Order Item Distribution" << std::endl;

    std::cout << "[DEBUG] Starting BFS Item Distribution" << std::endl;
    if (root && root->isle->getItem() == EMPTY) {
        dropItemBFS();
    } else {
        std::cout << "[DEBUG] Root is not empty after pre/post distribution, no BFS performed." << std::endl;
    }
}

void Map::preOrderItemDrop(MapNode* current, int& count) {
    if (!current) {
        std::cout << "[DEBUG] Pre-Order Visit: NULL Node" << std::endl;
        return;
    }

    count++;
    std::cout << "[DEBUG] Pre-Order Visit: Node = " << current->isle->getName()
              << ", Current Item = " << current->isle->getItem()
              << ", Visit Count = " << count << std::endl;

    if (current->isle->getItem() == EMPTY && count % 5 == 0) {
        current->isle->setItem(EINSTEINIUM);
        std::cout << "[DEBUG] EINSTEINIUM dropped on Isle: "
                  << current->isle->getName() << std::endl;
    }

    preOrderItemDrop(current->left, count);
    preOrderItemDrop(current->right, count);
}

void Map::postOrderItemDrop(MapNode* current, int& count) {
    if (!current) {
        std::cout << "[DEBUG] Post-Order Visit: NULL Node" << std::endl;
        return;
    }

    postOrderItemDrop(current->left, count);
    postOrderItemDrop(current->right, count);

    count++;
    std::cout << "[DEBUG] Post-Order Visit: Node = " << current->isle->getName()
              << ", Current Item = " << current->isle->getItem()
              << ", Visit Count = " << count << std::endl;

    if (current->isle->getItem() == EMPTY && count % 3 == 0) {
        current->isle->setItem(GOLDIUM);
        std::cout << "[DEBUG] GOLDIUM dropped on Isle: "
                  << current->isle->getName() << std::endl;
    }
}

void Map::dropItemBFS() {
    if (!root) {
        //std::cout << "[DEBUG] BFS Drop: No eligible Isle found for AMAZONITE drop." << std::endl;
        return;
    }

    std::queue<MapNode*> queue;
    queue.push(root);
    int bfsOrder = 0;

    while (!queue.empty()) {
        MapNode* current = queue.front();
        queue.pop();

        bfsOrder++;
        //std::cout << "[DEBUG] BFS Visit: Node = " << current->isle->getName()
            //      << ", Current Item = " << current->isle->getItem()
            //      << ", BFS Order = " << bfsOrder << std::endl;

        if (current->isle->getItem() == EMPTY) {
            current->isle->setItem(AMAZONITE);
           // std::cout << "[DEBUG] AMAZONITE dropped on Isle: "
           //           << current->isle->getName() << std::endl;
            return;
        }

        if (current->left) queue.push(current->left);
        if (current->right) queue.push(current->right);
    }

  //  std::cout << "[DEBUG] BFS Drop: No eligible Isle found for AMAZONITE drop." << std::endl;
}





MapNode* Map::findFirstEmptyIsle(MapNode* node) {
    if (!node) return nullptr; // Eğer düğüm null ise nullptr döndür

    if (!node->isle->getItem()) { // Eğer ada boşsa, düğümü döndür
        return node;
    }

    MapNode* leftResult = findFirstEmptyIsle(node->left);
    if (leftResult) return leftResult;

    return findFirstEmptyIsle(node->right); // Sağ alt ağacı kontrol et
}




void Map::displayMap()
{
    std::cout << "[World Map]" << std::endl;
    display(root, 0, 0);
}

int Map::getDepth(MapNode* node)
{
    if (!root || !node) return -1;

    int depth = 0;
    MapNode* current = root;

    while (current) {
        if (current == node) return depth;
        depth++;
        if (*node->isle < *current->isle) current = current->left;
        else current = current->right;
    }
    return -1; // Node bulunamadı
}


int Map::getIsleDepth(Isle* isle) {
    MapNode* node = findNode(isle->getName());
    return node ? getDepth(node) : -1;
}


int Map::getDepth() {
    return height(root) - 1;
}



Isle* Map::findIsle(Isle isle)
{
    MapNode* node = findNode(isle);
    return node ? node->isle : nullptr;
}


Isle* Map::findIsle(std::string name)
{
    MapNode* node = findNode(name);
    return node ? node->isle : nullptr;
}


MapNode* Map::findNode(Isle isle)
{
    MapNode* current = root;
    while (current) {
        if (*current->isle == isle) return current;
        if (isle < *current->isle) current = current->left;
        else current = current->right;
    }
    return nullptr;
}



MapNode* Map::findNode(std::string name)
{
    MapNode* current = root;
    while (current) {
        if (current->isle->getName() == name) return current;
        if (name < current->isle->getName()) current = current->left;
        else current = current->right;
    }
    return nullptr;
}


void Map::display(MapNode *current, int depth, int state)
{
    // SOURCE:

    if (current->left)
        display(current->left, depth + 1, 1);

    for (int i = 0; i < depth; i++)
        printf("     ");

    if (state == 1) // left
        printf("   ┌───");
    else if (state == 2) // right
        printf("   └───");

    std::cout << "[" << *current->isle << "] - (" << current->height << ")\n"<< std::endl;

    if (current->right)
        display(current->right, depth + 1, 2);
}

void Map::writeToFile(const std::string &filename) {
    if (!root) return;

    std::ofstream file(filename);
    if (!file) {
        std::cerr << "[Error] Cannot open file: " << filename << std::endl;
        return;
    }

    std::queue<MapNode*> queue;
    queue.push(root);

    while (!queue.empty()) {
        int levelSize = queue.size();

        for (int i = 0; i < levelSize; ++i) {
            MapNode* current = queue.front();
            queue.pop();

            if (current) {
                file << current->isle->getName() << " ";
                queue.push(current->left);
                queue.push(current->right);
            } else {
                file << "NULL ";
            }
        }
        file << "\n";
    }

    file.close();
    std::cout << "[Output] Tree has been written to " << filename << " in level-order format." << std::endl;
}



void Map::writeIslesToFile(const std::string &filename) {
    if (!root) {
        std::cerr << "[Error] Map tree is empty. Nothing to write." << std::endl;
        return;
    }

    std::vector<std::string> isleNames;
    collectIslesInOrder(root, isleNames);

    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "[Error] Cannot open file: " << filename << std::endl;
        return;
    }

    for (const auto &name : isleNames) {
        outFile << name << std::endl;
    }

    outFile.close();
    std::cout << "[Output] Isles have been written to " << filename << " in alphabetical order." << std::endl;
}


// Helper fonksiyon: Adaları inorder (alfabetik sıralı) olarak toplar
void Map::collectIslesInOrder(MapNode* node, std::vector<std::string>& isleNames) {
    if (!node) return;

    collectIslesInOrder(node->left, isleNames);   // Sol alt ağacı gez
    isleNames.push_back(node->isle->getName());  // Geçerli düğümün adını ekle
    collectIslesInOrder(node->right, isleNames); // Sağ alt ağacı gez
}


MapNode* Map::findMin(MapNode* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}


MapNode* Map::balance(MapNode* node) {
    logNodeDetails(node, "Before Balancing");
    int balanceFactor = height(node->left) - height(node->right);

    if (balanceFactor > 1) {
        if (height(node->left->left) >= height(node->left->right)) {
            logNodeDetails(node, "Performing Right Rotation (Left-Left Case)");
            node = rotateRight(node);
        } else {
            logNodeDetails(node, "Performing Left-Right Rotation");
            node->left = rotateLeft(node->left);
            node = rotateRight(node);
        }
    } else if (balanceFactor < -1) {
        if (height(node->right->right) >= height(node->right->left)) {
            logNodeDetails(node, "Performing Left Rotation (Right-Right Case)");
            node = rotateLeft(node);
        } else {
            logNodeDetails(node, "Performing Right-Left Rotation");
            node->right = rotateRight(node->right);
            node = rotateLeft(node);
        }
    }

    // Root düğüm kontrolü
    if (node == root) {
        root = node;
        logNodeDetails(root, "Root Updated After Balancing");
    }

    logNodeDetails(node, "Balancing Complete");
    return node;
}



void Map::logNodeDetails(MapNode* node, const std::string& message) {
    /*
     if (!node) {
        std::cout << "[LOG] " << message << ": Node is NULL" << std::endl;
        return;
    }
    std::cout << "[LOG] " << message << ": "
              << "Node = " << node->isle->getName()
              << ", Left = " << (node->left ? node->left->isle->getName() : "NULL")
              << ", Right = " << (node->right ? node->right->isle->getName() : "NULL")
              << ", Height = " << node->height
              << ", BalanceFactor = " << (height(node->left) - height(node->right))
              << std::endl;

              */
}






