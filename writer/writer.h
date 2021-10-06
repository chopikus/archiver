#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

class Writer {
    public:
        Writer(const std::string& file_path);
        void Write1(bool b);
        void Write8(uint8_t u);
        void Write9(uint16_t u);
        void WriteAny(const std::string& s);
        void Finish();
    private:
        std::ofstream os_;
        uint8_t buf_;
        uint8_t buf_pos_;
        void CheckAndWrite();
};
