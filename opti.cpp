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

    std::vector<std::unordered_map<std::string, int>> countsVec(numThreads);

    std::clock_t start = std::clock();

    #pragma omp parallel num_threads(numThreads)
    {
        int threadId = omp_get_thread_num();
        std::ifstream file(filePath);
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        std::streampos chunkSize = fileSize / numThreads;
        std::streampos startPosition = chunkSize * threadId;
        std::streampos endPosition = threadId == numThreads - 1 ? fileSize : startPosition + chunkSize;
        file.seekg(startPosition, std::ios::beg);

        std::unordered_map<std::string, int> localCounts; // Local counts for each thread
        std::string word;
        while (file.tellg() < endPosition && file >> word) {
            localCounts[word]++;
        }

        // Combine local counts into global counts with a critical section
        #pragma omp critical
        {
            for (const auto& pair : localCounts) {
                countsVec[threadId][pair.first] += pair.second;
            }
        }
    }

    std::unordered_map<std::string, int> totalCounts = mergeCounts(countsVec);

    std::clock_t end = std::clock();

    std::cout << "Word count completed in " << (end - start) / (double)CLOCKS_PER_SEC << " seconds\n";

    return 0;
}