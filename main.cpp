#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

std::string generateWord() {
    static const char charset[] =
        "abcdefghijklmnopqrstuvwxyz";
    const int max_length = 10;
    int length = rand() % max_length + 1;
    std::string word;
    word.reserve(length);
    for (int i = 0; i < length; ++i) {
        word += charset[rand() % (sizeof(charset) - 1)];
    }
    return word;
}

void generateTextFile(const std::string& filePath, int sizeGB) {
    long long wordsNeeded = static_cast<long long>(sizeGB) * 1024 * 1024 * 128;
    const int blockSize = 1024 * 1024;

    std::ofstream file(filePath);
    if (!file) {
        std::cerr << "Error opening file for writing: " << filePath << std::endl;
        return;
    }

    std::clock_t start = std::clock();

    while (wordsNeeded > 0) {
        int block = std::min(static_cast<long long>(blockSize), wordsNeeded);
        std::vector<std::string> words(block);
        for (int i = 0; i < block; ++i) {
            words[i] = generateWord();
        }
        for (const auto& word : words) {
            file << word << " ";
        }
        wordsNeeded -= block;
    }

    std::clock_t end = std::clock();
    std::cout << "Time taken to generate file: " << (end - start) / (double)CLOCKS_PER_SEC << " seconds\n";
}

int main() {
    const std::string filePath = "C:\\Users\\pc\\Desktop\\9no Semestre\\Big Data\\20gbfile.txt";
    const int sizeGB = 1;
    srand(time(nullptr));
    generateTextFile(filePath, sizeGB);
    
    return 0;
}