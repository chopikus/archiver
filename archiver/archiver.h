#pragma once
#include "../reader/reader.h"
#include "../writer/writer.h"
#include "../min_priority_queue/min_priority_queue.h"
#include "../trie/trie.h"

#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>

class Archiver {
    public:
        Archiver(const std::string& file_path);
        void CompressTo(Writer& w);
        void DecompressTo(Writer& w);

    private:
        std::map<uint16_t, uint64_t> GetFrequencies();
        std::string file_path_;
};
