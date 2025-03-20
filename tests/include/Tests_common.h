#pragma once

#include <cstdint>
#include <array>
#include <gtest/gtest.h>
#include <SDL2/SDL.h>
#include <random>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <limits>

// TODO: should look at formal testing to handle all the cases

template<typename T = uint8_t>
T generateRandomInt(const int maxValue=16) {
    std::random_device rd;  // Seed
    std::mt19937 gen(rd()); // Mersenne Twister PRNG
    std::uniform_int_distribution<T> dist(1, maxValue);
    return dist(gen);
}


template<typename T = uint8_t, size_t size = 8>
std::array<T, size> generateRandomData() {
    std::array<T, size> data;
    std::random_device rd;  // Seed
    std::mt19937 gen(rd()); // Mersenne Twister PRNG
    std::uniform_int_distribution<T> dist(
    std::numeric_limits<T>::min(), std::numeric_limits<T>::max());

    for (size_t i = 0; i < size; ++i) {
        data[i] = dist(gen);
    }

    return data;
}

template<typename T = uint8_t, size_t size = 8>
void writeFile(const std::string& tempFilePath, const std::array<T, size>& testData) {
    std::ofstream outFile(tempFilePath, std::ios::binary);
    if (!outFile.is_open()) {
        throw std::runtime_error("Failed to create test file: " + tempFilePath);
    }

    outFile.write(reinterpret_cast<const char*>(testData.data()), testData.size());
    if (!outFile) {
        throw std::runtime_error("Failed to write to file: " + tempFilePath);
    }

    outFile.close();
}

// Use generateRandomData() to generate the tempFile
template<typename T = uint8_t, size_t size = 8>
std::array<T, size> generateTempFile(const std::string& tempFilePath) {
    std::array<T, size> testData = generateRandomData<T, size>();
    writeFile(tempFilePath, testData);
    return testData;
}

template<typename T, size_t size>
std::string printHexArray(const std::array<T, size>& arr) {
    std::ostringstream oss;
    oss << "[ ";
    for (const auto& val : arr) {
        oss << "0x" << std::hex << static_cast<int>(val) << " ";
    }
    oss << "]";
    return oss.str();
}