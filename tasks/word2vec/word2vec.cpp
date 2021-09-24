#include <cmath>
#include <vector>
#include <stdexcept>
#include "word2vec.h"

using std::string;
using std::vector;

double ScalarOnNorm(const vector<double>& norm1, const vector<double>& norm2) {
    double result = 0;
    for (size_t i = 0; i < std::min(norm1.size(), norm2.size()); i++) {
        result += norm1[i] * norm2[i];
    }
    return result;
}

vector<string> FindClosestWords(const vector<string>& words, const vector<vector<int>>& vectors) {
    vector<vector<double>> normalized_vectors;
    for (const vector<int>& v : vectors) {
        double len = 0;
        for (int x : v) {
            len += x * x;
        }
        len = sqrt(len);
        normalized_vectors.emplace_back(vector<double>());
        for (int x : v) {
            double normalized_x = x;
            normalized_x /= len;
            normalized_vectors.back().push_back(normalized_x);
        }
    }
    double mx = 0;
    for (size_t i = 1; i < normalized_vectors.size(); i++) {
        mx = std::max(mx, ScalarOnNorm(normalized_vectors[0], normalized_vectors[i]));
    }
    vector<string> result;
    for (size_t i = 1; i < normalized_vectors.size(); i++) {
        double sc = ScalarOnNorm(normalized_vectors[0], normalized_vectors[i]);
        if (sc == mx) {
            result.push_back(words[i]);
        }
    }
    return result;
}
