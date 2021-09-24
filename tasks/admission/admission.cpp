#include "admission.h"
#include <map>
#include <algorithm>

using std::map;
using std::string;
using std::tie;
using std::vector;

bool CompareApplicants(const Applicant* a, const Applicant* b) {
    int minus_a_points = -a->points;
    int minus_b_points = -b->points;
    const Student sa = a->student;
    const Student sb = b->student;
    return tie(minus_a_points, sa.birth_date.year, sa.birth_date.month, sa.birth_date.day, sa.name) <
           tie(minus_b_points, sb.birth_date.year, sb.birth_date.month, sb.birth_date.day, sb.name);
}

bool CompareStudents(const Student* a, const Student* b) {
    return tie(a->name, a->birth_date.year, a->birth_date.month, a->birth_date.day) <
           tie(b->name, b->birth_date.year, b->birth_date.month, b->birth_date.day);
}

AdmissionTable FillUniversities(const std::vector<University>& universities, const std::vector<Applicant>& applicants) {
    map<string, int> cur_uni_capacity;
    vector<const Applicant*> sorted_applicants;
    AdmissionTable result;
    for (const Applicant& applicant : applicants) {
        sorted_applicants.push_back(&applicant);
    }
    sort(sorted_applicants.begin(), sorted_applicants.end(), CompareApplicants);
    for (const University& university : universities) {
        cur_uni_capacity[university.name] = university.max_students;
    }
    for (const Applicant* applicant : sorted_applicants) {
        for (const string& uni_wanted : applicant->wish_list) {
            if (cur_uni_capacity[uni_wanted] > 0) {
                --cur_uni_capacity[uni_wanted];
                result[uni_wanted].push_back(&(applicant->student));
                break;
            }
        }
    }
    for (auto [uni, students] : result) {
        sort(students.begin(), students.end(), CompareStudents);
    }
    return result;
}
