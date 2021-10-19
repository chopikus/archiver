#include "reader.h"

Reader::Reader(const std::string& file_path) : file_(file_path, std::ios::binary | std::ios::ate), file_path_{file_path} {
    if (file_.tellg() != -1) {
        file_size_ = file_.tellg();
    } else {
        throw "File is not found! (or empty)";
    }
    file_.seekg(0);
}

bool Reader::IsEOF() {
    return (bytes_read_ >= file_size_ && available_bits_ == 0);
}

bool Reader::ReadByte() {
    if (bytes_read_ < file_size_) {
        uint8_t byte = 0;
        file_.read(reinterpret_cast<char*>(&byte), 1);
        ++bytes_read_;
        buffer_ += (byte << available_bits_);
        available_bits_ += 8;
        return true;
    }
    return false;
}

uint16_t Reader::Read(const uint8_t bit_count) {
    while (available_bits_ < bit_count) {
        if (!ReadByte()) {
            available_bits_ = bit_count;
        }
    }
    uint16_t result = buffer_ % (1 << bit_count);
    available_bits_ -= bit_count;
    buffer_ >>= bit_count;
    return result;
}

std::string Reader::FileName() const {
    return file_path_.substr(file_path_.find_last_of("/\\") + 1);
}

size_t Reader::FileSize() {
    return file_size_;
}
