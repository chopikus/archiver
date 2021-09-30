#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

using std::map;

using StudentName = std::string;
using TaskName = std::string;

using ScoreTable = std::map<StudentName, std::set<TaskName>>;

class Scorer {
public:
    void OnCheckFailed(const StudentName& student_name, const TaskName& task_name);
    void OnCheckSuccess(const StudentName& student_name, const TaskName& task_name);
    void OnMergeRequestOpen(const StudentName& student_name, const TaskName& task_name);
    void OnMergeRequestClosed(const StudentName& student_name, const TaskName& task_name);
    void Reset();

    ScoreTable GetScoreTable() const;

private:
    struct StudentTask {
        StudentName student_name;
        TaskName task_name;
        bool operator<(const StudentTask& another) const {
            return tie(student_name, task_name) < tie(another.student_name, another.task_name);
        }
    };
    map<StudentTask, bool> last_check_success_;
    map<StudentTask, int> opened_merge_requests_;
};
