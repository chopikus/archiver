#include "../error_handler/error_handler.h"
#include "writer.h"
#include <iostream>


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
void Writer::WriteAny(const string& s) {
    for (size_t i = 0; i < s.size(); ++i) {
        Writer::Write1(s[i]=='1');
    }
}

void Writer::Finish() {
    if (buf_pos_ < 8) { 
        if (!os_.good()) {
            ErrorHandler::foundError(ErrorHandler::WRITE_ERROR);
        }
        os_.write((char*) &buf_, 1);
    }
    os_.close();
}
