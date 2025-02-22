#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include <sstream>
#include "CelestialObject.h" // Assuming CelestialObject is needed for collision checks

using namespace std;

class Player {
public:
    // Player's spacecraft shape
    vector<vector<bool>> spacecraft_shape;

    // Row where the top-left of the spacecraft is located
    int position_row;

    // Column where the top-left of the spacecraft is located
    int position_col;

    // Name of the player
    string player_name;

    // Maximum ammo capacity
    int max_ammo;

    // Current ammo level
    int current_ammo;

    // Current lives of the player
    int lives;

    // Constructor with default values
    Player(const vector<vector<bool>> &shape, int row, int col, const string &player_name, int max_ammo = 10, int lives = 3);

    // Move player in the space grid
    void move_left();
    void move_right(int grid_width);
    void move_up();
    void move_down(int grid_height);

    // Check if the player has enough ammo
    bool has_ammo() const;

    // Use one unit of ammo
    void use_ammo();

    // Refill ammo to maximum capacity
    void refillAmmo();

    // Take damage, reducing lives by one
    void take_damage();

    // Increase the player's lives by one
    void increaseLife();
    void decreaseLife();

    // Check if the player is destroyed (no lives left)
    bool is_destroyed() const;

    // Check if there is a collision with a celestial object
    bool check_collision(const CelestialObject *object) const;

    // Place the player's spacecraft on the grid
    void place_on_grid(vector<vector<int>> &grid) const;

    // Get fonksiyonları
    const vector<vector<bool>>& getSpacecraftShape() const;
    int getPositionRow() const;
    int getPositionCol() const;
    const string& getPlayerName() const;
    int getMaxAmmo() const;
    int getCurrentAmmo() const;
    int getLives() const;
};

#endif // PLAYER_H
