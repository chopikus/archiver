#include "error_handler.h"

void ErrorHandler::foundError(int which) {
    if (is_severe[which]) {
        std::cerr << "Failed unpacking archive: error code #" << which << std::endl;
        exit(EXIT_FAILURE); 
    } else {
        std::cerr << "Found non-severe error during unpacking: code #" << which << std::endl; 
    }
}
