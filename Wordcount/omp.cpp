#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <ctime>
#include <omp.h>
#include <vector>

using namespace std;

unordered_map<string, int> mergeCounts(const vector<unordered_map<string, int>>& countsVec) {
    unordered_map<string, int> mergedCounts;
    for (const auto& counts : countsVec) {
        for (const auto& pair : counts) {
            mergedCounts[pair.first] += pair.second;
        }
    }
    return mergedCounts;
}

int main() {
    // const string filein = "Input/words.txt";
    const string filein = "Input/18000mb_file_0.txt";
    // const string filein = "Input/1000mb_file_0.txt";
    const int numThreads = omp_get_max_threads();

    vector<unordered_map<string, int>> countsVec(numThreads);

    clock_t start = clock();

    #pragma omp parallel num_threads(numThreads)
    {
        int threadId = omp_get_thread_num();
        ifstream file(filein);
        file.seekg(0, ios::end);
        streampos fileSize = file.tellg();
        streampos chunkSize = fileSize / numThreads;
        streampos startPosition = chunkSize * threadId;
        streampos endPosition = threadId == numThreads - 1 ? fileSize : startPosition + chunkSize;
        file.seekg(startPosition, ios::beg);

        unordered_map<string, int> localCounts;
        string word;
        while (file.tellg() < endPosition && file >> word) {
            localCounts[word]++;
        }

        #pragma omp critical
        {
            for (const auto& pair : localCounts) {
                countsVec[threadId][pair.first] += pair.second;
            }
        }
    }

    unordered_map<string, int> totalCounts = mergeCounts(countsVec);

    clock_t end = clock();
    cout << "Word count completed in " << (end - start) / (double)CLOCKS_PER_SEC << " seconds\n";

    freopen("Output/frecc_18000mb.txt", "w", stdout);
    long long finalCount = 0;
    for(auto x : totalCounts) {
        cout << x.first << " " << x.second << "\n";
        finalCount += x.second;
    }
    cout << "Final Count " << finalCount << "\n";
    return 0;
}