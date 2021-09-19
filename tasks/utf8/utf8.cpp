#include "utf8.h"
#include <cstddef>
#include <vector>
#include <string>
#include <iostream>
#include <cstring>

const uint8_t FIRST_BYTE_START[5] = {0, 0, 0b11'000'000, 0b11'100'000, 0b11'110'000};
const uint8_t NEXT_BYTE_START = 0b10'000'000;

size_t CountUtf8Symbols(const std::string& str) {
    const char* c_str = str.c_str();
    size_t symbols = 0;
    while ((*c_str) != '\0') {
        uint8_t byte = *c_str;
        ++c_str;
        ++symbols;
        int ones_in_front = 0;
        for (int i = 7; i >= 0; --i) {
            if ((byte >> i) & 1) {
                ++ones_in_front;
            }
            else {
                break;
            }
        }
        if (ones_in_front == 1 || ones_in_front > 4) {
            break;    // something has gone horribly wrong
        }
        while (ones_in_front > 1 && (*c_str) != '\0') {
            --ones_in_front;
            ++c_str;
        }
    }    
    return symbols;
}

void EncodeUtf8(const std::vector<uint32_t>& codepoints, std::string& str) {
    for (size_t i = 0; i < codepoints.size(); ++i) {
        uint32_t point = codepoints[i];
        uint8_t bits_needed = std::max(1, 32 - __builtin_clz(point));
        for (uint8_t bytes = 1; bytes <= 5; ++bytes) {
            uint8_t max_bits = 0;
            if (bytes == 1) {
                max_bits = 7;
            } else {
                max_bits = (7 - bytes) + 6 * (bytes - 1); 
            }
            if (bits_needed <= max_bits) {
                uint8_t bits_to_first_byte = 0;
                if (bytes == 1) {
                    bits_to_first_byte = 7;
                } else {
                    bits_to_first_byte = (7 - bytes); 
                }
                uint8_t first_piece = (point >> (max_bits - bits_to_first_byte));
                uint8_t first_byte = FIRST_BYTE_START[bytes] + first_piece;
                str += first_byte;
                for (size_t i = 2; i <= bytes; ++i) {
                    uint8_t next_piece = (point >> (6 * (bytes - i))) % (1 << 6);
                    uint8_t next_byte = NEXT_BYTE_START + next_piece;
                    str += next_byte;
                }
                break;        
            }
        }    
    }
}

void DecodeUtf8(const std::string& str, std::vector<uint32_t>& codepoints) {
    const char* c_str = str.c_str();
    while ((*c_str) != '\0') {
        uint8_t first_symbol_byte = *c_str;
        ++c_str;
        int ones_in_front = 0;
        for (int i = 7; i >= 0; --i) {
            if ((first_symbol_byte >> i) & 1) {
                ++ones_in_front;
            }
            else {
                break;
            }
        }
        uint32_t code_point = first_symbol_byte % (1 << (8 - ones_in_front));    
        if (ones_in_front == 1 || ones_in_front > 4) {
            break;  // something has gone horribly wrong
        }
        while (ones_in_front > 1 && (*c_str) != '\0') {
            --ones_in_front;
            code_point *= (1 << 6);
            uint8_t byte = *c_str;
            code_point += byte % (1 << 7);
            ++c_str;
        }
        codepoints.push_back(code_point);
    }
}
