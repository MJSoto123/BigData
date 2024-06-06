#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <ctime>
#include <set>
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
    const int numThreads = omp_get_max_threads();
    cout << "Num Threads: " << numThreads << "\n";

    unordered_map<string, set<int>> countsVec;

    clock_t start = clock();

    #pragma omp parallel num_threads(numThreads)
    {
        int threadId = omp_get_thread_num();
        const string filein = "Input/test2/file_" + to_string(threadId) + ".txt";

        ifstream file(filein);
        unordered_map<string, int> localCounts;
        string word;
        while (file >> word) localCounts[word]++;

        #pragma omp critical
        {
            for (const auto& pair : localCounts) {
                countsVec[pair.first].insert(threadId);
            }
        }
    }

    // unordered_map<string, int> totalCounts = mergeCounts(countsVec);

    clock_t end = clock();
    cout << "Word count completed in " << (end - start) / (double)CLOCKS_PER_SEC << " seconds\n";

    freopen("Output/test2.txt", "w", stdout);
    long long finalCount = 0;
    for(auto x : countsVec) {
        cout << x.first << " => ";
        for(auto y : x.second){
            cout << y << " ";
        }
        cout << "\n";
    }
    return 0;
}