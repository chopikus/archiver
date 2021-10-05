#include "error_handler.h"

using std::string;

string error_text[12] = {
    "Reading error",
    "Writing error",
    "Corrupt archive (can't decode symbol)",
    "Corrupt archive (can't find FILENAME_END symbol)",
    "Corrupt archive (NO_FILE_END)",
    "Corrupt archive (NO_SYMBOLS_IN_FILE)",
    "File not found (or it's empty)",
    "Internal error",
    "Corrupt archive (can't read Huffman codes)",
    "Confusing arguments (use -h for help)",
    "Wrong arguments (use -h for help)",
    "No arguments are provided (use -h for help)",
};

bool is_severe[12] = {
    true,
    true,
    true,
    true,
    false,
    true,
    false,
    true,
    true,
    true,
    true,
    true,
};

void ErrorHandler::foundError(int which) {
    if (is_severe[which]) {
        std::cerr << "ERROR: " << error_text[which] << " (code #" << which << ")" << std::endl;
        exit(EXIT_FAILURE); 
    } else {
        std::cerr << "WARNING: " << error_text[which] << " (code #" << which << ")" << std::endl;
    }
}
