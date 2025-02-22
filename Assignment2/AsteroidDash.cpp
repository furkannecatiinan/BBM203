#include "AsteroidDash.h"

// Constructor to initialize AsteroidDash with the given parameters
AsteroidDash::AsteroidDash(const string &space_grid_file_name,
                           const string &celestial_objects_file_name,
                           const string &leaderboard_file_name,
                           const string &player_file_name,
                           const string &player_name)

        : leaderboard_file_name(leaderboard_file_name), leaderboard(Leaderboard()) {

    read_player(player_file_name, player_name);  // Initialize player using the player.dat file
    read_space_grid(space_grid_file_name);  // Initialize the grid after the player is loaded

    read_celestial_objects(celestial_objects_file_name);  // Load celestial objects

    leaderboard.read_from_file(leaderboard_file_name);


}

// Function to read the space grid from a file
void AsteroidDash::read_space_grid(const string &input_file) {
    ifstream file(input_file);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << input_file << endl;
        return;
    }

    string line;
    space_grid.clear();

    // Read each line from the file
    while (getline(file, line)) {
        vector<int> row;
        stringstream ss(line);
        int cell;
        // Read each integer from the line and add it to the row
        while (ss >> cell) {
            row.push_back(cell);
        }
        space_grid.push_back(row);
    }

    file.close();
}

// Function to read the player from a file
void AsteroidDash::read_player(const std::string &player_file_name, const std::string &player_name) {
    std::ifstream file(player_file_name);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << player_file_name << std::endl;
        return;
    }

    int row, col;
    file >> row >> col;

    // Check for invalid position
    if (row < 0 || col < 0) {
        std::cerr << "Error: Invalid player position. Rows and columns must be non-negative." << std::endl;
        file.close();
        return;
    }

    std::vector<std::vector<bool>> shape;
    std::string line;

    while (getline(file, line)) {
        if (!line.empty()) {
            std::vector<bool> row_data;
            for (char c : line) {
                if (c == '1') {
                    row_data.push_back(true);
                } else if (c == '0') {
                    row_data.push_back(false);
                } else if (c != ' ') {  // Boşluk karakterlerini yok say
                    std::cerr << "Error: Invalid character '" << c << "' in player shape. Only '1' and '0' are allowed." << std::endl;
                    file.close();
                    return;
                }
            }
            shape.push_back(row_data);
        }
    }


    // Check if the shape is empty
    if (shape.empty()) {
        std::cerr << "Error: Player shape cannot be empty." << std::endl;
        file.close();
        return;
    }

    player = new Player(shape, row, col, player_name);
    file.close();
}

// Function to read celestial objects from a file
void AsteroidDash::read_celestial_objects(const std::string &input_file) {
    std::ifstream file(input_file);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << input_file
                  << "'. Please check if the file path is correct and the file "
                     "exists."
                  << std::endl;
        return;
    }

    CelestialObject *previous = nullptr;
    celestial_objects_list_head = nullptr;
    std::string line;
    int line_number = 0;
    std::string current_line;

    // Read and create all CelestialObjects
    while (true) {
        if (!current_line.empty()) {
            line = current_line;
            current_line.clear();
        } else {
            if (!std::getline(file, line)) break;
            line_number++;
        }

        if (line.empty() ||
            line.find_first_not_of(" \t\n\r") == std::string::npos)
            continue;

        if (line[0] == '[' || line[0] == '{') {
            ObjectType type = (line[0] == '[') ? ASTEROID : POWER_UP;
            std::vector<std::vector<bool>> shape;
            char end_char = (line[0] == '[') ? ']' : '}';

            do {
                std::vector<bool> row;
                for (char c : line) {
                    if (c == '1')
                        row.push_back(true);
                    else if (c == '0')
                        row.push_back(false);
                }
                if (!row.empty()) shape.push_back(row);

                if (line.back() == end_char) break;

                if (!std::getline(file, line)) break;
                line_number++;
            } while (true);

            int start_row = -1;
            int time_of_appearance = -1;
            std::string effect;

            while (true) {
                if (!std::getline(file, line)) break;
                line_number++;
                if (line.empty()) continue;

                if (line[0] == 's') {
                    std::stringstream ss(line.substr(2));
                    ss >> start_row;
                } else if (line[0] == 't') {
                    std::stringstream ss(line.substr(2));
                    ss >> time_of_appearance;
                } else if (line[0] == 'e') {
                    effect = line.substr(2);
                    if (effect == "life")
                        type = LIFE_UP;
                    else if (effect == "ammo")
                        type = AMMO;
                    else {
                        std::cerr << "Error (line " << line_number
                                  << "): Invalid effect type '" << effect
                                  << "'. Expected 'life' or 'ammo'."
                                  << std::endl;
                        continue;
                    }
                } else {
                    current_line = line;
                    break;
                }
            }

            // Create CelestialObject without generating rotations yet
            CelestialObject *new_object = new CelestialObject(
                    shape, type, start_row, time_of_appearance);

            if (!celestial_objects_list_head) {
                celestial_objects_list_head = new_object;
            } else {
                previous->next_celestial_object = new_object;
            }
            previous = new_object;
        } else {
            std::cerr << "Error (line " << line_number << "): Unrecognized line '"
                      << line << "'." << std::endl;
        }
    }

    file.close();


    // Now, traverse the list and generate rotations for each CelestialObject
    CelestialObject *current = celestial_objects_list_head;
    while (current != nullptr) {
        // Generate rotations for the current object
        current->generate_rotations();

        // Move to the next original object
        current = current->next_celestial_object;
    }
}

// Print the entire space grid
void AsteroidDash::print_space_grid(){
    // En yüksek puanı alın
    unsigned long high_score = leaderboard.get_high_score();

    // Eğer mevcut skor en yüksek skoru aşıyorsa, en yüksek skoru güncelle
    if (current_score > high_score) {
        high_score = current_score;
        // En yüksek skoru liderlik tablosuna kaydet
        LeaderboardEntry* new_entry = new LeaderboardEntry(current_score, std::time(nullptr), player->getPlayerName());
        leaderboard.insert(new_entry);
        leaderboard.write_to_file(leaderboard_file_name); // Değişiklikleri dosyaya yaz
    }

    // Oyun bilgilerini yazdır
    std::cout << "Tick: " << game_time << std::endl;
    std::cout << "Lives: " << player->getLives() << std::endl;
    std::cout << "Ammo: " << player->getCurrentAmmo() << std::endl;
    std::cout << "Score: " << current_score << std::endl;
    std::cout << "High Score: " << high_score << std::endl;

    // Hücreler için karakter tanımları
    const std::string occupied_cell = occupiedCellChar;    // "██"
    const std::string unoccupied_cell = unoccupiedCellChar; // "▒▒"

    // Grid'i satır satır yazdır
    for (const auto &row : space_grid) {
        for (int cell : row) {
            if (cell == 0) {
                // Boş hücre
                std::cout << unoccupied_cell;
            } else {
                // Dolu hücre
                std::cout << occupied_cell;
            }
        }
        std::cout << std::endl;
    }
}


// Updated update_space_grid function with collision detection
void AsteroidDash::update_space_grid() {
    if (player->lives <= 0) {
        game_over = true;
        return;
    }

    // Clear the space grid for the new frame
    for (auto &row : space_grid) {
        fill(row.begin(), row.end(), 0);
    }

    // Place the player on the grid
    int grid_height = space_grid.size();
    int grid_width = space_grid[0].size();

    for (size_t i = 0; i < player->spacecraft_shape.size(); ++i) {
        for (size_t j = 0; j < player->spacecraft_shape[i].size(); ++j) {
            if (player->spacecraft_shape[i][j]) {
                int target_row = player->position_row + i;
                int target_col = player->position_col + j;

                space_grid[target_row][target_col] = 1; // '1' represents the player
            }
        }
    }

    // Initialize pointers for traversing the linked list
    CelestialObject *current = celestial_objects_list_head;
    CelestialObject *previous = nullptr;

    // Traverse the linked list of celestial objects
    while (current) {
        CelestialObject *next = current->next_celestial_object; // Store next object

        // Check if the celestial object should appear based on game_time
        if (game_time < current->time_of_appearance) {
            // Object has not appeared yet; move to the next one
            previous = current;
            current = next;
            continue;
        }

        // Calculate the current horizontal position (current_col) of the celestial object
        int shape_width = current->shape.empty() ? 0 : current->shape[0].size();
        int ticks_since_appearance = game_time - current->time_of_appearance;
        int current_col = grid_width - shape_width - ticks_since_appearance;

        // Determine if the celestial object is completely out of bounds (to the left)
        if (current_col + shape_width <= 0) {
            // Remove the celestial object from the linked list
            if (previous) {
                previous->next_celestial_object = next;
            } else {
                celestial_objects_list_head = next;
            }

            // Delete all rotations associated with this celestial object to prevent memory leaks
            CelestialObject::delete_rotations(current);

            // Delete the main celestial object
            delete current;

            // Move to the next celestial object
            current = next;
            continue;
        }

        // Initialize flags for collision detection and object removal
        bool collision_detected = false;
        bool will_remove = false;

        // Iterate through each cell of the celestial object's shape
        for (int i = 0; i < current->shape.size(); ++i) {
            for (int j = 0; j < current->shape[i].size(); ++j) {
                if (!current->shape[i][j]) continue; // Skip empty cells

                int obj_row = current->starting_row + i;
                int obj_col = current_col + j;

                // Check if the object's cell is within grid bounds
                if (obj_row < 0 || obj_row >= grid_height ||
                    obj_col < 0 || obj_col >= grid_width) {
                    continue; // Skip cells outside the grid
                }

                // Check for collision with the player
                if (space_grid[obj_row][obj_col] == 1) {
                    if (current->object_type == ASTEROID) {
                        player->lives--;
                    } else if (current->object_type == LIFE_UP) {
                        player->lives++;
                    } else if (current->object_type == AMMO) {
                        player->current_ammo = player->max_ammo;
                    }

                    collision_detected = true;
                    will_remove = true;
                    break; // Exit the inner loop as collision has been handled
                }
            }
            if (collision_detected) break; // Exit the outer loop if collision detected
        }

        if (will_remove) {
            // Remove the celestial object from the linked list
            if (previous != nullptr) {
                previous->next_celestial_object = next;
            } else {
                celestial_objects_list_head = next;
            }

            // Delete all rotations associated with this celestial object to prevent memory leaks
            CelestialObject::delete_rotations(current);

            // Delete the main celestial object
            delete current;

            // Move to the next celestial object
            current = next;
            continue;
        }

        // Place the celestial object on the grid based on its current position
        for (int i = 0; i < current->shape.size(); ++i) {
            for (int j = 0; j < current->shape[i].size(); ++j) {
                if (!current->shape[i][j]) continue; // Skip empty cells

                int obj_row = current->starting_row + i;
                int obj_col = current_col + j;

                // Check if the cell is within grid bounds before placing
                if (obj_row < 0 || obj_row >= grid_height ||
                    obj_col < 0 || obj_col >= grid_width) {
                    continue; // Skip cells outside the grid
                }

                // Place the celestial object on the grid
                space_grid[obj_row][obj_col] = 2; // '2' represents a celestial object
            }
        }

        // Move to the next celestial object in the linked list
        previous = current;
        current = next;
    }

    // Final check to see if the game is over after all updates
    if (player->lives <= 0) {
        game_over = true;
        return;
    }
}


void AsteroidDash::update_celestial_objects() {
    CelestialObject* current = celestial_objects_list_head;
    while (current != nullptr) {
        current->move_left(); // Her gök cismini bir adım sola hareket ettir
        current = current->next_celestial_object;
    }
}

void AsteroidDash::update_projectiles() {
    for (auto it = active_projectiles.begin(); it != active_projectiles.end(); ) {
        // Projeyi bir adım yukarı hareket ettir
        it->first -= 1;

        // Proje grid'in üstünden çıktıysa, listeden kaldır
        if (it->first < 0) {
            it = active_projectiles.erase(it);
        } else {
            ++it;
        }
    }
}

// Corresponds to the SHOOT command.
// It should shoot if the player has enough ammo.
// It should decrease the player's ammo
// AsteroidDash.cpp

void AsteroidDash::shoot() {
    // Oyuncunun mevcut pozisyonunu alın
    int player_row = player->getPositionRow();
    int player_col = player->getPositionCol();

    // Uzay aracının genişliğini hesaplayın
    int spacecraft_width = player->getSpacecraftShape()[0].size();

    // Projenin başlangıç pozisyonunu belirleyin (oyuncunun üstündeki satır ve ortadaki sütun)
    int projectile_row = player_row - 1; // Bir satır yukarısı
    int projectile_col = player_col + spacecraft_width / 2; // Uzay aracının ortası

    // Proje grid'in üstünde değilse, aktif projeler listesine ekleyin
    if (projectile_row >= 0) {
        active_projectiles.push_back(std::make_pair(projectile_row, projectile_col));
    }
}


// Destructor. Remove dynamically allocated member variables here.
AsteroidDash::~AsteroidDash() {
    // TODO: Your code here
}


// Getters
Player* AsteroidDash::get_player() const {
    return player;
}

int AsteroidDash::get_grid_width() const {
    return space_grid[0].size();
}

int AsteroidDash::get_grid_height() const {
    return space_grid.size();
}

bool AsteroidDash::is_game_over() const {
    return game_over;
}


void AsteroidDash::game_tick() {

    // Gök cisimlerini güncelle
    update_celestial_objects();

    // Projeleri güncelle
    update_projectiles();

    // Oyuncu hareket etmiyorsa, sabit kalır (hareket komutu verilmediği varsayılıyor)

    // Grid'i güncelle
    update_space_grid();

    game_time++;

}




