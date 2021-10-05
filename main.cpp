#include <iostream>
#include "archiver/archiver.h"
#include "writer/writer.h"

using namespace std;

int main() {
    Archiver archiver("../tests/a");

    string path = "build/a.cho";
    Writer writer(path);
    
    archiver.CompressTo(writer, true);    
    
    writer.Finish();
    return 0;
}
