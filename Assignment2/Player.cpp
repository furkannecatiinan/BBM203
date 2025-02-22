#include "Player.h"
#include <iostream>
#include <sstream>
// Constructor to initialize the player's spacecraft, position, and ammo
Player::Player(const vector<vector<bool>> &shape, int row, int col, const string &player_name, int max_ammo, int lives)
        : spacecraft_shape(shape), position_row(row), position_col(col), player_name(player_name),
          max_ammo(max_ammo), current_ammo(max_ammo), lives(lives) {}

// Move player left within the grid boundaries, considering shape width
void Player::move_left() {
    if (position_col > 0) {
        position_col--;
    }
}

// Move player right within the grid boundaries, considering shape width
void Player::move_right(int grid_width) {
    int shape_width = spacecraft_shape[0].size();
    if (position_col + shape_width < grid_width) {
        position_col++;
    } else {
        // Prevent moving beyond the right edge
        position_col = grid_width - shape_width;
    }
}


// Move player up within the grid boundaries, considering shape height
void Player::move_up() {
    if (position_row > 0) {
        position_row--;
    }
}

// Move player down within the grid boundaries, considering shape height
void Player::move_down(int grid_height) {
    int shape_height = spacecraft_shape.size();
    if (position_row + shape_height < grid_height) {
        position_row++;
    } else {
        // Prevent moving beyond the bottom edge
        position_row = grid_height - shape_height;
    }
}


// Check if the player has enough ammo
bool Player::has_ammo() const {
    return current_ammo > 0;
}

// Use one unit of ammo
void Player::use_ammo() {
    if (current_ammo > 0) {
        current_ammo--;
    }
}

// Refill ammo to maximum capacity
void Player::refillAmmo() {
    current_ammo = max_ammo;
}

// Take damage, reducing lives by one
void Player::take_damage() {
    if (lives > 0) {
        lives--;
    }
}

// Increase the player's lives by one
void Player::increaseLife() {
    lives++;
}

void Player::decreaseLife() {
    lives--;
}

// Check if the player is destroyed (no lives left)
bool Player::is_destroyed() const {
    return lives <= 0;
}

// Check if there is a collision with a celestial object
bool Player::check_collision(const CelestialObject *object) const {
    if (!object) {
        cerr << "Warning: Null celestial object provided for collision check." << endl;
        return false;
    }

    int object_row = object->starting_row;
    int object_col = object->time_of_appearance;
    const vector<vector<bool>>& object_shape = object->shape;

    int player_height = spacecraft_shape.size();
    int player_width = spacecraft_shape[0].size();
    int object_height = object_shape.size();
    int object_width = object_shape[0].size();

    for (int i = 0; i < player_height; ++i) {
        for (int j = 0; j < player_width; ++j) {
            if (spacecraft_shape[i][j]) {
                for (int oi = 0; oi < object_height; ++oi) {
                    for (int oj = 0; oj < object_width; ++oj) {
                        if (object_shape[oi][oj]) {
                            int player_abs_row = position_row + i;
                            int player_abs_col = position_col + j;
                            int object_abs_row = object_row + oi;
                            int object_abs_col = object_col + oj;

                            if (player_abs_row == object_abs_row && player_abs_col == object_abs_col) {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}

// Place the player on the grid
void Player::place_on_grid(vector<vector<int>> &grid) const {
    int grid_height = grid.size();
    int grid_width = grid[0].size();

    for (size_t i = 0; i < spacecraft_shape.size(); ++i) {
        for (size_t j = 0; j < spacecraft_shape[i].size(); ++j) {
            if (spacecraft_shape[i][j]) {
                int target_row = position_row + i;
                int target_col = position_col + j;

                if (target_row >= 0 && target_row < grid_height &&
                    target_col >= 0 && target_col < grid_width) {
                    grid[target_row][target_col] = 1;
                }
            }
        }
    }
}



const vector<vector<bool>>& Player::getSpacecraftShape() const {
    return spacecraft_shape;
}

int Player::getPositionRow() const {
    return position_row;
}

int Player::getPositionCol() const {
    return position_col;
}

const string& Player::getPlayerName() const {
    return player_name;
}

int Player::getMaxAmmo() const {
    return max_ammo;
}

int Player::getCurrentAmmo() const {
    return current_ammo;
}

int Player::getLives() const {
    return lives;
}
