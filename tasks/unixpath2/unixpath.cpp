#include "unixpath.h"
#include <unordered_map>
#include <vector>

using std::min;
using std::string;
using std::string_view;
using std::vector;

const char PATH_DELIMITER = '/';
const std::regex SPLIT_REGEX("/");

UnixPath::UnixPath(string_view initial_dir) {
    this->initial_dir_ = Normalize(initial_dir.data());
    this->absolute_path_ = this->initial_dir_;
    this->relative_path_ = ".";
}

string UnixPath::GetAbsolutePath() const {
    return this->absolute_path_;
};

string UnixPath::GetRelativePath() const {
    return this->relative_path_;
};

void UnixPath::ChangeDirectory(string_view path) {
    if (!path.empty() && path[0] == PATH_DELIMITER) {
        // it's absolute
        this->absolute_path_ = Normalize(path.data());
        this->relative_path_ = GetRelation(this->initial_dir_, this->absolute_path_);
    } else {
        // it's relative
        this->absolute_path_ = GetAbsolutePath(this->absolute_path_, Normalize(path.data()));
        this->relative_path_ = GetRelation(this->initial_dir_, this->absolute_path_);
    }
}

string UnixPath::GetAbsolutePath(string_view id, string_view rp) {
    vector<string> split_initial_dir = UnixPath::Split(id.data());
    vector<string> split_relative_path = UnixPath::Split(rp.data());

    for (string element : split_relative_path) {
        if (element.empty() || element == ".") {
            continue;
        } else if (element == "..") {
            if (split_initial_dir.size() > 1) {
                split_initial_dir.pop_back();
            }
        } else {
            split_initial_dir.push_back(element);
        }
    }
    string result;
    for (string element : split_initial_dir) {
        result += element;
        result += "/";
    }
    if (result.size() > 1 && result.back() == '/') {
        result.pop_back();
    }
    return result;
}

string UnixPath::Normalize(const string path) {
    // removing multiple slashes, multiple ././ and / in the end
    if (path.empty()) {
        return ".";
    }
    vector<string> split_path = UnixPath::Split(path);
    vector<string> normalized_split_path;
    normalized_split_path.push_back(split_path[0]);
    bool is_previous_dot = false;
    for (size_t i = 1; i < split_path.size(); ++i) {
        if (split_path[i].empty()) {
            continue;
        }
        if (split_path[i] == ".") {
            if (!is_previous_dot) {
                normalized_split_path.push_back(split_path[i]);
            }
            is_previous_dot = true;
        } else {
            normalized_split_path.push_back(split_path[i]);
            is_previous_dot = false;
        }
    }
    string result;
    for (size_t i = 0; i < normalized_split_path.size(); ++i) {
        result += normalized_split_path[i];
        if (i + 1 < normalized_split_path.size()) {
            result += "/";
        }
    }
    return result;
}

string UnixPath::GetRelation(string_view id, string_view ap) {
    vector<string> split_initial_dir = UnixPath::Split(id.data());
    vector<string> split_absolute_path = UnixPath::Split(ap.data());
    size_t mx_i = 0;
    size_t check_size = min(split_initial_dir.size(), split_absolute_path.size());
    for (size_t i = 0; i < check_size; ++i) {
        if (split_initial_dir[i] == split_absolute_path[i]) {
            mx_i = i;
        }
    }
    string result;
    if (check_size == 0 || mx_i + 1 == check_size) {
        result = "./";
    } else {
        for (size_t i = mx_i + 1; i < check_size; ++i) {
            result += "../";
        }
    }
    for (size_t i = mx_i + 1; i < split_absolute_path.size(); ++i) {
        result += split_absolute_path[i];
        result += '/';
    }
    if (result.size() > 1 && result.back() == '/') {
        result.pop_back();
    }
    return result;
}

vector<string> UnixPath::Split(const string str) {
    if (this->split_cache_.count(str) == 1) {
        return this->split_cache_.at(str);
    }
    std::vector<string> list(std::sregex_token_iterator(str.begin(), str.end(), SPLIT_REGEX, -1),
                             std::sregex_token_iterator());
    std::pair<std::string, std::vector<string> > p = {str, list};
    this->split_cache_.insert(p);
    return list;
}
