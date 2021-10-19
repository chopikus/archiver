#include "writer.h"

using std::ofstream;
using std::string;

namespace {
const size_t bufsize = 256*1024;
char write_buf[bufsize];
};  // namespace

Writer::Writer(const string& file_path) : os_(file_path, std::ios::binary | std::ios::out) {
    os_.rdbuf()->pubsetbuf(write_buf, bufsize);
};

void Writer::Write(uint16_t code, uint8_t length) {
    buffer_ += (code << bits_in_buffer_);
    bits_in_buffer_ += length;
    while (bits_in_buffer_ >= 8) {
        uint8_t byte = buffer_ % (1 << 8);
        os_.write(reinterpret_cast<char*>(&byte), 1);
        buffer_ >>= 8;
        bits_in_buffer_ -= 8;
    }            
}

void Writer::Finish() { 
    while (bits_in_buffer_ > 0) {
        uint8_t byte = buffer_ % (1 << 8);
        os_.write(reinterpret_cast<char*>(&byte), 1);
        buffer_ >>= 8;
        if (bits_in_buffer_ >= 8) {
            bits_in_buffer_ -= 8;
        } else {
            break;
        }
    } 
    os_.close();
}
