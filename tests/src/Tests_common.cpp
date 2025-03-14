#include "Tests_common.h

// TODO: should look at formal testing to handle all the cases

template<typename T=uint8_t>
std::vector<T> generateRandomData(size_t size=8) {
    std::vector<T> data(size);
    std::random_device rd;  // Seed
    std::mt19937 gen(rd()); // Mersenne Twister PRNG
    std::uniform_int_distribution<T> dist(
        std::numeric_limits<T>::min(), std::numeric_limits<T>::max());

    for (size_t i = 0; i < size; ++i) {
        data[i] = dist(gen);
    }
    
    return data;
}

template<typename T=uint8_t>
void generateTempFile(const std::string& tempFilePath, size_t size=8) {
    // Create a known test file
    std::vector<T> testData = generateRandomData<T>(size);

    std::ofstream outFile(tempFilePath, std::ios::binary);
    if (!outFile.is_open()) throw std::runtime_error("Failed to create test file");
    outFile.write(reinterpret_cast<const char*>(testData.data()), testData.size());
    outFile.close();
}