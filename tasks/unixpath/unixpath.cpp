#include <iostream>
#include <regex>
#include <stack>
#include "unixpath.h"
#include <vector>

using std::stack;
using std::string;
using std::string_view;
using std::vector;

std::vector<std::string> SplitByRegex(const std::string str, const std::string regex_str) {
    std::regex regexz(regex_str);
    std::vector<std::string> list(std::sregex_token_iterator(str.begin(), str.end(), regexz, -1),
                                  std::sregex_token_iterator());
    return list;
}

std::string NormalizePath(std::string_view current_working_dir, std::string_view path) {
    if (path[0] == '/') {
        return NormalizePath(path, "");
    }
    vector<string> split_cur_dir = SplitByRegex(current_working_dir.data(), "/");
    stack<string> dirs;
    bool is_empty_previous = false;
    for (const string& between_slashes : split_cur_dir) {
        if (between_slashes.empty()) {
            if (is_empty_previous) {
                continue;
            }
            is_empty_previous = true;
        } else {
            is_empty_previous = false;
        }
        dirs.push(between_slashes);
    }
    vector<string> split_path = SplitByRegex(path.data(), "/");
    for (const string& between_slashes : split_path) {
        if (between_slashes.empty() || between_slashes == ".") {
            continue;
        } else if (between_slashes == "..") {
            if (dirs.size() > 1) {
                dirs.pop();
            }
        } else {
            dirs.push(between_slashes);
        }
    }
    if (dirs.size() == 1) {
        return "/";
    }
    stack<string> reversed_dirs;
    while (!dirs.empty()) {
        reversed_dirs.push(dirs.top());
        dirs.pop();
    }
    string result;
    while (!reversed_dirs.empty()) {
        result += reversed_dirs.top();
        reversed_dirs.pop();
        if (!reversed_dirs.empty()) {
            result += "/";
        }
    }
    while (result.back() == '/') {
        result.pop_back();
    }
    return result;
}
