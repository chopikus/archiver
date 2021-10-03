#include <iostream>
#include "archiver/archiver.h"

using namespace std;

int main() {
    Archiver archiver("../abacaba");
    archiver.CompressTo("abacaba.cho"); 
    return 0;
}
