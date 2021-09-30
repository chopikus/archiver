#include "scorer.h"
#include <iostream>
#include <algorithm>
#include <map>

using std::map;
using std::set;
using std::sort;
using std::string;
using std::tie;

void Scorer::OnCheckFailed(const StudentName& student_name, const TaskName& task_name) {
    Scorer::StudentTask student_task = StudentTask{.student_name = student_name, .task_name = task_name};
    last_check_success_[student_task] = false;
}

void Scorer::OnCheckSuccess(const StudentName& student_name, const TaskName& task_name) {
    Scorer::StudentTask student_task = StudentTask{.student_name = student_name, .task_name = task_name};
    last_check_success_[student_task] = true;
}

void Scorer::OnMergeRequestOpen(const StudentName& student_name, const TaskName& task_name) {
    Scorer::StudentTask student_task = StudentTask{.student_name = student_name, .task_name = task_name};
    ++opened_merge_requests_[student_task];
}

void Scorer::OnMergeRequestClosed(const StudentName& student_name, const TaskName& task_name) {
    Scorer::StudentTask student_task = StudentTask{.student_name = student_name, .task_name = task_name};
    if (opened_merge_requests_[student_task] > 0) {
        --opened_merge_requests_[student_task];
    }
}

ScoreTable Scorer::GetScoreTable() const {
    ScoreTable tasks_solved_by_student;
    for (auto [student_task, last_check] : last_check_success_) {
        if (last_check &&
            (opened_merge_requests_.count(student_task) == 0 || opened_merge_requests_.at(student_task) == 0)) {
            tasks_solved_by_student[student_task.student_name].insert(student_task.task_name);
        }
    }
    return tasks_solved_by_student;
}

void Scorer::Reset() {
    last_check_success_.clear();
    opened_merge_requests_.clear();
}

/*ScoreTable GetScoredStudents(const Events& events, time_t score_time) {

    Events sorted_events = events;
    sort(sorted_events.begin(), sorted_events.end(), CompareByTime);
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
                break;
            case EventType::MergeRequestOpen:
                ++opened_merge_requests[student_task];
                break;
            case EventType::MergeRequestClosed:
                --opened_merge_requests[student_task];
                break;
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
}*/
