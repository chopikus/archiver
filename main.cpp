#include <iostream>
#include "archiver/archiver.h"
#include "writer/writer.h"

using namespace std;

int main() {
    Archiver archiver("../a");

    string path = "../a.cho";
    Writer writer(path);
    
    archiver.CompressTo(writer);    
    
    writer.Finish();
    return 0;
}
