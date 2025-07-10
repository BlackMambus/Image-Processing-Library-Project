#ifndef SIMPLE_IMAGE_LIB_HPP
#define SIMPLE_IMAGE_LIB_HPP

#include <string>
#include <vector>

class Image {
public:
    int width, height, channels;
    std::vector<unsigned char> data;

    Image();
    Image(const std::string& filename);
    bool load(const std::string& filename);
    bool save(const std::string& filename) const;
    void to_grayscale();
    void invert_colors();
    void resize(int new_width, int new_height);
};

#endif
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION

#include "SimpleImageLib.hpp"
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize.h"

#include <iostream>

Image::Image() : width(0), height(0), channels(0) {}

Image::Image(const std::string& filename) {
    load(filename);
}

bool Image::load(const std::string& filename) {
    unsigned char* img = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (!img) {
        std::cerr << "Failed to load image: " << filename << std::endl;
        return false;
    }
    data.assign(img, img + width * height * channels);
    stbi_image_free(img);
    return true;
}

bool Image::save(const std::string& filename) const {
    int success = stbi_write_png(filename.c_str(), width, height, channels, data.data(), width * channels);
    return success != 0;
}

void Image::to_grayscale() {
    if (channels < 3) return;
    for (int i = 0; i < width * height; ++i) {
        int idx = i * channels;
        unsigned char gray = static_cast<unsigned char>(
            0.299 * data[idx] + 0.587 * data[idx + 1] + 0.114 * data[idx + 2]);
        data[idx] = data[idx + 1] = data[idx + 2] = gray;
    }
}

void Image::invert_colors() {
    for (auto& pixel : data) {
        pixel = 255 - pixel;
    }
}

void Image::resize(int new_width, int new_height) {
    std::vector<unsigned char> resized(new_width * new_height * channels);
    stbir_resize_uint8(data.data(), width, height, 0,
                       resized.data(), new_width, new_height, 0, channels);
    data = std::move(resized);
    width = new_width;
    height = new_height;
}
#include "SimpleImageLib.hpp"
#include <iostream>

int main() {
    Image img("images/sample.bmp");

    img.to_grayscale();
    img.invert_colors();
    img.resize(200, 200);

    if (img.save("images/output.png")) {
        std::cout << "Image processed and saved successfully!" << std::endl;
    } else {
        std::cerr << "Failed to save image." << std::endl;
    }

    return 0;
}



