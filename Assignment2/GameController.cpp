#include "GameController.h"
#include <fstream>
#include <iostream>
#include <sstream>

// Simply instantiates the game
GameController::GameController(
        const std::string &space_grid_file_name,
        const std::string &celestial_objects_file_name,
        const std::string &leaderboard_file_name,
        const std::string &player_file_name,
        const std::string &player_name
) {
    game = new AsteroidDash(space_grid_file_name, celestial_objects_file_name, leaderboard_file_name, player_file_name,
                            player_name);
}

// Reads commands from the given input file, executes each command in a game tick
void GameController::play(const std::string &commands_file) {
    std::ifstream commandFile(commands_file);
    if (!commandFile.is_open()) {
        std::cerr << "Error: Unable to open commands file." << std::endl;
        return;
    }

    std::string command;
    while (std::getline(commandFile, command)) {
        // Remove any leading/trailing whitespace
        command.erase(0, command.find_first_not_of(" \t\n\r"));
        command.erase(command.find_last_not_of(" \t\n\r") + 1);

        if (command.empty()) continue;

        if (command == "PRINT_GRID") {
            game->print_space_grid();
        } else if (command == "MOVE_UP") {
            game->get_player()->move_up();
        } else if (command == "MOVE_DOWN") {
            game->get_player()->move_down(game->get_grid_height());
        } else if (command == "MOVE_LEFT") {
            game->get_player()->move_left();
        } else if (command == "MOVE_RIGHT") {
            game->get_player()->move_right(game->get_grid_width());
        } else if (command == "SHOOT") {
            game->shoot();
        } else if (command == "NOP") {
            // No operation, proceed to next tick
        } else {
            std::cerr << "Unknown command: " << command << std::endl;
        }

        game->game_tick();
        game->update_space_grid();
        game->print_space_grid();

        // Check for game over condition
        if (game->is_game_over()) {
            std::cout << "GAME FINISHED! No more commands!" << std::endl;
            game->print_space_grid();
            break;
        }

    }

    commandFile.close();

    // After the game ends, write the leaderboard
    //game->write_leaderboard();
}

// Destructor to delete dynamically allocated member variables here
GameController::~GameController() {
    delete game;
}
