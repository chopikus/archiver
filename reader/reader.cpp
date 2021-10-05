#include "reader.h"
#include <iostream>

Reader::Reader(const std::string& file_path) : file_(file_path, std::ios::binary | std::ios::ate), file_path_{file_path} {
    if (file_.tellg() != -1) {
        file_size_ = file_.tellg();
    }
    file_.seekg(0);
}

bool Reader::IsEOF() {
    return (bytes_read_ >= file_size_ && buf_pos_ == 8);
}

uint8_t Reader::Read8() {
    uint8_t result;
    for (size_t i = 0; i < 8; ++i) {
        result *= 2;
        result += Read1();
    }
    return result;
}

uint16_t Reader::Read9() {
    uint16_t result;
    for (size_t i = 0; i < 9; ++i) {
        result *= 2;
        result += Read1();
    }
    return result;
}

unsigned char Reader::ReadByte() {
    char buffer;

    file_.read(&buffer, 1);
    ++bytes_read_;
    return buffer;
}

bool Reader::Read1() {
    if (buf_pos_ < 8) {
        return (buf_ >> (7 - buf_pos_++)) & 1;
    } else if (bytes_read_ < file_size_) {
        buf_ =  ReadByte();
        buf_pos_ = 1;
        return (buf_ >> 7) & 1;
    } else
        return false;
}

std::string Reader::FileName() const {
    return file_path_.substr(file_path_.find_last_of("/\\") + 1);
}

size_t Reader::FileSize() {
    return file_size_;
}
