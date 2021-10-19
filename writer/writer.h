#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

class Writer {
    public:
        Writer(const std::string& file_path);
        void Write(uint16_t code, uint8_t length);
        void Finish();
    private:
        std::ofstream os_;
        uint32_t buffer_ = 0;
        uint8_t bits_in_buffer_ = 0;
};
