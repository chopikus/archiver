#include "scorer.h"
#include <iostream>
#include <algorithm>

using std::map;
using std::set;
using std::sort;
using std::string;
using std::tie;

struct StudentTask {
    string student_name;
    string task_name;
    bool operator<(const StudentTask& another) const {
        return tie(student_name, task_name) < tie(another.student_name, another.task_name);
    }
};

map<StudentTask, bool> last_check_success;
map<StudentTask, int> opened_merge_requests;
map<string, set<string> > tasks_solved_by_student;

bool compare_by_time(const Event& event1, const Event& event2) {
    return (event1.time < event2.time);
}

ScoreTable GetScoredStudents(const Events& events, time_t score_time) {
    Events sorted_events = events;
    sort(sorted_events.begin(), sorted_events.end(), compare_by_time);
    for (Event event : sorted_events) {
        if (event.time > score_time) {
            break;
        }
        StudentTask student_task = StudentTask{.student_name = event.student_name, .task_name = event.task_name};
        switch (event.event_type) {
            case EventType::CheckSuccess:
                last_check_success[student_task] = true;
                break;
            case EventType::CheckFailed:
                last_check_success[student_task] = false;
            case EventType::MergeRequestOpen:
                ++opened_merge_requests[student_task];
            case EventType::MergeRequestClosed:
                --opened_merge_requests[student_task];
            default:
                break;
        }
    }
    for (auto [student_task, last_check] : last_check_success) {
        if (last_check && opened_merge_requests[student_task] == 0) {
            tasks_solved_by_student[student_task.student_name].insert(student_task.task_name);
        }
    }
    return tasks_solved_by_student;
}
