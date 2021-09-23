#include "tests_checking.h"
#include <deque>

using std::deque;
using std::string;
using std::vector;

std::vector<std::string> StudentsOrder(const std::vector<StudentAction>& student_actions,
                                       const std::vector<size_t>& queries) {
    deque<string> papers;
    vector<string> result;
    for (StudentAction action : student_actions) {
        if (action.side == Side::Top) {
            papers.push_front(action.name);
        } else {
            papers.push_back(action.name);
        }
    }
    for (size_t query : queries)
        result.push_back(papers.at(query - 1));
    return result;
}
