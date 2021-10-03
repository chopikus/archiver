#include <queue>
#include <string>
#include "trie.h"

using std::pair;
using std::queue;
using std::string;
using std::vector;

Trie::Trie(uint16_t size): children_{new pair<uint16_t, uint16_t>[size]}, size_{size} {
    for (uint16_t i = 0; i < size; ++i) {
        children_[i].first = i;
        children_[i].second = i;
    }
};
Trie::~Trie() {
    delete[] children_;
}

void Trie::AddChild(uint16_t parent, uint16_t child) {
    if (parent >= size_ || child >= size_) { 
        return; 
    }
    if (children_[parent].first == parent) {
        children_[parent].first = child;
    } else if (children_[parent].second == parent) {
        children_[parent].second = child;   
    }
}

vector<pair<uint16_t, string> > Trie::LeavesFrom(uint16_t start) {
    if (start >= size_) {
        return vector<pair<uint16_t, string> >();
    }
    queue<pair<uint16_t, string> > q;
    q.push({start, ""});
    vector<pair<uint16_t, string> > result;
    while (!q.empty()) {
        pair<uint16_t, string> qt = q.front();
        q.pop();
        uint16_t v = qt.first;
        if (v >= size_) {
            continue;
        }
        string path = qt.second;
        if (children_[v].first == v && children_[v].second == v) {
            result.push_back({v, path});
        }
        if (children_[v].first != v) {
            q.push({children_[v].first, path + "0"});
        }
        if (children_[v].second != v) {
            q.push({children_[v].second, path + "1"});
        }
    }
    return result;
}
