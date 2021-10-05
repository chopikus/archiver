#include <cstdlib>
#include <iostream>
#include <string>

class ErrorHandler {
    public:
        static void foundError(int which); 
        static const int READ_ERROR = 0;
        static const int WRITE_ERROR = 1;
        static const int CANT_DECODE_SYMBOL = 2;
        static const int NO_FILENAME_END = 3;
        static const int NO_FILE_END = 4;
        static const int NO_SYMBOLS_IN_FILE = 5;
        static const int FILE_NOT_FOUND_OR_EMPTY = 6;
        static const int TRIE_ERROR = 7;
        static const int CANT_READ_CODES = 8;   
        static const int CONFUSING_ARGUMENTS = 9;
        static const int WRONG_ARGUMENTS = 10;
        static const int NO_ARGUMENTS = 11;
};
