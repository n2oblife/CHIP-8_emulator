#pragma once

#include <gtest/gtest.h>
#include <SDL2/SDL.h>
#include <vector>
#include <random>
#include <fstream>
#include <limits>
#include <stdexcept>

// TODO: should look at formal testing to handle all the cases

template<typename T=uint8_t>
std::vector<T> generateRandomData(size_t size=8);

// Uses the generateRandomData function
template<typename T=uint8_t>
std::vector<T> generateTempFile(const std::string& tempFilePath, size_t size=8);