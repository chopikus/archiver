#include "associative_operation.h"

bool IsAssociative(const std::vector<std::vector<size_t>>& table) {
    for (size_t x = 0; x < table.size(); x++) {
        for (size_t y = 0; y < table.size(); y++) {
            for (size_t z = 0; z < table.size(); z++) {
                //(x*y)*z should be equal to x*(y*z)
                size_t xy_z = table[table[x][y]][z];  // (x*y)*z
                size_t x_yz = table[x][table[y][z]];  // x*(y*z)
                if (xy_z != x_yz) {
                    return false;
                }
            }
        }
    }
    return true;
}
