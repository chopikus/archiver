#pragma once
#include "../reader/reader.h"
#include "../min_priority_queue/min_priority_queue.h"

#include <map>
#include <string>

class Archiver {
    public:
        Archiver(const std::string& file_path);
        void CompressTo(const std::string& file_path);
        void DecompressTo(const std::string& file_path);

    private:
        std::map<uint16_t, uint64_t> GetFrequencies();
        std::string file_path_;
        Reader reader_;
};
