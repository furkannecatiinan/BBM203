#include "SecretImage.h"


SecretImage::SecretImage(const GrayscaleImage& image) {
    width = image.get_width();
    height = image.get_height();

    int upperSize = (width * (width + 1)) / 2;
    int lowerSize = (width * (width - 1)) / 2;

    upper_triangular = new int[upperSize];
    lower_triangular = new int[lowerSize];

    int upperIndex = 0, lowerIndex = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (i <= j) {
                upper_triangular[upperIndex++] = image.get_pixel(i, j);
            } else {
                lower_triangular[lowerIndex++] = image.get_pixel(i, j);
            }
        }
    }
}


// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int * upper, int * lower) {
    width = w;
    height = h;

    int upperSize = (width * (width + 1)) / 2;
    int lowerSize = (width * (width - 1)) / 2;

    upper_triangular = new int[upperSize];
    lower_triangular = new int[lowerSize];

    std::copy(upper, upper + upperSize, upper_triangular);
    std::copy(lower, lower + lowerSize, lower_triangular);
}

// Copy Constructor
SecretImage::SecretImage(const SecretImage& other) {
    width = other.width;
    height = other.height;

    int upperSize = (width * (width + 1)) / 2;
    int lowerSize = (width * (width - 1)) / 2;

    upper_triangular = new int[upperSize];
    lower_triangular = new int[lowerSize];

    std::copy(other.upper_triangular, other.upper_triangular + upperSize, upper_triangular);
    std::copy(other.lower_triangular, other.lower_triangular + lowerSize, lower_triangular);
}

// Copy Assignment Operator
SecretImage& SecretImage::operator=(const SecretImage& other) noexcept {
    if (this != &other) {
        delete[] upper_triangular;
        delete[] lower_triangular;

        width = other.width;
        height = other.height;

        int upperSize = (width * (width + 1)) / 2;
        int lowerSize = (width * (width - 1)) / 2;

        upper_triangular = new int[upperSize];
        lower_triangular = new int[lowerSize];

        std::copy(other.upper_triangular, other.upper_triangular + upperSize, upper_triangular);
        std::copy(other.lower_triangular, other.lower_triangular + lowerSize, lower_triangular);
    }
    return *this;
}



// Destructor: free the arrays
SecretImage::~SecretImage() {
    delete[] upper_triangular;
    delete[] lower_triangular;
}


// Reconstructs and returns the full image from upper and lower triangular matrices.
GrayscaleImage SecretImage::reconstruct() const {
    GrayscaleImage image(width, height);

    int upperIndex = 0, lowerIndex = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (i <= j) {
                image.set_pixel(i, j, upper_triangular[upperIndex++]);
            } else {
                image.set_pixel(i, j, lower_triangular[lowerIndex++]);
            }
        }
    }

    return image;
}


// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage& image) {
    int upperIndex = 0, lowerIndex = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (i <= j) {
                upper_triangular[upperIndex++] = image.get_pixel(i, j);
            } else {
                lower_triangular[lowerIndex++] = image.get_pixel(i, j);
            }
        }
    }
}


// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("File cannot be opened.");
    }

    file << width << " " << height << "\n";

    int upperSize = (width * (width + 1)) / 2;
    for (int i = 0; i < upperSize; ++i) {
        file << upper_triangular[i];
        if (i < upperSize - 1) file << " ";
    }
    file << "\n";

    int lowerSize = (width * (width - 1)) / 2;
    for (int i = 0; i < lowerSize; ++i) {
        file << lower_triangular[i];
        if (i < lowerSize - 1) file << " ";
    }
    file.close();
}


// Static function to load a SecretImage from a file
SecretImage SecretImage::load_from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("File cannot be opened.");
    }

    int width, height;
    file >> width >> height;

    int upperSize = (width * (width + 1)) / 2;
    int lowerSize = (width * (width - 1)) / 2;

    int* upper = new int[upperSize];
    int* lower = new int[lowerSize];

    for (int i = 0; i < upperSize; ++i) {
        file >> upper[i];
    }

    for (int i = 0; i < lowerSize; ++i) {
        file >> lower[i];
    }

    file.close();

    SecretImage secret_image(width, height, upper, lower);

    delete[] upper;
    delete[] lower;

    return secret_image;
}



// Returns a pointer to the upper triangular part of the secret image.
int * SecretImage::get_upper_triangular() const {
    return upper_triangular;
}

// Returns a pointer to the lower triangular part of the secret image.
int * SecretImage::get_lower_triangular() const {
    return lower_triangular;
}

// Returns the width of the secret image.
int SecretImage::get_width() const {
    return width;
}

// Returns the height of the secret image.
int SecretImage::get_height() const {
    return height;
}