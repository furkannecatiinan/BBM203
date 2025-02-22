#include "LeaderboardEntry.h"

// Constructor using correct member variable names
LeaderboardEntry::LeaderboardEntry(unsigned long score, time_t last_played, const std::string &player_name)
        : score(score), last_played(last_played), player_name(player_name), next(nullptr) {
    // Additional initializations if needed
}
