#include <cstring>
#include <iostream>
#include <unordered_map>
#include "error_handler/error_handler.h"
#include "archiver/archiver.h"
#include "writer/writer.h"

using namespace std;

const string COMPRESS_ARG = "-c";
const string DECOMPRESS_ARG = "-d";
const string HELP_ARG = "-h";
const string HELP_TEXT = "Compressing files [file1, file2, ...] to file with name 'archive_name': 'archiver -c archive_name file1 [file2 ...]'.\n"
                         "Decompressing file archive_name to current folder: 'archiver -d archive_name'.";
void ShowHelpAndExit() {
    std::cout << HELP_TEXT << std::endl;
    exit(EXIT_SUCCESS);
}

pair<bool, vector<string> > ParseArguments(int argc, char* argv[]) {
    if (argc == 1) {
        ErrorHandler::foundError(ErrorHandler::NO_ARGUMENTS);
        return {false, vector<string>()};
    } else {
        vector<string> result;
        bool has_compress, has_decompress;
        for (int i = 1; i < argc; ++i) {
            if (strcmp(argv[i], "-c") == 0) {
                has_compress = true;
            } 
            if (strcmp(argv[i], "-d") == 0) {
                has_decompress = true;
            }
            if (strcmp(argv[i], "-h") == 0) {
                ShowHelpAndExit();
            }
            result.push_back(argv[i]);
        }
        if (has_compress && has_decompress) {
            ErrorHandler::foundError(ErrorHandler::CONFUSING_ARGUMENTS);
        }
        if (has_compress && result.size() < 3) {
            ErrorHandler::foundError(ErrorHandler::WRONG_ARGUMENTS);
        }
        if (has_decompress && result.size() < 2) {
            ErrorHandler::foundError(ErrorHandler::WRONG_ARGUMENTS);
        }
        return {has_compress, result};
    }
}

int main(int argc, char* argv[]) {
    pair<bool, vector<string> > p = ParseArguments(argc, argv);
    bool is_compress = p.first;
    vector<string> args = p.second;
    if (is_compress) {
        Writer writer(args[1]);
        for (size_t i = 2; i < args.size(); ++i) {
            std::cout << args[i] << std::endl;
            Archiver archiver(args[i]);
            archiver.CompressTo(writer, (i + 1 == args.size()));
        }       
        writer.Finish();
    } else {
        Archiver archiver(args[1]);
        archiver.DecompressTo(".");
    }
    return 0;
}
