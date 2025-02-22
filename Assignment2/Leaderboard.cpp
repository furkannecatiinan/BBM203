#include "Leaderboard.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>

// Destructor for Leaderboard, which frees dynamically allocated memory
Leaderboard::~Leaderboard() {
    LeaderboardEntry *current = head_leaderboard_entry;
    while (current != nullptr) {
        LeaderboardEntry *next = current->next;
        delete current;
        current = next;
    }
}

// Function to read leaderboard data from a file
void Leaderboard::read_from_file(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    unsigned long score;
    time_t timestamp;
    std::string playerName;
    while (file >> score >> timestamp >> playerName) {
        auto *entry = new LeaderboardEntry(score, timestamp, playerName);
        insert(entry);
    }
    file.close();
}

// Function to write the leaderboard data to a file
void Leaderboard::write_to_file(const std::string &filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    LeaderboardEntry *current = head_leaderboard_entry;
    while (current != nullptr) {
        file << current->score << " " << current->last_played << " " << current->player_name << std::endl;
        current = current->next;
    }
    file.close();
}

// Function to print the leaderboard to the console
void Leaderboard::print_leaderboard() {
    std::cout << "Leaderboard\n-----------" << std::endl;
    LeaderboardEntry *current = head_leaderboard_entry;
    int rank = 1;
    while (current != nullptr) {
        std::tm *timeinfo = std::localtime(&current->last_played);
        char buffer[80];
        strftime(buffer, 80, "%H:%M:%S/%d.%m.%Y", timeinfo);

        std::cout << rank << ". " << current->player_name << " " << current->score << " " << buffer << std::endl;
        current = current->next;
        rank++;
    }
}

// Function to insert a new entry into the leaderboard
void Leaderboard::insert(LeaderboardEntry *new_entry) {
    if (!new_entry) return;

    if (!head_leaderboard_entry || new_entry->score > head_leaderboard_entry->score) {
        new_entry->next = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
    } else {
        LeaderboardEntry *current = head_leaderboard_entry;
        while (current->next && current->next->score >= new_entry->score) {
            current = current->next;
        }
        new_entry->next = current->next;
        current->next = new_entry;
    }

    // Limit the leaderboard to 10 entries
    LeaderboardEntry *current = head_leaderboard_entry;  // 'current' yeniden tanımlandı
    int count = 1;

// Geçerli girişler 10'u aşarsa, fazladan olanları kaldır
    while (current && current->next) {
        count++;
        if (count > 10) {
            LeaderboardEntry *to_delete = current->next;
            current->next = current->next->next;  // Fazladan olan girişin referansını güncelle
            delete to_delete;  // Fazla olan düğümü serbest bırak
            break;  // İşlem tamamlandıktan sonra döngüden çık
        }
        current = current->next;
    }

}

unsigned long Leaderboard::get_high_score() const {
    if (head_leaderboard_entry != nullptr) {
        return head_leaderboard_entry->score;
    } else {
        return 0; // Eğer liderlik tablosu boşsa, en yüksek skor 0'dır
    }
}
