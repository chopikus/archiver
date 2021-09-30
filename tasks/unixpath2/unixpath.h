#pragma once

#include <regex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

class UnixPath {
public:
    UnixPath(std::string_view initial_dir);

    void ChangeDirectory(std::string_view path);

    std::string GetAbsolutePath() const;
    std::string GetRelativePath() const;

private:
    std::string GetRelation(std::string_view id, std::string_view ap);
    std::string GetAbsolutePath(std::string_view id, std::string_view rp);
    std::string Normalize(std::string path);
    std::vector<std::string> Split(const std::string path);
    std::unordered_map<std::string, std::vector<std::string> > split_cache_;

    std::string initial_dir_;
    std::string relative_path_;
    std::string absolute_path_;
};
