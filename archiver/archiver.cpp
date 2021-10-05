#include "archiver.h"
#include "../error_handler/error_handler.h"
#include <iostream>

using std::map;
using std::pair;
using std::sort;
using std::string;
using std::unordered_map;
using std::vector;

const uint16_t FILENAME_END = 256;
const uint16_t ONE_MORE_FILE = 257;
const uint16_t ARCHIVE_END = 258;
const uint16_t TRIE_SIZE = 1000;
const uint16_t NEW_VERTEX_START = 259;
const uint16_t MAX_SYMBOLS = 259;
const uint16_t MAX_FILENAME_LEN = 255;
const char PATH_DELIMITER = '/';

string Increment(string s) {
    ++s.back();
    intmax_t i = s.size() - 1;
    while (i >= 0) {
        if (s[i] == '2'){
            s[i] = '0'; 
            if (i == 0) {
                s = "1" + s;
            } else {
                ++s[i-1];
            }
        } else {
            break;
        }
        --i;
    }
    return s;
}

bool CanonicalCompare(const pair<uint16_t, string>& p1, const pair<uint16_t, string>& p2) {
    if (p1.second.size() != p2.second.size()) {
        return p1.second.size() < p2.second.size();
    }
    return p1.second < p2.second;
}

Archiver::Archiver(const std::string& file_path) : file_path_{file_path} {};

void Archiver::CompressTo(Writer& w, bool is_last_file) {
    map<uint16_t, uint64_t> freqs = GetFrequencies(); 
    vector<pair<uint16_t, string> > codes;
    map<uint16_t, string> code_of_symbol;

    MinPriorityQueue q{};
    Trie trie(TRIE_SIZE);
    for (auto [symbol, freq] : freqs) {
        q.Push(symbol, freq);
    }
    uint16_t v = NEW_VERTEX_START;
    while (!q.Empty()) {
        KeyPriority kp = q.Pop();
        if (q.Empty()) {
            // root vertex
            vector<pair<uint16_t, string> > leaves = trie.LeavesFrom(v-1);
            sort(leaves.begin(), leaves.end(), CanonicalCompare);
            for (auto& ch : leaves[0].second) {
                ch = '0';
            }
            for (size_t i = 1; i < leaves.size(); ++i) {
                size_t old_size = leaves[i].second.size();
                leaves[i].second = Increment(leaves[i - 1].second);
                while (leaves[i].second.size() < old_size) {
                    leaves[i].second.push_back('0');
                }
            }
            for (auto [number, path] : leaves) {
                code_of_symbol[number] = path;
            } 
            codes = leaves;
            break;
        } else {
            KeyPriority kp2 = q.Pop();
            trie.AddChild(v, kp.key);
            trie.AddChild(v, kp2.key);
            q.Push(v, kp.priority + kp2.priority);
            ++v;
        }
    }
    w.Write9(static_cast<uint16_t>(freqs.size()));
    for (auto [symbol, path] : codes) {
        w.Write9(symbol);
    }
    map<uint16_t, uint16_t> codes_with_size;
    uint16_t max_symbol_code_size = 0;
    for (auto& [symbol, code] : codes) {
        ++codes_with_size[code.size()];
        max_symbol_code_size = std::max(max_symbol_code_size, static_cast<uint16_t>(code.size()));
    }
    for (uint16_t i = 1; i <= max_symbol_code_size; ++i) {
        w.Write9(codes_with_size[i]);
    }
    Reader reader = Reader(file_path_);
    string file_name = reader.FileName();
    for (char& ch : file_name) {
        w.WriteAny(code_of_symbol[ch]);
    }
    w.WriteAny(code_of_symbol[FILENAME_END]); 
    while (!reader.IsEOF()) {
        uint8_t byte = reader.Read8();
        w.WriteAny(code_of_symbol[byte]);
    }
    if (is_last_file) {
        w.WriteAny(code_of_symbol[ARCHIVE_END]);
    } else {
        w.WriteAny(code_of_symbol[ONE_MORE_FILE]);
    }
}

uint16_t Archiver::ReadSymbol(Reader& reader, unordered_map<string, uint16_t>& code_to_symbol) {
    string buf;
    while (!reader.IsEOF()) {
        buf+='0';
        if (reader.Read1()) {
            buf.back()++;
        }
        if (code_to_symbol.count(buf)) {
            return code_to_symbol[buf];
        }
    }
    ErrorHandler::foundError(ErrorHandler::CANT_DECODE_SYMBOL);
    return 0;
}

bool Archiver::DecompressOneFile(const string& file_path, Reader& reader) {
    uint16_t symbol_count = reader.Read9Reversed();
    if (symbol_count == 0) {
        return false;
    } 
    if (symbol_count > MAX_SYMBOLS) {
        ErrorHandler::foundError(ErrorHandler::CANT_READ_CODES);
    }
    vector<uint16_t> alphabet;
    for (uint16_t i = 0; i < symbol_count; ++i) {
        alphabet.push_back(reader.Read9Reversed()); 
    }
    uint16_t symbols_sum = 0;
    vector<uint16_t> symbols_with_size;
    while (symbols_sum != symbol_count) {
        if (symbols_sum > symbol_count || reader.IsEOF()) {
            ErrorHandler::foundError(ErrorHandler::CANT_READ_CODES);
        }
        symbols_with_size.push_back(reader.Read9Reversed());
        symbols_sum += symbols_with_size.back();
    }
    uint16_t max_symbol_code_size = symbols_with_size.size();
    vector<string> canonical_codes;
    for (size_t i = 0; i < max_symbol_code_size; ++i) {
        uint16_t symbols = symbols_with_size[i];
        for (size_t j = 0; j < symbols; ++j) {
            canonical_codes.push_back("");
            for (size_t k = 0; k < i+1; ++k) {
                canonical_codes.back().push_back('0');
            }
        }
    }
    unordered_map<string, uint16_t> code_to_symbol;
    for (size_t i = 1; i < canonical_codes.size(); ++i) {
        size_t prev_size = canonical_codes[i].size();
        canonical_codes[i] = Increment(canonical_codes[i-1]);
        if (canonical_codes[i].size() > prev_size) {
            ErrorHandler::foundError(ErrorHandler::CANT_READ_CODES);
        }
        while (canonical_codes[i].size() < prev_size) {
            canonical_codes[i].push_back('0');
        }
    }
    
    for (size_t i = 0; i < alphabet.size(); ++i) {
        code_to_symbol[canonical_codes[i]] = alphabet[i];    
    }
    string file_name = "";
    bool found_filename_end = false;
    for (size_t i = 0; i < MAX_FILENAME_LEN; ++i) {
        uint16_t symbol = ReadSymbol(reader, code_to_symbol);
        if (symbol == FILENAME_END) {
            found_filename_end = true;
            break;
        }
        if (symbol >= 256) {
            ErrorHandler::foundError(ErrorHandler::CANT_DECODE_SYMBOL);
        } else {
            file_name += symbol;
        }
    }
    if (!found_filename_end) {
        ErrorHandler::foundError(ErrorHandler::NO_FILENAME_END);
    }
    Writer writer(file_path+PATH_DELIMITER+file_name);
    bool found_file_end = false;
    while (!reader.IsEOF()) {
        uint16_t symbol = ReadSymbol(reader, code_to_symbol); 
        if (symbol == ONE_MORE_FILE || symbol == ARCHIVE_END) {
            found_file_end = true;
            break;
        }
        if (symbol >= 256) {
            ErrorHandler::foundError(ErrorHandler::CANT_DECODE_SYMBOL);
        } else {
            writer.Write8(symbol); 
        }
    }
    if (!found_file_end) {
        ErrorHandler::foundError(ErrorHandler::NO_FILE_END);
    }
    writer.Finish();
    return true;
}

void Archiver::DecompressTo(const std::string& path) { 
    Reader reader(file_path_);
    while (!reader.IsEOF()) {
        bool is_not_empty = DecompressOneFile(path, reader);
        if (!is_not_empty && !reader.IsEOF()) {
            ErrorHandler::foundError(ErrorHandler::NO_SYMBOLS_IN_FILE);
        }
    }
}

map<uint16_t, uint64_t> Archiver::GetFrequencies() {
    map<uint16_t, uint64_t> result;
    Reader reader = Reader(file_path_);
    while (!reader.IsEOF()) {
        uint8_t byte = reader.Read8();
        ++result[byte];
    }
    string file_name = reader.FileName();
    for (const uint8_t& ch : file_name) {
        ++result[ch];
    }
    ++result[FILENAME_END];
    ++result[ONE_MORE_FILE];
    ++result[ARCHIVE_END];
    return result; 
}
