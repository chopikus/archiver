#pragma once

#include <cctype>
#include <cmath>
#include <set>
#include <string_view>
#include <vector>
#include <map>

class SearchEngine {
public:
    void BuildIndex(std::string_view text);
    std::vector<std::string_view> Search(std::string_view query, size_t results_count) const;

private:
    struct MapCompareLower {
        bool operator()(const std::string_view& a, const std::string_view& b) const {
            for (size_t i = 0; i < std::min(a.size(), b.size()); ++i) {
                if (tolower(a[i]) != tolower(b[i])) {
                    return tolower(a[i]) < tolower(b[i]);
                }
            }
            return a.size() < b.size();
        }
    };

    std::vector<std::string_view> Split(const std::string_view s, int (*fits)(int)) const;
    static bool UniqueCompareLower(const std::string_view& a, const std::string_view& b);

    std::vector<std::string_view> text_lines_;
    std::vector<std::vector<std::string_view> > text_line_words_;
    std::map<std::string_view, double, MapCompareLower> idf_by_word_;

    struct RelevanceIndex {
        double relevance = 0;
        size_t index = 0;
        bool operator<(RelevanceIndex& another) {
            double minus_relevance = -relevance;
            double minus_another_relevance = -another.relevance;
            return std::tie(minus_relevance, index) < std::tie(minus_another_relevance, another.index);
        }
    };
};
