#include "search.h"
#include <iostream>

using std::min;
using std::string_view;
using std::map;
using std::vector;

int LineSplitter(int ch) {
    return (ch != '\n');
}

bool SearchEngine::UniqueCompareLower(const string_view& a, const string_view& b) {
    for (size_t i = 0; i < min(a.size(), b.size()); ++i) {
        if (tolower(a[i]) != tolower(b[i])) {
            return false;
        }
    }
    return a.size() == b.size();
}

void SearchEngine::BuildIndex(string_view text) {

    text_lines_.clear();
    text_line_words_.clear();
    idf_by_word_.clear();
        
    text_lines_ = SearchEngine::Split(text, LineSplitter);
    for (const string_view& line : text_lines_) {
        vector<string_view> line_words = Split(line, isalpha);
        text_line_words_.push_back(line_words);
        auto line_words_end = unique(line_words.begin(), line_words.end(), UniqueCompareLower);
        for (auto it = line_words.begin(); it != line_words_end; ++it) {
            ++idf_by_word_[*it]; // TODO comparator
        }
    }
    for (auto & [word, count] : idf_by_word_) {
        if (count != 0 && !text_lines_.empty()) {
            count = std::log(static_cast<double>(text_lines_.size()) / count);
        }
    }
}

vector<string_view> SearchEngine::Search(string_view query, size_t results_count) const {

    vector<string_view> query_words = SearchEngine::Split(query, isalpha);
    auto query_words_end = unique(query_words.begin(), query_words.end(), UniqueCompareLower);
    vector<SearchEngine::RelevanceIndex> relevance_on_lines;

    size_t i = 0;
    for (auto line_words : text_line_words_) {
        map<string_view, int, MapCompareLower> word_counter; //TODO comparator
        for (string_view word : line_words) {
            word_counter[word]++;
        }
        double relevance = 0;
        for (auto it = query_words.begin(); it != query_words_end; ++it) {
            string_view word = *it;
            double tf = word_counter[word];
            if (!line_words.empty()) {
                tf /= line_words.size();
            }
            if (idf_by_word_.count(word) > 0) {
                relevance += tf * idf_by_word_.at(word);
            }
        }
        if (relevance > 0) {
            relevance_on_lines.push_back({.relevance = relevance, .index = i});
        }
        ++i;
    }
    sort(relevance_on_lines.begin(), relevance_on_lines.end());
    vector<string_view> result;
    for (size_t i = 0; i < std::min(results_count, relevance_on_lines.size()); ++i) {
        result.push_back(text_lines_[relevance_on_lines[i].index]);
    }
    return result;
}

vector<string_view> SearchEngine::Split(string_view s, int (*fits)(int)) const {
    vector<string_view> result;
    bool was_alpha_symbol = false;
    size_t l = 0, r = 0; //[l, r)
    for (size_t i = 0; i < s.size(); ++i) {
        if (fits(s[i])) {
            was_alpha_symbol |= isalpha(s[i]);
            r = i + 1;
        } else {
            if (r > l && was_alpha_symbol) {
                result.push_back(s.substr(l, r - l));
            }
            l = i+1;
            r = i+1;
            was_alpha_symbol = false;
        }
    }
    if (r > l && was_alpha_symbol) {
        result.push_back(s.substr(l, r - l));
    }
    return result;
}
