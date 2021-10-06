#include <cstring>
#include <iostream>
#include <unordered_map>
#include "archiver/archiver.h"
#include "writer/writer.h"

using namespace std;

string HELP_TEXT = "Compressing files [file1, file2, ...] to file with name 'archive_name': 'archiver -c archive_name file1 [file2 ...]'.\n"
"Decompressing file archive_name to current folder: 'archiver -d archive_name'.";
void ShowHelpAndExit() {
    std::cout << HELP_TEXT << std::endl;
    exit(EXIT_SUCCESS);
}

pair<bool, vector<string> > ParseArguments(int argc, char* argv[]) {
    if (argc == 1) {
        throw "no arguments were provided!";
        return {false, vector<string>()};
    } else {
        vector<string> result;
        bool has_compress = false, has_decompress = false;
        for (int i = 1; i < argc; ++i) {
            if (strcmp(argv[i], "-c") == 0) {
                has_compress = true;
            } else if (strcmp(argv[i], "-d") == 0) {
                has_decompress = true;
            } else if (strcmp(argv[i], "-h") == 0) {
                ShowHelpAndExit();
            } else {
                result.push_back(argv[i]);
            }
        }
        if (has_compress && has_decompress) {
            throw "confusing arguments! use -h for help";
        }
        if (has_compress && result.size() < 2) {
            throw "wrong arguments! use -h for help";
        }
        if (has_decompress && result.size() < 1) {
            throw "wrong arguments! use -h for help";
        }
        return {has_compress, result};
    }
}

int main(int argc, char* argv[]) {
    pair<bool, vector<string> > p = ParseArguments(argc, argv);
    bool is_compress = p.first;
    vector<string> files = p.second;
    if (is_compress) {
        try {
            string compress_path = files[0];
            files.erase(files.begin());
            Archiver archiver(files);
            archiver.CompressTo(compress_path);
        } catch (const char* e) {
            std::cerr << "ERROR: " << e << std::endl;
        }
    } else {
        try {
        Archiver archiver(files);
        archiver.Decompress();
        } catch (const char * e) {
            std::cerr << "ERROR: " << e << std::endl;
        }
    }
    return 0;
}
