#pragma once

#include <cstdint>
#include <utility>
#include <vector>
#include <string>

class Trie {
    public:
        Trie(uint16_t size); 
        void AddChild(uint16_t parent, uint16_t child);
        std::vector<std::pair<uint16_t, std::string> > LeavesFrom(uint16_t start);
        ~Trie();
    private:
        std::pair<uint16_t, uint16_t>* children_;
        uint16_t size_;
};
