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
        Archiver(std::vector<std::string> file_paths);
        void CompressTo(std::string compress_path);
        void Decompress();

    private:
        void DecompressOneFile(const std::string& file_path, Reader& reader); 
        void CompressOneFile(const std::string& file_path, const std::string& compress_path, bool is_last_file);
        uint16_t ReadSymbol(Reader& reader, std::unordered_map<std::string, uint16_t>& code_to_symbol); 
        std::vector<std::string> file_paths_;
};
