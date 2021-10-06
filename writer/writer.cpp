#include "writer.h"

using std::ofstream;
using std::string;

Writer::Writer(const string& file_path) : os_(file_path, std::ios::binary | std::ios::out) {};

void Writer::Write1(bool b) {
    ++buf_pos_;
    buf_ *= 2;
    buf_ += b;
    if (buf_pos_ == 8) {
        os_.write((char*) &buf_, 1);
        buf_pos_ = 0;
    }
}

void Writer::Write9(uint16_t u) {
    for (int i = 0; i < 9; ++i) {
        Writer::Write1((u >> i) & 1);
    }
}

void Writer::Write8(uint8_t u) {
    for (int i = 7; i >= 0; --i) {
        Writer::Write1((u >> i) & 1);
    }
}
void Writer::WriteAny(uint16_t code, uint16_t length) {
    for (int i = length - 1; i >= 0; --i) {
        Writer::Write1((code >> i) & 1);
    }
}

void Writer::Finish() {
    if (buf_pos_ > 0 && buf_pos_ < 8) { 
        if (!os_.good()) {
            throw "Writing error!";
        }
        buf_ <<= (8 - buf_pos_);
        os_.write((char*) &buf_, 1);
        buf_pos_ = 0;
    }
    os_.close();
}
