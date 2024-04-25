#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <ctime>
#include <omp.h>
#include <vector>

std::unordered_map<std::string, int> wordCount(const std::string& filePath) {
    std::unordered_map<std::string, int> counts;
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return counts;
    }

    std::string word;
    while (file >> word) {
        counts[word]++;
    }

    return counts;
}

std::unordered_map<std::string, int> mergeCounts(const std::vector<std::unordered_map<std::string, int>>& countsVec) {
    std::unordered_map<std::string, int> mergedCounts;
    for (const auto& counts : countsVec) {
        for (const auto& pair : counts) {
            mergedCounts[pair.first] += pair.second;
        }
    }
    return mergedCounts;
}

int main() {
    const std::string filePath = "texto_20GB.txt";
    const int numThreads = omp_get_max_threads();
    long long totalWords = 0;

    std::clock_t start = std::clock();

    #pragma omp parallel num_threads(numThreads)
    {
        std::ifstream file(filePath);
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        std::streampos chunkSize = fileSize / numThreads;
        std::streampos startPosition = chunkSize * omp_get_thread_num();
        std::streampos endPosition = omp_get_thread_num() == numThreads - 1 ? fileSize : startPosition + chunkSize;
        file.seekg(startPosition, std::ios::beg);

        std::string word;
        long long localWordCount = 0;
        while (file.tellg() < endPosition && file >> word) {
            ++localWordCount;
        }

        #pragma omp atomic
        totalWords += localWordCount;
    }

    std::clock_t end = std::clock();

    std::cout << "Total words: " << totalWords << std::endl;
    std::cout << "Word count completed in " << (end - start) / (double)CLOCKS_PER_SEC << " seconds\n";

    return 0;
}