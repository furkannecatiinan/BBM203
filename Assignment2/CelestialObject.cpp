#include "CelestialObject.h"

// Constructor
CelestialObject::CelestialObject(const std::vector<std::vector<bool>> &shape,
                                 ObjectType type, int start_row,
                                 int time_of_appearance)
        : shape(shape), object_type(type), starting_row(start_row),
          time_of_appearance(time_of_appearance),
          position_row(start_row),
          position_col(-1), // Will be set when the object appears
          next_celestial_object(nullptr), right_rotation(this),
          left_rotation(this) {}


// Destructor
CelestialObject::~CelestialObject() {

}


void CelestialObject::generate_rotations() {
    if (shape.empty()) return;

    std::vector<std::vector<std::vector<bool>>> unique_shapes;
    unique_shapes.push_back(shape);  // Original shape

    CelestialObject *last_rotation = this;
    std::vector<std::vector<bool>> current_shape = shape;

    for (int i = 1; i < 4; ++i) {  // Generate up to 3 rotations
        // Rotate the shape 90 degrees
        current_shape = rotate_shape_90(current_shape);

        // Check if the new shape is unique
        bool is_unique = true;
        for (const auto &existing_shape : unique_shapes) {
            if (are_shapes_equal(current_shape, existing_shape)) {
                is_unique = false;
                break;
            }
        }

        if (is_unique) {
            unique_shapes.push_back(current_shape);

            // Create a new CelestialObject for the rotation
            CelestialObject *rotation = new CelestialObject(
                    current_shape, object_type, starting_row, time_of_appearance);

            // Set next_celestial_object for the rotation
            rotation->next_celestial_object = this->next_celestial_object;

            // Link rotations via right_rotation and left_rotation
            rotation->right_rotation = this;
            rotation->left_rotation = last_rotation;
            last_rotation->right_rotation = rotation;
            this->left_rotation = rotation;

            last_rotation = rotation;
        }
    }
}


// Static function to delete rotations (unchanged)
void CelestialObject::delete_rotations(CelestialObject *target) {
    if (target) {
        target->delete_rotations();
    }
}
// Function to delete all rotations (unchanged)
void CelestialObject::delete_rotations() {
    if (!right_rotation || right_rotation == this) return;  // No rotations to delete

    CelestialObject *current = right_rotation;
    right_rotation = this;  // Break the circular reference from the head
    left_rotation = this;

    while (current != this && current != nullptr) {
        CelestialObject *next = current->right_rotation;
        current->right_rotation = nullptr;
        current->left_rotation = nullptr;
        delete current;
        current = next;
    }
}


// Helper function to rotate the shape 90 degrees clockwise
std::vector<std::vector<bool>> CelestialObject::rotate_shape_90(
        const std::vector<std::vector<bool>> &shape) {
    int rows = shape.size();
    int cols = shape[0].size();
    std::vector<std::vector<bool>> rotated(cols,
                                           std::vector<bool>(rows, false));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            rotated[j][rows - 1 - i] = shape[i][j];
        }
    }
    return rotated;
}

// Helper function to compare two shapes
bool CelestialObject::are_shapes_equal(
        const std::vector<std::vector<bool>> &shape1,
        const std::vector<std::vector<bool>> &shape2) {
    if (shape1.size() != shape2.size()) return false;
    for (size_t i = 0; i < shape1.size(); ++i) {
        if (shape1[i].size() != shape2[i].size()) return false;
        for (size_t j = 0; j < shape1[i].size(); ++j) {
            if (shape1[i][j] != shape2[i][j]) return false;
        }
    }
    return true;
}


void CelestialObject::rotate_clockwise() {
    // Shape'i saat yönünde 90 derece döndür
    shape = rotate_shape_90(shape);
}

void CelestialObject::rotate_counterclockwise() {
    // Shape'i saat yönünün tersinde 90 derece döndürmek için 3 kere saat yönünde döndür
    for (int i = 0; i < 3; ++i) {
        shape = rotate_shape_90(shape);
    }
}

// CelestialObject.cpp

void CelestialObject::move_left() {
    position_col -= 1;
}

// Getter ve Setter fonksiyonları
int CelestialObject::getPositionCol() const {
    return position_col;
}

int CelestialObject::getPositionRow() const {
    return position_row;
}

void CelestialObject::setPosition(int row, int col) {
    position_row = row;
    position_col = col;
}

void CelestialObject::remove_cell_at(int row, int col) {
    if (row >= 0 && row < shape.size() && col >= 0 && col < shape[0].size()) {
        shape[row][col] = false;


        bool row_empty = std::all_of(shape[row].begin(), shape[row].end(), [](bool cell) { return !cell; });
        if (row_empty) {
            shape.erase(shape.begin() + row);
            position_row += (row > 0) ? 0 : 1; // Eğer ilk satırsa, position_row'u güncelle
        }


        bool col_empty = true;
        for (const auto& r : shape) {
            if (col < r.size() && r[col]) {
                col_empty = false;
                break;
            }
        }
        if (col_empty) {
            for (auto& r : shape) {
                r.erase(r.begin() + col);
            }
            position_col += (col > 0) ? 0 : 1; // Eğer ilk sütunsa, position_col'u güncelle
        }
    }
}

bool CelestialObject::is_destroyed() const {
    for (const auto& row : shape) {
        for (bool cell : row) {
            if (cell) {
                return false;
            }
        }
    }
    return true;
}

bool CelestialObject::check_collision_with_projectile(int projectile_row, int projectile_col) const {
    int relative_row = projectile_row - position_row;
    int relative_col = projectile_col - position_col;

    if (relative_row >= 0 && relative_row < shape.size() &&
        relative_col >= 0 && relative_col < shape[0].size()) {
        return shape[relative_row][relative_col];
    }
    return false;
}

