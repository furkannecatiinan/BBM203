#ifndef CELESTIALOBJECT_H
#define CELESTIALOBJECT_H

#include <vector>
#include <algorithm>


using namespace std;

// Enum to represent the type of the object (asteroid, life-up, ammo, or power-up)
enum ObjectType {
    ASTEROID,
    LIFE_UP,
    AMMO,
    POWER_UP
};

class CelestialObject {
public:
    // Constructor with flag to control rotation generation
    CelestialObject(const std::vector<std::vector<bool>> &shape,ObjectType type, int start_row, int time_of_appearance);



    // Destructor
    ~CelestialObject();

    // Function to delete all rotations
    void delete_rotations();

    // Static function to delete rotations of a given celestial object
    static void delete_rotations(CelestialObject *target);

    // Function to generate all rotations
    void generate_rotations();

    // Helper function to rotate the shape 90 degrees clockwise
    vector<vector<bool>> rotate_shape_90(const vector<vector<bool>> &shape);

    // Helper function to compare two shapes
    bool are_shapes_equal(const vector<vector<bool>> &shape1, const vector<vector<bool>> &shape2);

    // Shape of the object represented as a 2D boolean vector
    vector<vector<bool>> shape;

    // Pointer to the object's clockwise neighbor (its right rotation)
    CelestialObject *right_rotation;

    // Pointer to the object's counter-clockwise neighbor (its left rotation)
    CelestialObject *left_rotation;

    // Pointer to the next celestial object in the list
    CelestialObject *next_celestial_object;

    // Type of the object (asteroid, life-up, ammo, or power-up)
    ObjectType object_type;

    // Row in the space grid where the object's top will appear
    int starting_row;

    // The step in the game after which the object will appear on the grid
    int time_of_appearance;

    // Asteroid'i saat yönünde döndüren fonksiyon
    void rotate_clockwise();

// Asteroid'i saat yönünün tersinde döndüren fonksiyon
    void rotate_counterclockwise();

    // Nesneyi bir adım sola hareket ettirir
    void move_left();

    // Nesnenin konumunu alır
    int getPositionCol() const;
    int getPositionRow() const;

    // Nesnenin konumunu ayarlar
    void setPosition(int row, int col);

    void remove_cell_at(int row, int col);

    bool is_destroyed() const;
    bool check_collision_with_projectile(int projectile_row, int projectile_col) const;

    void place_on_grid(std::vector<std::vector<int>>& grid) const;
    int position_row;
    int position_col;

        private:
    // Indicates if this object is the head of a rotation chain
    bool is_rotation_head;

};

#endif // CELESTIALOBJECT_H
