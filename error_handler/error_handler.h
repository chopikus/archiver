#pragma once

#include <cstdlib>
#include <iostream>

class ErrorHandler {
    public:
        static void foundError(int which); 
        static constexpr bool is_severe[9] = {true, true, true, true, false, false, true, true, true};
        static const int READ_ERROR = 0;
        static const int WRITE_ERROR = 1;
        static const int CANT_DECODE_SYMBOL = 2;
        static const int NO_FILENAME_END = 3;
        static const int NO_FILE_END = 4;
        static const int NO_SYMBOLS_IN_FILE = 5;
        static const int FILE_NOT_FOUND_OR_EMPTY = 6;
        static const int TRIE_ERROR = 7;
        static const int CANT_READ_CODES = 8;
};
