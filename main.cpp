#include <iostream>
#include "archiver/archiver.h"
#include "writer/writer.h"

using namespace std;

int main() {
    Archiver archiver("a");
    Writer writer("a_compressed3");
    archiver.CompressTo(writer, true);
    
    Archiver archiver2("a");
    archiver2.CompressTo(writer, true);
    writer.Finish();
    return 0;
}
