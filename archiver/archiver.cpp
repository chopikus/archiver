#include "archiver.h"

#include <iostream>

using std::map;
using std::pair;
using std::sort;
using std::string;
using std::vector;

const uint16_t FILENAME_END = 256;
const uint16_t ONE_MORE_FILE = 257;
const uint16_t ARCHIVE_END = 258;
const uint16_t TRIE_SIZE = 1000;
const uint16_t NEW_VERTEX_START = 259;

string Increment(string s) {
    ++s.back();
    intmax_t i = s.size() - 1;
    while (i >= 0) {
        if (s[i] == '2'){
            s[i] = '0'; 
            if (i == 0) {
                s = "1" + s;
            } else {
                ++s[i-1];
            }
        }
        else {
            break;
        }
    }
    return s;
}

bool CanonicalCompare(const pair<uint16_t, string>& p1, const pair<uint16_t, string>& p2) {
    if (p1.second.size() != p2.second.size()) {
        return p1.second.size() < p2.second.size();
    }
    return p1.second < p2.second;
}

Archiver::Archiver(const std::string& file_path) : file_path_{file_path} {};

void Archiver::CompressTo(Writer& w) {
    map<uint16_t, uint64_t> freqs = GetFrequencies(); 
    vector<pair<uint16_t, string> > paths;
    map<uint16_t, string> paths_by_symbol;

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
            vector<pair<uint16_t, string> > leaves = trie.LeavesFrom(v-1);
            sort(leaves.begin(), leaves.end(), CanonicalCompare);
            for (auto& ch : leaves[0].second) {
                ch = '0';
            }
            for (size_t i = 1; i < leaves.size(); ++i) {
                size_t old_size = leaves[i].second.size();
                leaves[i].second = Increment(leaves[i - 1].second);
                while (leaves[i].second.size() < old_size) {
                    leaves[i].second.push_back('0');
                }
            }
            for (auto [number, path] : leaves) {
                std::cout << number << ' ' << path << std::endl;
                paths_by_symbol[number] = path;
            } 
            paths = leaves;
            break;
        } else {
            KeyPriority kp2 = q.Pop();
            trie.AddChild(v, kp.key);
            trie.AddChild(v, kp2.key);
            q.Push(v, kp.priority + kp2.priority);
            ++v;
        }
    }
    w.Write9(static_cast<uint16_t>(freqs.size()));
    for (auto [symbol, path] : paths) {
        w.Write9(symbol);
    }
    map<uint16_t, uint16_t> paths_with_size;
    uint16_t max_symbol_code_size = 0;
    for (auto& [symbol, path] : paths) {
        ++paths_with_size[path.size()];
        max_symbol_code_size = std::max(max_symbol_code_size, static_cast<uint16_t>(path.size()));
    }
    for (uint16_t i = 1; i <= max_symbol_code_size; ++i) {
        w.Write9(paths_with_size[i]);
    }
    Reader reader = Reader(file_path_);
    string file_name = reader.file_name();
    for (char& ch : file_name) {
        w.WriteAny(paths_by_symbol[ch]);
    }
    w.WriteAny(paths_by_symbol[FILENAME_END]); 
    while (reader.has_next_byte()) {
        uint8_t byte = reader.read_next_byte();
        w.WriteAny(paths_by_symbol[byte]);
    }
}

void Archiver::DecompressTo(Writer& w) {
}

map<uint16_t, uint64_t> Archiver::GetFrequencies() {
    map<uint16_t, uint64_t> result;
    Reader reader = Reader(file_path_);
    while (reader.has_next_byte()) {
        uint8_t byte = reader.read_next_byte();
        ++result[byte];
    }
    string file_name = reader.file_name();
    for (const uint8_t& ch : file_name) {
        ++result[ch];
    }
    ++result[FILENAME_END];
    ++result[ONE_MORE_FILE];
    ++result[ARCHIVE_END];
    return result; 
}
