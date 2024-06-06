/*
    Genero 1gb en 5min
*/ 

#include <set>
#include <map>
#include <queue>
#include <stack>
#include <limits>
#include <bitset>
#include <time.h>
#include <math.h>
#include <vector>
#include <string>
#include <climits>
#include <numeric>
#include <assert.h>
#include <iostream>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <unordered_map>
#include <cstdlib>
#include <chrono>
#include <random>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <ctime>
#include <omp.h>
#include <vector>
#define ll long long
#define vi vector<int>
#define vvi vector<vi>
#define vvl vector<vl>
#define pii pair<int,int>
#define vl vector<long long>
#define vii vector<pair<int,int>>

using namespace std;

inline ll gcd(ll a, ll b) { while (b != 0) swap(b, a %= b); return a; }

int rand(int l, int r){
  static mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
  uniform_int_distribution<int> ludo(l, r); 
  return ludo(rng);
}

string generateRandomWord() {
    string word;
    for (int i = 0; i < 10; ++i) {
        char randomChar = 'a' + rand(0, 25);
        word += randomChar;
    }
    return word;
}

vector<string> mp;
long long uniqueWords = 0;
void readF(){
    const char* filename = "Input/words.txt"; 
    ifstream file(filename);
    string s;
    while (file >> s) uniqueWords++, mp.emplace_back(s);
    file.close();
    cout << "Total Unique Words " << uniqueWords << "\n";
}

string getRandomWord(){
    return  mp[rand(0, uniqueWords - 1)];
}


int main() {
    int mb, num; 
    readF();
    mb = 18000; num = 0;
    const long long fileSize = 1LL * mb * 1024 * 1024;
    string temp = "Input/"+  to_string(mb) + "mb_file_" + to_string(num) + ".txt";
    cout << temp << "\n";
    const char* filename = temp.c_str();

    FILE* file = fopen(filename, "w");

    const int numThreads = omp_get_max_threads();
    #pragma omp parallel num_threads(numThreads)
    {
        long long threadId = omp_get_thread_num();
        long long chunkSize = fileSize / numThreads;
        long long startPosition = chunkSize * threadId;
        long long endPosition = (threadId == numThreads - 1) ? fileSize : startPosition + chunkSize;

        for (long long i = startPosition; i < endPosition; i += 10) {
            string word = getRandomWord() + " ";
            fwrite(word.c_str(), sizeof(char), word.length(), file);
            fwrite(" ", sizeof(char), 1, file);
        }
    }

    fclose(file);
    cout << "Archivo de "<< fileSize/ (1024 * 1024) <<" MB generado exitosamente: " << filename << endl;
    return 0;
}
