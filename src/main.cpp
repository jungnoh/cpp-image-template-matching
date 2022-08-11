#include <iostream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "matcher.h"

double toGrayscale(uint8_t r, uint8_t g, uint8_t b) {
    return 0.299*r + 0.587*g + 0.114*b;
}

void stbiToGrayscale(uint8_t* img, uint8_t* out, int width, int height) {
    for (auto i=0; i<width*height; i++) {
        out[i] = toGrayscale(img[3*i], img[3*i+1], img[3*i+2]);
    }
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Incorrect arguments. Usage: ./match [target image] [pattern image]" << std::endl;
        return 1;
    }

    int imgWidth, imgHeight, imgBpp;
    int patWidth, patHeight, patBpp;

    uint8_t* target_image = stbi_load(argv[1], &imgWidth, &imgHeight, &imgBpp, 3);
    uint8_t* pattern_image = stbi_load(argv[2], &patWidth, &patHeight, &patBpp, 3);

    std::cout << "Target image: " << imgWidth << "x" << imgHeight << std::endl;
    std::cout << "Pattern image: " << patWidth << "x" << patHeight << std::endl;

    uint8_t img[imgWidth*imgHeight], pat[patWidth*patHeight];
    stbiToGrayscale(target_image, img, imgWidth, imgHeight);
    stbiToGrayscale(pattern_image, pat, patWidth, patHeight);
    auto result = search(img, pat, imgWidth, imgHeight, patWidth, patHeight);
    std::cout << result.x << ", " << result.y << ": " << result.value << std::endl;

    stbi_image_free(target_image);
    stbi_image_free(pattern_image);
    return 0;
}
