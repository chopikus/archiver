#include "archiver.h"

#include <iostream>

using std::map;
using std::string;

const uint16_t FILENAME_END = 256;
const uint16_t ONE_MORE_FILE = 257;
const uint16_t ARCHIVE_END = 258;
const uint16_t TRIE_SIZE = 1000;
const uint16_t NEW_VERTEX_START = 259;

Archiver::Archiver(const std::string& file_path) : file_path_{file_path}, reader_(file_path) {};

void Archiver::CompressTo(const string& write_path) {
    map<uint16_t, uint64_t> freqs = GetFrequencies(); 
    MinPriorityQueue q{}; 
    Trie trie(TRIE_SIZE);
    for (auto [symbol, freq] : freqs) {
        q.Push(symbol, freq);
    }
    uint16_t v = NEW_VERTEX_START;
    while (!q.Empty()) {
        KeyPriority kp = q.Pop();
        if (q.Empty()) {
            // root vertex
            std::cout << v-1 << std::endl;
            break;
        } else {
            KeyPriority kp2 = q.Pop();
            trie.AddChild(v, kp.key);
            trie.AddChild(v, kp2.key);
            q.Push(v);
            ++v;
        }
    }
}

void Archiver::DecompressTo(const string& write_path) {
}

map<uint16_t, uint64_t> Archiver::GetFrequencies() {
    map<uint16_t, uint64_t> result;
    while (reader_.has_next_byte()) {
        uint8_t byte = reader_.read_next_byte();
        ++result[byte];
    }
    string file_name = reader_.file_name();
    for (const uint8_t& ch : file_name) {
        ++result[ch];
    }
    return result; 
}
