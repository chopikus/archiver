#pragma once
#include "../reader/reader.h"
#include "../writer/writer.h"
#include "../min_priority_queue/min_priority_queue.h"
#include "../trie/trie.h"

#include <algorithm>
#include <cstdint>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class Archiver {
    public:
        Archiver(const std::string& file_path);
        void CompressTo(Writer& w, bool is_last_file);
        void DecompressTo(const std::string& file_path);

    private:
        std::map<uint16_t, uint64_t> GetFrequencies();
        bool DecompressOneFile(const std::string& file_path, Reader& reader); 
        void ReadSymbol(Reader& reader);
        uint16_t ReadSymbol(Reader& reader, std::unordered_map<std::string, uint16_t>& code_to_symbol); 
        std::string file_path_;
};
