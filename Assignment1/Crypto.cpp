#include "Crypto.h"
#include "GrayscaleImage.h"


// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length
std::vector<int> Crypto::extract_LSBits(SecretImage& secret_image, int message_length) {
    std::vector<int> LSB_array;
    GrayscaleImage image = secret_image.reconstruct();
    int total_bits = message_length * 7;
    int total_pixels = image.get_width() * image.get_height();

    if (total_bits > total_pixels) {
        throw std::runtime_error("Message is too long for the image.");
    }

    int bit_index = total_pixels - total_bits;
    for (int i = bit_index; i < total_pixels; ++i) {
        int pixel_value = image.get_pixel(i / image.get_width(), i % image.get_width());
        int LSB = pixel_value & 1;
        LSB_array.push_back(LSB);
    }

    return LSB_array;
}



// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int>& LSB_array) {
    if (LSB_array.size() % 7 != 0) {
        throw std::runtime_error("LSB array size must be a multiple of 7.");
    }

    std::string message;
    for (size_t i = 0; i < LSB_array.size(); i += 7) {
        int ascii_value = 0;
        for (int j = 0; j < 7; ++j) {
            ascii_value |= (LSB_array[i + j] << (6 - j));
        }
        message.push_back(static_cast<char>(ascii_value));
    }

    return message;
}



// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string& message) {
    std::vector<int> LSB_array;

    for (char ch : message) {
        std::bitset<7> bits(ch);
        for (int i = 6; i >= 0; --i) {
            LSB_array.push_back(bits[i]);
        }
    }

    return LSB_array;
}


// Embed LSB array into GrayscaleImage starting from the last bit of the image
SecretImage Crypto::embed_LSBits(GrayscaleImage& image, const std::vector<int>& LSB_array) {
    int total_bits = LSB_array.size();
    int total_pixels = image.get_width() * image.get_height();

    if (total_bits > total_pixels) {
        throw std::runtime_error("The image does not have enough pixels to embed the message.");
    }


    int bit_index = total_pixels - total_bits;

    for (int i = bit_index; i < total_pixels; ++i) {
        int pixel_value = image.get_pixel(i / image.get_width(), i % image.get_width());
        pixel_value &= ~1;
        pixel_value |= LSB_array[i - bit_index];
        image.set_pixel(i / image.get_width(), i % image.get_width(), pixel_value);
    }

    SecretImage secret_image(image);
    return secret_image;
}

