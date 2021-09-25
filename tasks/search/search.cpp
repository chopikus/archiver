#include <ctype.h>
#include <iostream>
#include <math.h>
#include "search.h"
#include <tuple>
#include <unordered_map>
#include <set>

using std::pair;
using std::string;
using std::string_view;
using std::unordered_map;
using std::vector;

struct RelevanceIndex {
    double relevance = 0;
    size_t index = 0;
    bool operator<(RelevanceIndex& another) {
        double minus_relevance = -relevance;
        double minus_another_relevance = -another.relevance;
        return std::tie(minus_relevance, index) < std::tie(minus_another_relevance, another.index);
    }
};

int LineSplitter(int ch) {
    return (ch != '\n');
}

vector<string> Split(const string_view& s, int (*fits)(int)) {
    string buf;
    vector<string> result;
    bool was_alpha_symbol = false;
    for (char ch : s) {
        if (fits(ch)) {
            was_alpha_symbol |= isalpha(ch);
            buf += tolower(ch);
        } else {
            if (!buf.empty() && was_alpha_symbol) {
                result.push_back(buf);
            }
            buf = "";
            was_alpha_symbol = false;
        }
    }
    if (!buf.empty() && was_alpha_symbol) {
        result.push_back(buf);
    }
    return result;
}

vector<pair<size_t, size_t> > SplitIndices(const string_view& s, int (*fits)(int)) {
    size_t start = 0;
    size_t len = 0;
    bool was_alpha_symbol = false;
    vector<pair<size_t, size_t> > result;
    for (size_t i = 0; i < s.size(); ++i) {
        if (fits(s[i])) {
            was_alpha_symbol |= isalpha(s[i]);
            ++len;
        } else {
            if (len > 0 && was_alpha_symbol) {
                result.push_back({start, len});
            }
            start = i + 1;
            len = 0;
            was_alpha_symbol = false;
        }
    }
    if (len > 0 && was_alpha_symbol) {
        result.push_back({start, len});
    }
    return result;
}

vector<string_view> Search(string_view text, string_view query, size_t results_count) {
    vector<string> text_lines = Split(text, LineSplitter);
    vector<string> query_words = Split(query, isalpha);
    std::set<string> diff_query_words(query_words.begin(), query_words.end());
    vector<RelevanceIndex> relevance_on_lines;
    unordered_map<string, double> idf_by_word;
    for (const string& line : text_lines) {
        vector<string> line_words = Split(line, isalpha);
        for (const string& query_word : diff_query_words) {
            for (const string& line_word : line_words) {
                if (line_word == query_word) {
                    ++idf_by_word[query_word];
                    break;
                }
            }
        }
    }
    for (const string& query_word : diff_query_words) {
        if (idf_by_word[query_word] != 0 && !text_lines.empty()) {
            idf_by_word[query_word] = std::log(static_cast<double>(text_lines.size()) / idf_by_word[query_word]);
        }
    }
    size_t i = 0;
    for (const string& line : text_lines) {
        vector<string> line_words = Split(line, isalpha);
        unordered_map<string, int> word_counter;
        for (const string& word : line_words) {
            word_counter[word]++;
        }
        double relevance = 0;
        for (const string& word : query_words) {
            double tf = word_counter[word];
            if (!line_words.empty()) {
                tf /= line_words.size();
            }
            relevance += tf * idf_by_word[word];
        }
        if (relevance > 0) {
            relevance_on_lines.push_back({.relevance = relevance, .index = i});
        }
        ++i;
    }
    sort(relevance_on_lines.begin(), relevance_on_lines.end());
    vector<string_view> result;
    vector<pair<size_t, size_t> > text_line_indlen = SplitIndices(text, LineSplitter);
    for (size_t i = 0; i < std::min(results_count, relevance_on_lines.size()); ++i) {
        pair<size_t, size_t> indlen = text_line_indlen[relevance_on_lines[i].index];
        result.push_back(text.substr(indlen.first, indlen.second));
    }
    return result;
}
