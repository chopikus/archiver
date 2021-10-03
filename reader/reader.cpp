#include "reader.h"

Reader::Reader(const std::string& file_path) : file_(file_path, std::ios::binary | std::ios::ate), file_path_{file_path} {
    if (file_.tellg() != -1) {
        file_size_ = file_.tellg();
    }
    file_.seekg(0);
}

unsigned char Reader::read_next_byte() {
    char buffer;

    file_.read(&buffer, 1);
    ++bytes_read_;

    return buffer;
}

bool Reader::has_next_byte() const {
    return bytes_read_ < file_size_;
}

std::string Reader::file_name() const {
    return file_path_.substr(file_path_.find_last_of("/\\") + 1);
}

size_t Reader::file_size() {
    return file_size_;
}
