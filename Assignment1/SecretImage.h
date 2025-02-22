#ifndef SECRETIMAGE_H
#define SECRETIMAGE_H

#include "GrayscaleImage.h"
#include <string>
#include <fstream>
#include <stdexcept>
#include <algorithm>

class SecretImage {
private:
    int* upper_triangular; // Upper triangular matrix
    int* lower_triangular; // Lower triangular matrix
    int width, height;

public:
    // Constructors
    SecretImage(const GrayscaleImage& image);
    SecretImage(int w, int h, int* upper, int* lower);

    // Copy constructor
    SecretImage(const SecretImage& other);

    // Copy assignment operator
// **Add this declaration:**
    SecretImage& operator=(const SecretImage& other) noexcept;
    // Destructor
    ~SecretImage();

    // Function to reconstruct the image
    GrayscaleImage reconstruct() const;

    // Function to save the filtered image back to the triangular arrays
    void save_back(const GrayscaleImage& image);

    // Function to save the triangular matrices to a file
    void save_to_file(const std::string& filename);

    // Static function to load a SecretImage from a file
    static SecretImage load_from_file(const std::string& filename);

    // Accessor functions
    int* get_upper_triangular() const;
    int* get_lower_triangular() const;
    int get_width() const;
    int get_height() const;
};

#endif // SECRETIMAGE_H
