#include "../error_handler/error_handler.h"
#include "writer.h"
#include <iostream>


using std::ofstream;
using std::string;

Writer::Writer(const string& file_path) : os_(file_path, std::ios::binary | std::ios::out) {};

void Writer::CheckAndWrite() {
    if (buf_.size() == 8) {
        uint8_t x = 0;
        for (size_t i = 0; i < buf_.size(); ++i) {
            x *= 2;
            x += buf_[i];
        }
        if (!os_.good()) {
            ErrorHandler::foundError(ErrorHandler::WRITE_ERROR);
        }
        os_.write((char*) &x, 1);
        buf_.clear();
    }
}

void Writer::Write1(bool b) {
    buf_.push_back(b);
    CheckAndWrite();    
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
    if (!buf_.empty()) {
        uint8_t x = 0;
        for (size_t i = 0; i < buf_.size(); ++i) {
            x *= 2;
            x += buf_[i];
        }
        if (!os_.good()) {
            ErrorHandler::foundError(ErrorHandler::WRITE_ERROR);
        }
        x <<= (8 - buf_.size());
        os_.write((char*) &x, 1);
        buf_.clear();
    }
    os_.close();
}
