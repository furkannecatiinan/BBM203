#include "GrayscaleImage.h"
#include <iostream>
#include <cstring>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdexcept>


// Constructor: load from a file
GrayscaleImage::GrayscaleImage(const char* filename) {
    int channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, STBI_grey);

    if (image == nullptr) {
        std::cerr << "Error: Could not load image " << filename << std::endl;
        exit(1);
    }

    data = new int*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new int[width];
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            data[i][j] = static_cast<int>(image[i * width + j]);
        }
    }



    stbi_image_free(image);
}
// Copy Assignment Operator
GrayscaleImage& GrayscaleImage::operator=(const GrayscaleImage& other) noexcept {
    if (this != &other) {
        for (int i = 0; i < height; ++i) {
            delete[] data[i];
        }
        delete[] data;

        width = other.width;
        height = other.height;

        data = new int*[height];
        for (int i = 0; i < height; ++i) {
            data[i] = new int[width];
        }

        for (int i = 0; i < height; ++i) {
            std::copy(other.data[i], other.data[i] + width, data[i]);
        }
    }
    return *this;
}



// Constructor: initialize from a pre-existing data matrix
GrayscaleImage::GrayscaleImage(int** inputData, int h, int w) : width(w), height(h) {
    data = new int*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new int[width];
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            data[i][j] = inputData[i][j];
        }
    }
}


// Constructor to create a blank image of given width and height
GrayscaleImage::GrayscaleImage(int w, int h) : width(w), height(h) {
    data = new int*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new int[width];
        for (int j = 0; j < width; ++j) {
            data[i][j] = 0;
        }
    }
}


// Copy constructor
GrayscaleImage::GrayscaleImage(const GrayscaleImage& other) {
    width = other.width;
    height = other.height;

    data = new int*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new int[width];
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            data[i][j] = other.data[i][j];
        }
    }
}


// Destructor
GrayscaleImage::~GrayscaleImage() {
    for (int i = 0; i < height; ++i) {
        delete[] data[i];
    }
    delete[] data;
}

// Equality operator
bool GrayscaleImage::operator==(const GrayscaleImage& other) const {
    if (width != other.width || height != other.height) {
        return false;
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (data[i][j] != other.data[i][j]) {
                return false;
            }
        }
    }
    return true;
}



// Addition operator
GrayscaleImage GrayscaleImage::operator+(const GrayscaleImage& other) const {
    GrayscaleImage result(width, height);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int sum = data[i][j] + other.data[i][j];
            result.data[i][j] = std::min(sum, 255); // Clamp to 255
        }
    }

    return result;
}

// Subtraction operator
GrayscaleImage GrayscaleImage::operator-(const GrayscaleImage& other) const {
    GrayscaleImage result(width, height);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int diff = data[i][j] - other.data[i][j];
            result.data[i][j] = std::max(diff, 0);
        }
    }

    return result;
}


// Get a specific pixel value
int GrayscaleImage::get_pixel(int row, int col) const {
    return data[row][col];
}

// Set a specific pixel value
void GrayscaleImage::set_pixel(int row, int col, int value) {
    data[row][col] = value;
}

// Function to save the image to a PNG file
void GrayscaleImage::save_to_file(const char* filename) const {
    unsigned char* imageBuffer = new unsigned char[width * height];

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            imageBuffer[i * width + j] = static_cast<unsigned char>(data[i][j]);
        }
    }

    if (!stbi_write_png(filename, width, height, 1, imageBuffer, width)) {
        std::cerr << "Error: Could not save image to file " << filename << std::endl;
    }

    delete[] imageBuffer;

}

// Function to print the pixel values of the image
void GrayscaleImage::print_image() const {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            std::cout << data[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

