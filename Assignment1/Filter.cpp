#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
#include <math.h>

#include <iostream>
#include <cmath>

void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {
    if (kernelSize % 2 == 0 || kernelSize < 1) {
        throw std::invalid_argument("Kernel size must be an odd number and greater than 0.");
    }

    int height = image.get_height();
    int width = image.get_width();
    int halfKernel = kernelSize / 2;


    GrayscaleImage resultImage(width, height);

    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            int sum = 0;
            int count = 0;

            for (int i = -halfKernel; i <= halfKernel; ++i) {
                for (int j = -halfKernel; j <= halfKernel; ++j) {
                    int neighborRow = row + i;
                    int neighborCol = col + j;

                    if (neighborRow >= 0 && neighborRow < height && neighborCol >= 0 && neighborCol < width) {
                        sum += image.get_pixel(neighborRow, neighborCol);
                    } else {
                        sum += 0;
                    }
                    ++count;
                }
            }

            int meanValue = sum / count;
            resultImage.set_pixel(row, col, meanValue);
        }
    }

    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            image.set_pixel(row, col, resultImage.get_pixel(row, col));
        }
    }
}



void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {
    int halfSize = kernelSize / 2;
    std::vector<std::vector<double>> kernel(kernelSize, std::vector<double>(kernelSize));
    double sum = 0.0;

    double sigma2 = sigma * sigma;
    double normalizationFactor = 1.0 / (2.0 * M_PI * sigma2);

    for (int i = -halfSize; i <= halfSize; ++i) {
        for (int j = -halfSize; j <= halfSize; ++j) {
            double exponent = -(i * i + j * j) / (2.0 * sigma2);
            kernel[i + halfSize][j + halfSize] = normalizationFactor * std::exp(exponent);
            sum += kernel[i + halfSize][j + halfSize];
        }
    }

    for (int i = 0; i < kernelSize; ++i) {
        for (int j = 0; j < kernelSize; ++j) {
            kernel[i][j] /= sum;
        }
    }

    GrayscaleImage originalImage = image;

    for (int x = 0; x < image.get_height(); ++x) {
        for (int y = 0; y < image.get_width(); ++y) {
            double newValue = 0.0;

            for (int i = -halfSize; i <= halfSize; ++i) {
                for (int j = -halfSize; j <= halfSize; ++j) {
                    int neighborX = x + i;
                    int neighborY = y + j;

                    if (neighborX >= 0 && neighborX < image.get_height() &&
                        neighborY >= 0 && neighborY < image.get_width()) {
                        newValue += originalImage.get_pixel(neighborX, neighborY) * kernel[i + halfSize][j + halfSize];
                    } else {
                        newValue += 0.0;
                    }
                }
            }

            int pixelValue = static_cast<int>(std::floor(newValue));
            image.set_pixel(x, y, std::min(255, std::max(0, pixelValue)));
        }
    }
}



void Filter::apply_unsharp_mask(GrayscaleImage& image, int kernelSize, double amount) {
    GrayscaleImage originalImage(image);

    GrayscaleImage blurredImage(originalImage);

    Filter::apply_gaussian_smoothing(blurredImage, kernelSize, 1.0);

    for (int x = 0; x < image.get_height(); ++x) {
        for (int y = 0; y < image.get_width(); ++y) {
            int highFreqValue = originalImage.get_pixel(x, y) - blurredImage.get_pixel(x, y);

            double enhancedValue = static_cast<double>(originalImage.get_pixel(x, y))
                                   + (amount * static_cast<double>(highFreqValue));

            int clampedValue = std::min(255, std::max(0, static_cast<int>(std::floor(enhancedValue))));

            image.set_pixel(x, y, clampedValue);
        }
    }
}






