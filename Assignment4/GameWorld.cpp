#include "GameWorld.h"
#include "Map.h"

GameWorld::GameWorld() : mapTree(), shaperTree() {}

void GameWorld::initializeGame(std::vector<Isle *> places, std::vector<RealmShaper *> players)
{
    shaperTree.initializeTree(players);
    mapTree.initializeMap(places);
}

Map& GameWorld::getMapTree()
{
    return mapTree;
}
ShaperTree& GameWorld::getShaperTree()
{
    return shaperTree;
}

bool GameWorld::hasAccess(RealmShaper *realmShaper, Isle *isle) {
    int playerDepth = shaperTree.getDepth(realmShaper);
    int totalShaperHeight = shaperTree.getDepth();
    int isleDepth = mapTree.getIsleDepth(isle);
    int totalMapDepth = mapTree.getDepth();

    if (playerDepth == -1 || isleDepth == -1) {
        return false;
    }

    if (shaperTree.getSize() == 1) {
        return true;
    }

    if (totalShaperHeight == 0 && shaperTree.getSize() > 1) {
        totalShaperHeight = 1;
    }

    double ratio = static_cast<double>(playerDepth) / static_cast<double>(totalShaperHeight);
    int minAccessDepth = static_cast<int>(totalMapDepth * ratio);

    return (isleDepth >= minAccessDepth);
}



void GameWorld::exploreArea(RealmShaper *realmShaper, Isle *isle) {
    Isle* existingIsle = mapTree.findIsle(isle->getName());



    if (!existingIsle) {
        if (realmShaper->hasEnoughEnergy()) {
            craft(realmShaper, isle->getName());
            existingIsle = mapTree.findIsle(isle->getName());
            if (!existingIsle) {
                std::cout << "[Explore Area] Isle could not be crafted for some reason." << std::endl;
                return;
            }
        } else {
            std::cout << "[Explore Area] " << realmShaper->getName() << " does not have enough energy to craft " << isle->getName() << std::endl;
            return;
        }
    }


    if (!hasAccess(realmShaper, existingIsle)) {
        std::cout << "[Explore Area] " << realmShaper->getName() << " does not have access to explore area " << existingIsle->getName() << std::endl;
        return;
    }

    if (existingIsle->increaseShaperCount()) {
        std::cout << "[Overcrowding] " << existingIsle->getName() << " self-destructed, it will be removed from the map." << std::endl;
        mapTree.remove(existingIsle);
        if (!mapTree.findIsle(existingIsle->getName())) {
            std::cout << "[Info] Isle removed successfully." << std::endl;
        } else {
            std::cerr << "[Error] Failed to remove Isle from mapTree." << std::endl;
        }
    }


    realmShaper->collectItem(existingIsle->getItem());
    std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << existingIsle->getName() << std::endl;
    std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;


}


void GameWorld::craft(RealmShaper *shaper, const std::string &isleName) {
    /*

    if (!shaper) {
        std::cerr << "[Craft] Invalid shaper provided." << std::endl;
        return;
    }


    if (!shaper->hasEnoughEnergy()) {
        std::cout << "[Craft] " << shaper->getName()
                  << " does not have enough energy points to craft "
                  << isleName << std::endl;
        return;
    }


    Isle *newIsle = new Isle(isleName);


    mapTree.insert(newIsle);


    shaper->loseEnergy();

    std::cout << "[Craft] " << shaper->getName()
              << " crafted new Isle: " << isleName << std::endl;

    std::cout << "[Energy] " << shaper->getName()
              << "'s new energy level: " << shaper->getEnergyLevel() << std::endl;
    */


}


void GameWorld::displayGameState()
{
    mapTree.displayMap();
    shaperTree.displayTree();
}

// TODO: Implement functions to read and parse Access and Duel logs

void GameWorld::processGameEvents(const std::string &accessLogs, const std::string &duelLogs) {
    /*
    std::ifstream accessFile(accessLogs);
    std::ifstream duelFile(duelLogs);

    if (!accessFile.is_open()) {
        std::cerr << "[Error] Cannot open access logs file: " << accessLogs << std::endl;
        return;
    }

    if (!duelFile.is_open()) {
        std::cerr << "[Error] Cannot open duel logs file: " << duelLogs << std::endl;
        return;
    }

    std::string accessLine, duelLine;

    std::getline(accessFile, accessLine);
    std::getline(duelFile, duelLine);

    int accessCount = 0; // 5 erişim kaydını saymak için sayaç

    while (std::getline(accessFile, accessLine)) {
        processAccessEvent(accessLine);
        accessCount++;

        if (accessCount == 5 && std::getline(duelFile, duelLine)) {
            processDuelEvent(duelLine);
            accessCount = 0;
        }

        displayGameState();
    }

    while (std::getline(duelFile, duelLine)) {
        processDuelEvent(duelLine);
        displayGameState();
    }

    accessFile.close();
    duelFile.close();
*/
     }

void GameWorld::processAccessEvent(const std::string &accessLine) {

    /*
    std::istringstream lineStream(accessLine);
    std::string playerName, isleName;

    if (!(lineStream >> playerName >> isleName)) {
        std::cerr << "[Error] Invalid access log format: " << accessLine << std::endl;
        return;
    }

    RealmShaper *player = shaperTree.findPlayer(playerName);
    Isle *isle = mapTree.findIsle(isleName);

    if (!player) {
        std::cerr << "[Error] Player not found in Shaper Tree: " << playerName << std::endl;
        return;
    }

    // Adanın mevcut olup olmadığını kontrol edin
    if (!isle) {
        // Yeterli enerji varsa yeni bir ada oluştur
        if (player->hasEnoughEnergy()) {
            craft(player, isleName);
        } else {
            std::cout << "[Access] " << playerName << " does not have enough energy to craft " << isleName << std::endl;
        }
        return;
    }

    if (!hasAccess(player, isle)) {
        std::cout << "[Access] " << playerName << " does not have access to " << isleName << std::endl;
        return;
    }

    exploreArea(player, isle); */
}

void GameWorld::processDuelEvent(const std::string &duelLine) {
    /*
    std::istringstream lineStream(duelLine);
    std::string challengerName;
    int duelResult;

    if (!(lineStream >> challengerName >> duelResult)) {
        std::cerr << "[Error] Invalid duel log format: " << duelLine << std::endl;
        return;
    }

    RealmShaper *challenger = shaperTree.findPlayer(challengerName);
    if (!challenger) {
        std::cerr << "[Duel] Challenger not found: " << challengerName << std::endl;
        return;
    }

    RealmShaper *winner = shaperTree.duel(challenger, duelResult == 1);
    if (winner) {
        std::cout << "[Duel] Winner: " << winner->getName() << std::endl;
    }
     */
}


void GameWorld::saveGameState(const std::string &currentIsles, const std::string &currentWorld, const std::string &currentShapers, const std::string &currentPlayerTree)
{
    mapTree.writeIslesToFile(currentIsles);
    mapTree.writeToFile(currentWorld);
    shaperTree.writeToFile(currentPlayerTree);
    shaperTree.writeShapersToFile(currentShapers);
}


