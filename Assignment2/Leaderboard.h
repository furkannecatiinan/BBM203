#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <ctime>
#include <string>
#include "LeaderboardEntry.h"

#define MAX_LEADERBOARD_SIZE 10

class Leaderboard {
public:
    // Constructor & Destructor
    Leaderboard() = default;
    virtual ~Leaderboard();

    // Pointer to the head of the linked list of leaderboard entries
    LeaderboardEntry *head_leaderboard_entry = nullptr;

    // Reads the leaderboard data from a file
    void read_from_file(const std::string &filename);

    // Writes the current leaderboard data to a file
    void write_to_file(const std::string &filename);

    // Prints the leaderboard to the standard output
    void print_leaderboard();

    // Inserts a new leaderboard entry, maintaining descending order
    void insert(LeaderboardEntry *new_entry);

    unsigned long get_high_score() const;


private:
    // Helper function to maintain the leaderboard size limit
    void enforce_size_limit();
};

#endif // LEADERBOARD_H
