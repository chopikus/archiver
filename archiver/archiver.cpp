#include "archiver.h"

using std::back_inserter;
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

Archiver::Archiver(vector<string> file_paths) : file_paths_{file_paths} {};

namespace {
    class CodeAndLength {
        public:
            uint16_t code;
            uint16_t length;
            bool operator<(const CodeAndLength& another) const {
                if (code != another.code) {
                    return code < another.code;
                }
                return length < another.length;
            }
    }; 
    bool SortBySecond(const std::pair<uint16_t, uint16_t>& p1, const std::pair<uint16_t, uint16_t>& p2) {
        if (p1.second != p2.second) {
            return (p1.second < p2.second);
        } else {
            return (p1.first < p2.first);
        }
    }

    map<uint16_t, uint64_t> GetFrequencies(const string& file_path) {
        map<uint16_t, uint64_t> result;
        Reader reader = Reader(file_path);
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

    vector<CodeAndLength> GenerateCanonicalCodes(vector<uint16_t> lengths) {
        if (lengths.size() == 0) {
            return vector<CodeAndLength>();
        }
        vector<CodeAndLength> codes_and_lengths;
        uint16_t code = 0;
        uint16_t code_length = lengths[0];
        codes_and_lengths.push_back({code, code_length});
        for (size_t i = 1; i < lengths.size(); ++i) {
            ++code;
            code <<= (lengths[i] - code_length);
            code_length = lengths[i];
            codes_and_lengths.push_back({code, code_length});
        }
        return codes_and_lengths;
    }

    vector<pair<uint16_t, CodeAndLength> > GetCodesFromData(const string& file_path) {
        map<uint16_t, uint64_t> frequencies = GetFrequencies(file_path);
        MinPriorityQueue priority_queue{};
        Trie trie(TRIE_SIZE);
        for (auto [symbol, freq] : frequencies) {
            priority_queue.Push(symbol, freq);
        }
        uint16_t new_vertex = NEW_VERTEX_START;
        while (!priority_queue.Empty()) {
            auto [first_symbol, first_frequency] = priority_queue.Pop(); 
            if (priority_queue.Empty()) {
                uint16_t root = new_vertex - 1;
                vector<pair<uint16_t, uint16_t> > symbols_and_lengths = trie.LeavesFrom(root);
                vector<uint16_t> lengths;
                sort(symbols_and_lengths.begin(), symbols_and_lengths.end(), SortBySecond);
                for (auto [symbol, length] : symbols_and_lengths) {
                    lengths.push_back(length);
                }
                vector<CodeAndLength> codes_and_lengths = GenerateCanonicalCodes(lengths);
                vector<pair<uint16_t, CodeAndLength> > result;
                for (size_t i = 0; i < symbols_and_lengths.size(); ++i) {
                    uint16_t symbol = symbols_and_lengths[i].first;
                    result.push_back({symbol, codes_and_lengths[i]});
                }
                return result;
            } else {
                auto [second_symbol, second_frequency] = priority_queue.Pop();
                trie.AddChild(new_vertex, first_symbol);
                trie.AddChild(new_vertex, second_symbol);
                priority_queue.Push(new_vertex, first_frequency + second_frequency);
                ++new_vertex;
            }
        }
        return vector<pair<uint16_t, CodeAndLength> > ();
    }

    pair<vector<uint16_t>, vector<CodeAndLength> > ReadArchiveAlphabet(Reader& reader) {
        uint16_t symbol_count = reader.Read9Reversed();
        if (symbol_count > MAX_SYMBOLS) {
            throw "Corrupt archive! (too much symbols)";
        }
        vector<uint16_t> alphabet;
        for (uint16_t i = 0; i < symbol_count; ++i) {
            alphabet.push_back(reader.Read9Reversed()); 
        }
        uint16_t symbols_sum = 0;
        vector<uint16_t> lengths;
        uint16_t current_length = 1;
        while (symbols_sum != symbol_count) {
            if (symbols_sum > symbol_count || reader.IsEOF()) {
                throw "can not read codes!";
            }
            uint16_t count = reader.Read9Reversed();
            for (uint16_t i = 0; i < count; ++i) {
                lengths.push_back(current_length);
                ++symbols_sum;
            }
            ++current_length;
        }
        vector<CodeAndLength> codes_and_lengths = GenerateCanonicalCodes(lengths);
        return {alphabet, codes_and_lengths};
    }

    uint16_t ReadCanonicalCode(Reader& reader, map<CodeAndLength, uint16_t>& code_to_symbol) {
        uint16_t code = 0;
        uint16_t length = 0;
        while (!reader.IsEOF()) {
            code *= 2;
            if (reader.Read1()) {
                ++code;
            }
            length++;
            if (code_to_symbol.count({code, length})) {
                return code_to_symbol[{code, length}];
            }
        }
        throw "Couldn't decode symbol";
    }
}


void Archiver::CompressOneFile(const string& file_path, Writer& writer, bool is_last_file) {
    vector<pair<uint16_t, CodeAndLength> > canonical_codes = GetCodesFromData(file_path);
    map<uint16_t, CodeAndLength> code_of_symbol;
    for (auto [symbol, code_and_length] : canonical_codes) {
        code_of_symbol[symbol] = code_and_length;
    }
    writer.Write9(canonical_codes.size());
    map<uint16_t, uint16_t> codes_with_size;
    uint16_t max_symbol_code_size = 0;
    for (auto& [symbol, code] : canonical_codes) {
        writer.Write9(symbol);
        ++codes_with_size[code.length];
        max_symbol_code_size = std::max(max_symbol_code_size, code.length);
    }
    for (uint16_t i = 1; i <= max_symbol_code_size; ++i) {
        writer.Write9(codes_with_size[i]);
    }
    Reader reader = Reader(file_path);
    string file_name = reader.FileName();
    for (char& ch : file_name) {
        writer.WriteAny(code_of_symbol[ch].code, code_of_symbol[ch].length);
    }
    CodeAndLength file_name_end = code_of_symbol[FILENAME_END];
    CodeAndLength archive_end = code_of_symbol[ARCHIVE_END];
    CodeAndLength one_more_file = code_of_symbol[ONE_MORE_FILE]; 

    writer.WriteAny(file_name_end.code, file_name_end.length); 
    while (!reader.IsEOF()) {
        uint8_t byte = reader.Read8();
        writer.WriteAny(code_of_symbol[byte].code, code_of_symbol[byte].length);
    }
    if (is_last_file) {
        writer.WriteAny(archive_end.code, archive_end.length);
    } else {
        writer.WriteAny(one_more_file.code, one_more_file.length);
    } 
}

void Archiver::DecompressOneFile(const string& decompress_to, Reader& reader) {
    auto [alphabet, canonical_codes] = ReadArchiveAlphabet(reader);
    if (alphabet.size() == 0) {
        return;
    }
    map<CodeAndLength, uint16_t> code_to_symbol;
    for (size_t i = 0; i < alphabet.size(); ++i) {
        code_to_symbol[canonical_codes[i]] = alphabet[i];
    }
    string file_name = "";
    bool found_filename_end = false;
    for (size_t i = 0; i < MAX_FILENAME_LEN; ++i) {
        uint16_t symbol = ReadCanonicalCode(reader, code_to_symbol);
        if (symbol == FILENAME_END) {
            found_filename_end = true;
            break;
        }
        file_name += symbol;
    }
    if (!found_filename_end) {
        throw "Corrupt archive (couldn't find FILENAME_END symbol)";
    }
    Writer writer(decompress_to+PATH_DELIMITER+file_name);
    while (!reader.IsEOF()) {
        uint16_t symbol = ReadCanonicalCode(reader, code_to_symbol);  
        if (symbol == ONE_MORE_FILE || symbol == ARCHIVE_END) {
            break;
        }
        writer.Write8(symbol); 
    }
    writer.Finish();
}

void Archiver::CompressTo(string compress_path) {
    size_t i = 0;
    Writer writer(compress_path);
    for (const string& file_path : file_paths_) {
        CompressOneFile(file_path, writer, (i + 1 == file_paths_.size()));
        ++i;
    } 
    writer.Finish();
}

void Archiver::Decompress() {
    Reader reader(file_paths_[0]);
    while (!reader.IsEOF()) {
        DecompressOneFile(".", reader);
    }
}
